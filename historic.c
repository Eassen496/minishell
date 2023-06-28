/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   historic.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 19:01:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/06/12 19:05:35 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int load_historic()
{
	int	 	fd;
	char	*line;

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

void	write_historic(char *line, int fd)
{
	int l;

	l = 0;
	while (line[l])
		l++;
	write(fd, line, l);
	write(fd, "\n", 1);
}
