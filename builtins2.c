/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:37:01 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/17 02:40:38 by abitonti         ###   ########.fr       */
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
