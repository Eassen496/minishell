/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins3.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 01:44:24 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/05 04:00:13 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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
	res[i] = 0;
	current = token;
	i = 1;
	while (current)
	{
		if (!(current->token))
			res[i++] = current->line;
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
		signal(SIGINT, ft_resetline);
		g_minishell.return_value = WEXITSTATUS(status);
		return (status);
	}
	arg = ft_buildarg(token->next, token->line);
	tabenv = ft_buildenv(env);
	dup2(cmd->fdin, 0);
	dup2(cmd->fdout, 1);
	execve(file, arg, tabenv);
	ft_error(file, 1, 0, 0);
	free(arg);
	free(tabenv);
	exit (errno);
}

int	ft_exec(t_cmd *cmd, t_env *env, t_token *token, char *path)
{
	int		fd[2];
	char	*file;
	int		i[2];

	if (ft_contain(token->line, '/'))
		return (ft_execve(cmd, env, token, token->line));
	pipe(fd);
	path = pipetostr(fd, ft_getenv(env, &path, fd[1], 0), 1);
	i[0] = 0;
	while (path && path[i[0]])
	{
		i[1] = 0;
		while (path[i[0] + i[1]] && path[i[0] + i[1]] != ':')
			i[1]++;
		if (path[i[0] + i[1]] == ':')
			path[i[0] + i[1]++] = 0;
		file = ft_strjoin(&(path[i[0]]), ft_strjoin("/", token->line, 0), 2);
		if ((!access(file, F_OK) && ft_free(path)) || !ft_free(file))
			return (ft_execve(cmd, env, token, file) + ft_free(file));
		i[0] += i[1];
	}
	g_minishell.return_value = 127;
	if (!path || (!(*path) && ft_free(path)) || !(ft_free(path)))
		return (ft_error(token->line, 0, ": No such file or directory\n", 0));
	return (ft_error(token->line, 0, ": command not found\n", 0));
}
