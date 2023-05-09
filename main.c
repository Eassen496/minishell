/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 16:36:11 by ale-roux          #+#    #+#             */
/*   Updated: 2023/05/09 19:42:59 by ale-roux         ###   ########.fr       */
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

int	count_space(char	*str);
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	while (str[i])
	{
		if (str[i] == ' ')
			j++;
		i++;
	}
	return (j);
}

void	command_logic(char *str)
{
	int	i;
	int	j;
	int	spacecount;

	i = 0;
	j = 0;
	spacecount = count_space(str);
	while (j < spacecount)
	{
		i = ft_strchr(str, ' ');
		j++;
	}
	return ;
}

void	prompt_format()
{
	printf("\033[1;32m");
	printf("minishell@42mulhouse");
	printf("\033[0;32m");
	printf(":");
	printf("\033[0;34m");
	printf("~");
	printf("\033[0;32m");
}

int	main(void)
{
	char	*command;

	rc_file();
	while (1)
	{
		prompt_format();
		command = readline("$ ");
		if(command_logic(command) == 1);
		{
			free(command);
			return (0);
		}
		free(command);
	}
	return (0);
}
