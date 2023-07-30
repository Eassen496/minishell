/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <ale-roux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:37:01 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 06:16:14 by ale-roux         ###   ########.fr       */
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

void	ft_exit(t_token *token)
{
	int		i;
	char	*tmp;

	i = 0;
	write(1, "exit\n", 5);
	if (!token)
		exit(g_minishell.return_value);
	while (token && token->next && token->token == 3)
		token = token->next;
	if (ft_atoiexit(token->line) == 300)
	{
		tmp = ft_strjoin("minishell: exit: ", token->line, 0);
		tmp = ft_strjoin(tmp, ": numeric argument required", 1);
		write(2, tmp, ft_strlen(tmp) - 1);
		write(2, "\n", 1);
		free(tmp);
		exit(255);
	}
	if (token->next && token)
	{
		write(2, "minishell: exit: too many arguments", 35);
		write(2, "\n", 1);
		g_minishell.return_value = 1;
	}
	else
		exit (ft_atoiexit(token->line));
}
