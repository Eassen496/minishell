/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools3.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/30 03:15:18 by ale-roux          #+#    #+#             */
/*   Updated: 2023/07/31 00:19:51 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	intlen(long int nb)
{
	int	len;

	len = 0;
	if (nb < 0)
	{
		nb = -nb;
		len++;
	}
	if (nb == 0)
		return (1);
	while (nb > 0)
	{
		len++;
		nb = nb / 10;
	}
	return (len);
}

char	*ft_itoa(int n)
{
	int				i;
	int				y;
	char			*nbr;
	unsigned int	saveme;

	i = intlen(n);
	y = 0;
	saveme = n;
	nbr = malloc((i + 1) * sizeof(char));
	if (!nbr)
		return (NULL);
	nbr[i] = '\0';
	if (n < 0)
	{
		nbr[0] = '-';
		saveme = -n;
		y++;
	}
	while (i > y)
	{
		nbr[--i] = (saveme % 10) + 48;
		saveme = saveme / 10;
	}
	return (nbr);
}

int	ft_atoiexit(char *str)
{
	int	i;
	int	ret;
	int	sign;

	i = 0;
	ret = 0;
	sign = 1;
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i] == '-' && ++i)
		sign *= -1;
	if (str[i] < '0' || str[i] > '9')
		return (300);
	while (str[i] >= '0' && str[i] <= '9')
		ret = (ret * 10) + (str[i++] - 48);
	while (str[i] == ' ' || str[i] == '\t')
		i++;
	if (str[i])
		return (300);
	ret *= sign;
	if (ret < 0 && ret % 256)
		return ((ret % 256) + 256);
	return (ret % 256);
}

int	ft_contain(char *str, char c)
{
	while (*str)
	{
		if (*str == c)
			return (1);
		str++;
	}
	return (0);
}