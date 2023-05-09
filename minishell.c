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

    fd = open("historic", O_CREAT | O_RDWR | O_APPEND);
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

int main(void)
{
    char    *line;
    int     historic;
    /*struct sigaction    plop;

    plop.sa_flags = ;
    plop.sa_mask = ;
    plop.__sigaction_u = ;
    sigaction(SIGINT, &plop, NULL);*/
    historic = load_historic();
    line = 0;
    while (1)
    {
        line = readline("plop>");
        if (line && *line)
        {
            add_history(line);
            write_historic(line, historic);
            printf("%s\n", line);
            free(line);
        }
        else if (line)
            free(line);
        else
        {
            printf("NULL\n");
            break ;
        }
    }
}
