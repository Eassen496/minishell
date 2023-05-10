/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:12:48 by ale-roux          #+#    #+#             */
/*   Updated: 2023/05/10 16:14:28 by ale-roux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	cd(char *str)
{
	int	code;

	if (!str)
		code = chdir(getenv("HOME"));
	else
		code = chdir(str);
	if (code == -1)
		printf("minishell: cd: %s: No such file or directory\n", str);
	return (0);
}
