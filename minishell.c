/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:13:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/19 06:18:42 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	global[2];

void	sig_handler(int plop)
{
	(void) plop;
	global[0] = 1;
	write(1, "\nminishell>", 11);
	/*rl_point = 0;
	rl_end = 0;
	rl_line_buffer[0] = 0;*/
	/*rl_replace_line("", 0);
	rl_on_new_line();
	rl_redisplay();*/
	return ;
	//readline("");
	//rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char	*line;
	int		historic;
	t_env	*env;

	(void) argc;
	(void) argv;
	env = load_env(envp);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler);
	historic = load_historic();
	global[0]= 0;
	while (1)
	{
		line = readline("minishell>");
		if (line && *line)
		{
			add_history(line);
			write_historic(line, historic);
			parse_line(line, &env);
			free(line);
		}
		else if (line)
			free(line);
		else
			return (0);
	}
}
