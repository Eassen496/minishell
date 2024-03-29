/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   command.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 02:30:42 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/08 06:00:00 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_cmd	*cmd_init(int fdin, int fdout)
{
	t_cmd	*res;

	res = malloc(sizeof(t_cmd));
	res->fdin = fdin;
	res->fdout = fdout;
	res->pid = 0;
	res->forked = 0;
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
		cmd->fdin = ft_changefd(0, cmd->fdin);
		cmd->fdout = ft_changefd(1, cmd->fdout);
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

void	cmdexec(t_cmd *cmd, t_env **env, t_token *token)
{
	while (token && token->token)
		token = token->next;
	if (!token)
		return ;
	if (cmd->forked)
		cmd->pid = fork();
	if (!cmd->pid && ft_strcmp(token->line, "echo"))
		ft_echo(token->next, cmd);
	else if (!cmd->pid && ft_strcmp(token->line, "cd"))
		ft_cd(env, token->next, 0, "HOME");
	else if (!cmd->pid && ft_strcmp(token->line, "pwd"))
		ft_pwd(cmd->fdout);
	else if (!cmd->pid && ft_strcmp(token->line, "export"))
		ft_export(env, token->next);
	else if (!cmd->pid && ft_strcmp(token->line, "unset"))
		ft_unset(env, token->next);
	else if (!cmd->pid && ft_strcmp(token->line, "env"))
		print_env(*env, cmd->fdout);
	else if (!cmd->pid && ft_strcmp(cmd->tokens->line, "exit"))
		ft_exit(cmd->tokens->next);
	else if (!cmd->pid)
		ft_exec(cmd, *env, token, "PATH");
	if (!cmd->pid && cmd->forked)
		exit (g_minishell.return_value);
}
