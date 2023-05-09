/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/11/10 00:28:09 by abitonti          #+#    #+#             */
/*   Updated: 2022/11/10 18:28:35 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include "get_next_line.h"

static int	freeall(t_line **line)
{
	t_line	*next;

	while (*line)
	{
		free((*line)->buff);
		next = (*line)->next;
		free(*line);
		*line = next;
	}
	return (1);
}

static int	new_tline(t_line **line)
{
	*line = malloc(sizeof(t_line));
	if (!(*line))
		return (0);
	(*line)->buff = malloc(100);
	if (!((*line)->buff))
	{
		free(*line);
		*line = 0;
		return (0);
	}
	(*line)->next = 0;
	(*line)->i = 0;
	(*line)->start = 0;
	(*line)->end = 0;
	return (1);
}

static int	get_next_buff(int fd, int n, t_line *line)
{
	if (!(line->i))
		line->end = read(fd, line->buff, 100);
	if (line->end == -1 || (line->end == 0 && n == 0))
		return (0);
	while (line->i < line->end && (line->buff)[line->i] != '\n')
		(line->i)++;
	if (line->i == 100)
	{
		if (!new_tline(&(line->next)))
			return (0);
		return (get_next_buff(fd, n + line->i - line->start, line->next));
	}
	if ((line->buff)[line->i] == '\n')
		(line->i)++;
	return (n + line->i - line->start);
}

static void	fillsol(char *sol, int n, t_line **line)
{
	int		j;
	t_line	*next;

	while (*line)
	{
		j = (*line)->start - 1;
		while (++j < (*line)->i)
			sol[n++] = ((*line)->buff)[j];
		if (j == (*line)->end)
		{
			free((*line)->buff);
			next = (*line)->next;
			free(*line);
			*line = next;
		}
		else
		{
			(*line)->start = j;
			break ;
		}
	}
	sol[n] = 0;
	return ;
}

char	*get_next_line(int fd)
{
	static t_line	*line[256];
	int				size;
	char			*sol;

	if (fd < 0 || fd == 1 || fd == 2 || fd > 255)
		return (0);
	if (!line[fd] && !new_tline(&(line[fd])))
		return (0);
	size = get_next_buff(fd, 0, line[fd]);
	if (!size && freeall(&(line[fd])))
		return (0);
	sol = malloc(size + 1);
	if (!sol && freeall(&(line[fd])))
		return (0);
	fillsol(sol, 0, &(line[fd]));
	return (sol);
}
