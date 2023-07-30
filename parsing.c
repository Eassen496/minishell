/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 02:33:21 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 04:17:11 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_token	*ft_redir(t_cmd *cmd, t_env *env, t_token *token, int flags)
{
	char	*filename;
	int		fd;

	filename = ft_filename(token->next->line, env, &(cmd->err));
	token->next->token = -1;
	if (!filename || (cmd->err && ft_free(filename)))
		cmd->err = ft_error(token->next->line, 0, ": ambiguous redirect\n", 0);
	else
	{
		fd = open(filename, flags, 00777);
		if (fd == -1)
			cmd->err = ft_error(filename, 0, 0, 0);
		else if (token->token == 3 || token->token == 4)
			cmd->fdout = dup2(fd, cmd->fdout);
		else if (token->token == 5)
			cmd->fdin = dup2(fd, cmd->fdin);
		free(filename);
	}
	return (token->next->next);
}

t_token	*ft_pipe(t_cmd *cmd, t_token *token)
{
	int	fd[2];

	pipe(fd);
	cmd->fdout = dup2(fd[1], cmd->fdout);
	cmd->next = cmd_init(fd[0], 1);
	cmd->next->tokens = token->next;
	token->next = 0;
	return (cmd->next->tokens);
}

int	exectoken(t_cmd *cmd, t_token *token, t_env **env)
{
	while (token)
	{
		if (token->token == 1)
		{
			token = ft_pipe(cmd, token);
			cmdexec(cmd, env, cmd->tokens);
			cmd = cmd->next;
		}
		else if (!(token->token) || cmd->err)
			token = token_to_word(token, token->line, *env);
		else if (token->token == 3)
			token = ft_redir(cmd, *env, token, O_CREAT | O_WRONLY | O_TRUNC);
		else if (token->token == 4)
			token = ft_redir(cmd, *env, token, O_CREAT | O_WRONLY | O_APPEND);
		else if (token->token == 5)
			token = ft_redir(cmd, *env, token, O_RDONLY);
	}
	cmdexec(cmd, env, cmd->tokens);
	return (0);
}

int	parse_line(char *line, t_env **env)
{
	t_cmd	*cmd;
	t_cmd	*firstcmd;
	t_token	**nexttoken;

	cmd = cmd_init(0, 1);
	firstcmd = cmd;
	nexttoken = &(cmd->tokens);
	while (*line)
	{
		while (ft_isspace(*line))
			*(line++) = 0;
		if (*line)
			nexttoken = ft_gettoken(nexttoken, &line);
	}
	if (checktokenlist(cmd->tokens, -1))
		return (freecmd(cmd, 0));
	exectoken(cmd, cmd->tokens, env);
	return (0 * freecmd(cmd, 1));
}
