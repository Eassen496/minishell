/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:38:09 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 04:17:16 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strjoin(char *s1, char *s2, int tofree)
{
	char	*res;
	int		l1;
	int		l2;

	l1 = ft_strlen(s1);
	l2 = ft_strlen(s2);
	res = malloc(l1 + l2 + 1);
	res[l1 + l2] = 0;
	while (l2--)
		res[l1 + l2] = s2[l2];
	while (l1--)
		res[l1] = s1[l1];
	if (tofree % 2)
		free(s1);
	if (tofree > 1)
		free(s2);
	return (res);
}

int	ft_error(char *str1, int freestr1, char *str2, int freestr2)
{
	if (str2)
	{
		write(2, "minishell: ", 11);
		write(2, str1, ft_strlen(str1));
		write(2, str2, ft_strlen(str2));
		if (freestr1)
			free(str1);
		if (freestr2)
			free(str2);
	}
	else
	{
		str1 = ft_strjoin("minishell: ", str1, 2 * freestr1);
		perror(str1);
		free(str1);
	}
	return (1);
}

int	ft_free(void *pt)
{
	if (pt)
		free(pt);
	return (1);
}

char	*pipetostr(int fd[2], int i, int toclose)
{
	char	*res;

	if (i < 0)
		return (0);
	res = malloc(i + 1);
	res[i] = 0;
	if (i)
		read(fd[0], res, i);
	if (toclose)
	{
		close(fd[0]);
		close(fd[1]);
	}
	return (res);
}

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}
