/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:13:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/04/13 17:13:26 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int load_historic()
{
    int     fd;
    char    *line;

    fd = open("historic", O_CREAT | O_RDWR | O_APPEND, 0777);
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

void    write_historic(char *line, int fd)
{
    int l;

    l = 0;
    while (line[l])
        l++;
    write(fd, line, l);
    write(fd, "\n", 1);
}

void sig_handler()
{
    char    c;

    c = rl_end + '0';
    write(1, "\nploc>", 6);
    write(1, &c, 1);
    return ;
    //readline("");

    //rl_redisplay();
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

int main(void)
{
    char    *line;
    int     historic;

    signal(SIGQUIT, SIG_IGN);
    signal(SIGINT, sig_handler);
    historic = load_historic();
    line = 0;
    while (1)
    {
        line = readline("plop>");
        if (line && *line)
        {
            add_history(line);
            write_historic(line, historic);
            parse_line(line);
            printf("%s %d\n", line, rl_end);
            free(line);
        }
        else if (line)
            free(line);
        else
        {
            //printf("NULL\n");
            return (0);
        }
    }
}
