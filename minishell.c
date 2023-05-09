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

int main(void)
{
    char    *line;
    /*struct sigaction    plop;

    plop.sa_flags = ;
    plop.sa_mask = ;
    plop.__sigaction_u = ;
    sigaction(SIGINT, &plop, NULL);*/
    line = 0;
    while (1)
    {
        line = readline("plop>");
        if (line && *line)
        {
            add_history(line);
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
