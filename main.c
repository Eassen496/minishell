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
char	*verif_quote(char *str);
void	rc_file(void);

char	*get_next_line(int fd);

void    write_historic(char *line, int fd)
{
    int l;

    l = 0;
    while (line[l])
        l++;
    write(fd, line, l);
    write(fd, "\n", 1);
}

int load_historic()
{
    int     fd;
    char    *line;

    fd = open("history", O_CREAT | O_RDWR | O_APPEND, 0777);
    if (fd == -1)
        return (fd);
    line = get_next_line(fd);
    while (line)
    {
        add_history(line);
        free(line);
        line = get_next_line(fd);
    }
    return (fd);
}

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

int	main(void)
{
	char	*command;
	int		history;

	rc_file();
	history = load_historic();
	while (1)
	{
		command = readline("minishell-0.5$ ");
		command = verif_quote(command);
		if (command && *command)
		{
			add_history(command);
			write_historic(command, history);
			parse_line(command);
			printf("%s %d\n", command, rl_end);
			free(command);
		}
		else if (command)
			free(command);
	}
	return (0);
}
