/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 02:33:21 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/08 06:06:31 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_redir(t_cmd *cmd, t_token *token, int flags)
{
	char	*filename;
	int		fd;

	filename = ft_filename(token->next->line, g_minishell.env, &(cmd->err));
	token->next->token = -1;
	if (!filename || (cmd->err && ft_free(filename)))
		cmd->err = ft_error(token->next->line, 0, ": ambiguous redirect\n", 0);
	else
	{
		fd = open(filename, flags, 00777);
		if (fd == -1)
			cmd->err = ft_error(filename, 0, 0, 0);
		else if (token->token == 3 || token->token == 4)
			cmd->fdout = ft_changefd(fd, cmd->fdout);
		else if (token->token == 5)
			cmd->fdin = ft_changefd(fd, cmd->fdin);
		free(filename);
	}
	if (cmd->err)
		g_minishell.return_value = 1;
	return (token->next->next);
}

t_token	*ft_pipe(t_cmd *cmd, t_token *token)
{
	int	fd[2];

	cmd->forked = 1;
	pipe(fd);
	cmd->next = cmd_init(fd[0], 1);
	cmd->next->forked = 1;
	cmd->fdout = fd[1];
	cmd->next->tokens = token->next;
	token->next = 0;
	return (cmd->next->tokens);
}

void	exectoken2(t_cmd *cmd, t_token *token, t_env **env)
{
	while (cmd)
	{
		token = cmd->tokens;
		while (token)
		{
			if (!(token->token))
				token = token_to_word(token, token->line, *env);
			else if (token->token == 3 && !cmd->err)
				token = ft_redir(cmd, token, O_CREAT | O_WRONLY | O_TRUNC);
			else if (token->token == 4 && !cmd->err)
				token = ft_redir(cmd, token, O_CREAT | O_WRONLY | O_APPEND);
			else if (token->token == 5 && !cmd->err)
				token = ft_redir(cmd, token, O_RDONLY);
			else
				token = token->next;
		}
		if (!cmd->err)
			cmdexec(cmd, env, cmd->tokens);
		ft_changefd(0, cmd->fdin);
		ft_changefd(1, cmd->fdout);
		cmd = cmd->next;
	}
}

int	exectoken(t_cmd *cmd, t_token *token, t_env **env)
{
	int	status;

	while (token)
	{
		if (token->token == 1)
		{
			token = ft_pipe(cmd, token);
			cmd = cmd->next;
		}
		else if (token->token == 6)
			token = ft_input(cmd, token);
		else
			token = token->next;
		if (cmd->err)
			return (1);
	}
	exectoken2(g_minishell.command, 0, env);
	cmd = g_minishell.command;
	while (cmd)
	{
		if (cmd->forked && cmd->pid && waitpid(cmd->pid, &status, 0))
			g_minishell.return_value = WEXITSTATUS(status);
		cmd = cmd->next;
	}
	return (0);
}

int	parse_line(char *line, t_env **env)
{
	t_cmd	*cmd;
	t_cmd	*firstcmd;
	t_token	**nexttoken;

	cmd = cmd_init(-1, 1);
	g_minishell.command = cmd;
	firstcmd = cmd;
	nexttoken = &(cmd->tokens);
	while (*line)
	{
		while (ft_isspace(*line))
			*(line++) = 0;
		if (*line)
			nexttoken = ft_gettoken(nexttoken, &line);
	}
	if (checktokenlist(cmd->tokens, -1) || exectoken(cmd, cmd->tokens, env))
		return (freecmd(cmd, 0));
	return (0 * freecmd(cmd, 1));
}
