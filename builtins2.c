/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:37:01 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 04:17:14 by abitonti         ###   ########.fr       */
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
}

char	**ft_buildarg(t_token *token)
{
	t_token	*current;
	char	**res;
	int		i;

	current = token;
	i = 0;
	while (current)
	{
		if (!(current->token))
			i++;
		current = current->next;
	}
	res = malloc((i + 1) * sizeof(char *));
	res[i]= 0;
	current = token;
	i = 0;
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
	res[i]= 0;
	current = env;
	i = 0;
	while (current)
	{
		res[i++] = current->nameval;
		current = current->next;
	}
	return (res);
}

int	ft_execve(t_cmd *cmd, t_env *env, t_token *token, char file)
{
	int		pid;
	char	**arg;
	char	**tabenv;

	pid = fork();
	if (pid)
		return (waitpid(pid, 0, 0));
	arg = ft_buildarg(token->next);
	tabenv = ft_buildenv(env);
	execve(file, arg, tabenv);
	free(arg);
	free(tabenv);
}

int	ft_exec(t_cmd *cmd, t_env *env, t_token *token)
{
	char	*file;

	if (!access(token->line, F_OK))
		return (ft_execve(cmd, env, token, token->line));
	file = ft_strjoin("/bin/", token->line, 0);
	if (!access(file, F_OK))
		return (ft_execve(cmd, env, token, file) + ft_free(file));
	free(file);
	ft_error(token->line, 0, ": command not found\n", 0);
	return (1);
}