/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/09 21:20:21 by ale-roux          #+#    #+#             */
/*   Updated: 2023/05/09 22:43:35 by ale-roux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>

void	print_echo_beginning(char *str, int lim)
{
	int	i;

	i = 0;
	while (str[i] == ' ')
		i++;
	while (i < lim)
	{
		printf("%c", str[i]);
		i++;
	}
	return ;
}

void	print_echo_end(char *str, int start, int option)
{
	int	i;

	i = start;
	if (i == 0)
		i = option;
	while (str[i] == ' ')
		i++;
	while (str[i] && str[i] != '>')
	{
		printf("%c", str[i]);
		i++;
		if (str[i] == ' ')
		{
			printf(" ");
			while (str[i] == ' ')
				i++;
		}
	}
	if (option == 0)
		printf("\n");
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

int	loc_option(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] == '-')
		{
			i++;
			while (str[i] == 'n')
			{
				i++;
			}
			if (str[i] == ' ')
			{
				while (str[i] == ' ')
					i++;
				return (i);
			}
			return (0);
		}
		i++;
	}
	return (0);
}

char	*echo(char *str)
{
	int	i;
	int	option;

	i = last_quote(str);
	option = loc_option(str);
	print_echo_beginning(str, i);
	print_echo_end(str, i, option);
	return (str);
}
