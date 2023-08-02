/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:37:01 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/02 03:33:02 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_pwd(int fdout)
{
	char	*wd;

	wd = getcwd(0, 0);
	write(fdout, wd, ft_strlen(wd));
	write(fdout, "\n", 1);
	free(wd);
	g_minishell.return_value = 0;
}

void	ft_echo(t_token *token, t_cmd *cmd)
{
	int	i;
	int	newline;

	i = 1;
	newline = 1;
	while (token && token->token)
		token = token->next;
	if (token && token->line[0] == '-')
	{
		while (token->line[i] == 'n')
			i++;
		if (token->line[i] == 0 && newline--)
			token = token->next;
	}
	while (token)
	{
		if (!(token->token))
			write(cmd->fdout, token->line, ft_strlen(token->line));
		token = token->next;
		if (token && !(token->token))
			write(cmd->fdout, " ", 1);
	}
	if (newline)
		write(cmd->fdout, "\n", 1);
	g_minishell.return_value = 0;
}

void	ft_freexit(int n)
{
	if (g_minishell.command && g_minishell.command->next)
	{
		g_minishell.return_value = n;
		return ;
	}
	if (g_minishell.line)
		free(g_minishell.line);
	freecmd(g_minishell.command, 1);
	free_env(g_minishell.environment);
	close(g_minishell.historic);
	exit (n);
}

void	ft_exit(t_token *token)
{
	int		i;
	char	*tmp;

	i = 0;
	if (!(g_minishell.command && g_minishell.command->next))
		write(2, "exit\n", 5);
	while (token && token->token)
		token = token->next;
	if (!token)
		ft_freexit(g_minishell.return_value);
	else if (ft_atoiexit(token->line) == 300)
	{
		tmp = ft_strjoin("minishell: exit: ", token->line, 0);
		tmp = ft_strjoin(tmp, ": numeric argument required\n", 1);
		write(2, tmp, ft_strlen(tmp));
		free(tmp);
		ft_freexit(255);
	}
	else if (token->next && token)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		g_minishell.return_value = 1;
	}
	else
		ft_freexit(ft_atoiexit(token->line));
}

char	**ft_buildarg(t_token *token, char *file)
{
	t_token	*current;
	char	**res;
	int		i;

	current = token;
	i = 1;
	while (current)
	{
		if (!(current->token))
			i++;
		current = current->next;
	}
	res = malloc((i + 1) * sizeof(char *));
	res[0] = file;
	res[i]= 0;
	current = token;
	i = 1;
	while (current)
	{
		if (!(current->token))
			res[i++] = current->line;
		printf("->%s\n", current->line);
		current = current->next;
	}
	return (res);
}

char	**ft_buildenv(t_env	*env)
{
	int		i;
	t_env	*current;
	char	**res;

	current = env;
	i = 0;
	while (current)
	{
		i++;
		current = current->next;
	}
	res = malloc((i + 1) * sizeof(char *));
	res[i] = 0;
	current = env;
	i = 0;
	while (current)
	{
		res[i++] = current->nameval;
		current = current->next;
	}
	return (res);
}

int	ft_execve(t_cmd *cmd, t_env *env, t_token *token, char *file)
{
	int		pid;
	char	**arg;
	char	**tabenv;
	int		status;

	(void) cmd;
	pid = fork();
	if (pid)
	{
		signal(SIGINT, SIG_IGN);
		waitpid(pid, &status, 0);
		signal(SIGINT, sig_handler);
		g_minishell.return_value = WEXITSTATUS(status);
		return (status);
	}
	arg = ft_buildarg(token->next, token->line);
	tabenv = ft_buildenv(env);
	dup2(cmd->fdin, 0);
	dup2(cmd->fdout, 1);
	execve(file, arg, tabenv);
	free(arg);
	free(tabenv);
	exit (errno);
}

int	ft_exec(t_cmd *cmd, t_env *env, t_token *token)
{
	char	*file;
	int		fd[2];
	char	*path;
	int		i[2];

	if (ft_contain(token->line, '/'))
		return (ft_execve(cmd, env, token, token->line));
	pipe(fd);
	path = "PATH";
	path = pipetostr(fd, ft_getenv(env, &path, fd[1], 0), 1);
	i[0] = 0;
	while (path[i[0]] || !ft_free(file))
	{
		i[1] = 0;
		while (path[i[0] + i[1]] && path[i[0] + i[1]] != ':')
			i[1]++;
		if (path[i[0] + i[1]] == ':')
			path[i[0] + i[1]++] = 0;
		file = ft_strjoin(&(path[i[0]]), ft_strjoin("/", token->line, 0), 2);
		if (!access(file, F_OK) && ft_free(path))
			return (ft_execve(cmd, env, token, file) + ft_free(file));
		i[0] += i[1];
	}
	g_minishell.return_value = 127;
	return (ft_error(token->line, 0, ": command not found\n", 0));
}
