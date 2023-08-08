/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:37:01 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/08 05:30:25 by abitonti         ###   ########.fr       */
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
	write(2, "exit\n", 5);
	while (token && token->token)
		token = token->next;
	if (!token)
		exit(g_minishell.return_value);
	else if (ft_atoiexit(token->line) == 300)
	{
		tmp = ft_strjoin("minishell: exit: ", token->line, 0);
		tmp = ft_strjoin(tmp, ": numeric argument required\n", 1);
		write(2, tmp, ft_strlen(tmp));
		free(tmp);
		exit(255);
	}
	else if (token->next && token)
	{
		write(2, "minishell: exit: too many arguments\n", 36);
		g_minishell.return_value = 1;
	}
	else
		exit(ft_atoiexit(token->line));
}
