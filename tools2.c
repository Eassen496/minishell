/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:40:49 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/05 02:08:29 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy(char *str)
{
	int		i;
	char	*res;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	res = malloc(i++ + 1);
	while (i--)
		res[i] = str[i];
	return (res);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (1);
	return (0);
}

int	ft_isamong(char c, char *sep)
{
	if (!c)
		return (1);
	while (*sep)
	{
		if (c == *sep)
			return (1);
		sep++;
	}
	return (0);
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\t'
		|| c == '\n' || c == '\r' || c == '\v')
		return (1);
	return (0);
}

int	ft_isseparator(char c)
{
	if (ft_isspace(c) || !c || c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}
