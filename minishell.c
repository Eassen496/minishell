/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:13:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/08 06:05:20 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	ft_resetline(int plop)
{
	(void) plop;
	printf("\n");
	rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();
	return ;
}

void	ft_init(int argc, char **argv, char **envp)
{
	(void) argc;
	(void) argv;
	rl_catch_signals = 0;
	g_minishell.env = load_env(envp);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, ft_resetline);
	g_minishell.historic = load_historic();
	g_minishell.return_value = 0;
}

int	main(int argc, char **argv, char **envp)
{
	ft_init(argc, argv, envp);
	while (1)
	{
		g_minishell.line = readline("minishell> ");
		if (g_minishell.line && *g_minishell.line)
		{
			signal(SIGINT, SIG_IGN);
			add_history(g_minishell.line);
			write_historic(g_minishell.line, g_minishell.historic);
			parse_line(g_minishell.line, &g_minishell.env);
			free(g_minishell.line);
			signal(SIGINT, ft_resetline);
		}
		else if (g_minishell.line)
			free(g_minishell.line);
		else
		{
			free_env(g_minishell.env);
			close(g_minishell.historic);
			write(2, "exit\n", 5);
			exit (g_minishell.return_value);
		}
	}
}
