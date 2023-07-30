/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <ale-roux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/08 22:28:41 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 03:29:13 by ale-roux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_export(t_env **env, t_token *token)
{
	int	i;

	while (token)
	{
		i = -1;
		while (token->line[++i])
		{
			if (token->line[i] == '=')
			{
				add_env(env, *env, token->line);
				g_minishell.return_value = 0;
				break ;
			}
			else if (ft_isamong(token->line[i], "!@#$^*(){}[]:\'\""))
			{
				write(2, "minishell: export: '", 20);
				write(2, token->line, ft_strlen(token->line));
				write(2, "' : not a valid identifier\n", 27);
				g_minishell.return_value = 1;
				break ;
			}
		}
		token = token->next;
	}
}

void	ft_updatepwd(t_env **env)
{
	char	*wd;
	int		fd[2];

	pipe(fd);
	wd = "PWD";
	write(fd[1], "OLDPWD=", 7);
	wd = pipetostr(fd, ft_getenv(*env, &wd, fd[1], 0) + 7, 0);
	add_env(env, *env, wd);
	free(wd);
	wd = ft_strjoin("PWD=", getcwd(0, 0), 2);
	add_env(env, *env, wd);
	free(wd);
	g_minishell.return_value = 0;
}

void	ft_cd(t_env **env, t_token *token, int tofree)
{
	char	*path;
	int		fd[2];

	path = "HOME";
	while (token && token->token)
		token = token->next;
	if (!token || !(token->line))
	{
		pipe(fd);
		path = pipetostr(fd, ft_getenv(*env, &path, fd[1], 0), 1);
		tofree = 2;
	}
	else
		path = token->line;
	if (!path)
	{
		write(2, "minishell: cd: HOME not set\n", 28);
		g_minishell.return_value = 1;
	}
	else if (chdir(path) == -1)
	{
		path = ft_strjoin("minishell: cd: ", path, tofree);
		perror(path);
		g_minishell.return_value = 1;
	}
	else
	{
		ft_updatepwd(env);
		g_minishell.return_value = 0;
	}
	if (tofree && path)
		free(path);
}

void	ft_unset(t_env **env, t_token *token)
{
	int	i;

	while (token)
	{
		if ((!token->token))
		{
			i = -1;
			while (token->line[++i])
			{
				if (ft_isamong(token->line[i], "!@#$^*(){}[]:\'\"="))
				{
					write(2, "minishell: unset: '", 19);
					write(2, token->line, ft_strlen(token->line));
					write(2, "' : not a valid identifier\n", 27);
					g_minishell.return_value = 1;
					break ;
				}
			}
			if (!token->line[i])
			{
				remove_env(env, *env, token->line);
				g_minishell.return_value = 0;
			}
		}
		token = token->next;
	}
}
