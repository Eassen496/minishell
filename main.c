/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:36:11 by ale-roux          #+#    #+#             */
/*   Updated: 2023/05/09 23:53:44 by ale-roux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <fcntl.h>
#include <stdio.h>
#include "minishell.h"
#include <fcntl.h>
#include <termios.h>

char	*verif_quote(char *str);
void	rc_file(void);
char	*get_next_line(int fd);
void    write_historic(char *line, int fd);
int 	load_historic();

void    parse_line(char *line)
{
    int i;
    int j;

    i = 0;
    while (line[i])
    {
        if (line[i] == ' ')
            line[i] = '_';
        else if (line[i] == '\"' || line[i] == '\'')
        {
            j = 1;
            while(line[i + j] && line[i + j] != line[i])
                j++;
            if (line[i + j] == line[i])
            {
                line[i] = '_';
                line[i + j] = '_';
                i += j;
            }
        }
        i++;
    }
    //execute(line);
}

void sig_handler()
{
    char    c;

    c = rl_end + '0';
    write(1, "\n", 1);
    write(1, "minishell-4.2$ ", 15);
    return ;

    //rl_redisplay();
}

int	main(int argc, char **argv, char **envp)
{
	char	*command;
	int		history;
    t_env   *env;
    struct termios term;

	rc_file();
    //env = load_env(envp);
    //print_env(env);
	history = load_historic();
    command = 0;
    tcgetattr(STDIN_FILENO, &term);
    term.c_lflag &= ~(ICANON | ECHO);
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
	while (1)
	{
        signal(SIGQUIT, SIG_IGN);
        signal(SIGINT, sig_handler);
		command = readline("minishell-4.2$ ");
        //command = verif_quote(command);
        if (!command)
        {
            write(1, "exit\n", 5);
            exit (1);
        }
		if (command && *command)
		{
            //command = verif_quote(command);
			add_history(command);
			write_historic(command, history);
			parse_line(command);
			//printf("%s\n", command);
			free(command);
		}
		else if (command)
			free(command);
        else
            return (0);
	}
    term.c_lflag |= ICANON | ECHO;
    tcsetattr(STDIN_FILENO, TCSANOW, &term);
	return (0);
}
