/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing2.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/08/05 02:04:26 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/05 05:32:39 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_removequote(char *line)
{
	int	i;
	int	n;
	int	fd[2];

	n = 0;
	pipe(fd);
	while (*line)
	{
		if (*line == '\'' || *line == '\"')
		{
			i = 1;
			while (*(line + i) && *(line + i) != *line)
				i++;
			if (*(line + i))
			{
				n += write(fd[1], ++line, i - 1);
				line += i;
			}
			else
				n += write(fd[1], line++, 1);
		}
		else
			n += write(fd[1], line++, 1);
	}
	return (pipetostr(fd, n, 1));
}

void	ft_resetexit(int plop)
{
	(void) plop;
	write(1, "\n", 1);
	g_minishell.return_value = 1;
	exit(g_minishell.return_value);
}

t_token	*ft_mother(int pid[2], int fd[2], t_token *token, t_cmd *cmd)
{
	(void) cmd;
	token->next->token = -1;
	signal(SIGINT, SIG_IGN);
	waitpid(pid[0], &(pid[1]), 0);
	signal(SIGINT, ft_resetline);
	g_minishell.return_value = WEXITSTATUS(pid[1]);
	cmd->err = WEXITSTATUS(pid[1]);
	close(fd[1]);
	return (token->next->next);
}

t_token	*ft_input(t_cmd *cmd, t_token *token)
{
	char	*stop;
	char	*line;
	int		fd[2];
	int		pid[2];

	pipe(fd);
	cmd->fdin = ft_changefd(fd[0], cmd->fdin);
	pid[0] = fork();
	if (pid[0])
		return (ft_mother(pid, fd, token, cmd));
	rl_catch_signals = 1;
	signal(SIGINT, ft_resetexit);
	stop = ft_removequote(token->next->line);
	g_minishell.return_value = 0;
	while (1)
	{
		line = readline("> ");
		if (!line || (ft_strcmp(line, stop) && ft_free(line) && ft_free(stop)))
			exit(g_minishell.return_value);
		write(fd[1], line, ft_strlen(line));
		write(fd[1], "\n", 1);
		free(line);
	}
}
