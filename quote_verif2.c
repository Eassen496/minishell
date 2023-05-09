/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   quote_verif2.c                                     :+:      :+:    :+:   */
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

int	quote_error(char *str)
{
	int	i;
	int	quote;
	int	dquote;
	int	order;

	i = -1;
	quote = 0;
	dquote = 0;
	order = 0;
	while (str[++i])
	{
		if (str[i] == '\'' && order != 1)
		{
			quote++;
			order--;
		}
		else if (str[i] == '\"' && order != -1)
		{
			dquote++;
			order++;
		}
		if (order == 2 || order == -2)
			order = 0;
	}
	return (order);
}

int	last_quote(char *str)
{
	int	quote;
	int	dquote;

	quote = ft_strrchr(str, '\'');
	dquote = ft_strrchr(str, '\"');
	if (quote > dquote)
		return (quote);
	else if (quote < dquote)
		return (dquote);
	else
		return (0);
}

char	*verif_quote(char *str)
{
	int	i;
	int	error;
	int	j;

	error = quote_error(str);
	while (error != 0)
	{
		if (error == -1)
			str = ft_strjoin(str, readline(">"));
		else
			str = ft_strjoin(str, readline(">"));
		error = quote_error(str);
	}
	return (str);
}
