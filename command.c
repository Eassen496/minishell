/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 02:30:42 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/19 04:16:17 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_init(int fdin, int fdout)
{
	t_cmd	*res;

	res = malloc(sizeof(t_cmd));
	res->fdin = dup(fdin);
	res->fdout = dup(fdout);
	res->next = 0;
	res->tokens = 0;
	res->err = 0;
	return (res);
}

int	freecmd(t_cmd *cmd, int freeline)
{
	t_token	*token;
	void	*next;

	while (cmd)
	{
		token = cmd->tokens;
		if (cmd->fdin != -1)
			close(cmd->fdin);
		if (cmd->fdout != -1)
			close(cmd->fdout);
		while (token)
		{
			next = token->next;
			if (freeline && !(token->token) && token->line)
				free(token->line);
			free(token);
			token = next;
		}
		next = cmd->next;
		free(cmd);
		cmd = next;
	}
	return (1);
}

int	cmdexec(t_cmd *cmd, t_env **env)
{
	if (!(cmd->tokens))
		return (0);
	if (ft_strcmp(cmd->tokens->line, "echo"))
		ft_echo(cmd->tokens->next, cmd);
	else if (ft_strcmp(cmd->tokens->line, "cd"))
		ft_cd(env, cmd->tokens->next, 0);
	else if (ft_strcmp(cmd->tokens->line, "pwd"))
		ft_pwd(cmd->fdout);
	else if (ft_strcmp(cmd->tokens->line, "export"))
		ft_export(env, cmd->tokens->next);
	else if (ft_strcmp(cmd->tokens->line, "unset"))
		ft_unset(env, cmd->tokens->next);
	else if (ft_strcmp(cmd->tokens->line, "env"))
		print_env(*env, cmd->fdout);
	//else if (ft_strcmp(cmd->tokens->line, "exit"))
	//	ft_exit(cmd->tokens->next);
	else
		ft_error(cmd->tokens->line, 0, ": command not found\n", 0);
	return (1);
}
