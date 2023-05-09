/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishellrc.c                                      :+:      :+:    :+:   */
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

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	rc_handler(char backslash, char color)
{
	if (backslash == '\\' && (color == 'r' || color == 'R'))
		printf("\033[0;31m");
	else if (backslash == '\\' && (color == 'g' || color == 'G'))
		printf("\033[0;32m");
	else if (backslash == '\\' && (color == 'y' || color == 'Y'))
        printf("\033[0;33m");
	else if (backslash == '\\' && (color == 'b' || color == 'B'))
        printf("\033[0;34m");
	else if (backslash == '\\' && (color == 'p' || color == 'P'))
        printf("\033[0;35m");
	else if (backslash == '\\' && (color == 'c' || color == 'C'))
        printf("\033[0;36m");
	else if (backslash == '\\' && (color == 'w' || color == 'W'))
        printf("\033[0;37m");
	else
		return (1);
	return (0);
}

void	rc_file()
{
	int		i;
	int		len;
	int		fd;
	char	*rc;

	fd = open("./.minishellrc", O_RDONLY);
	if (fd != -1)
	{
		rc = get_next_line(fd);
		while(rc != NULL)
		{
			i = 0;
			len = ft_strlen(rc);
			while (i <= len)
			{
				if (rc_handler(rc[i], rc[i + 1]) == 0)
					i = i + 2;
				printf("%c", rc[i]);
				i++;
			}
			free(rc);
			printf("\033[0m");
			rc = get_next_line(fd);
		}
	}
}