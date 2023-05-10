/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/05/10 16:14:48 by ale-roux          #+#    #+#             */
/*   Updated: 2023/05/10 16:15:58 by ale-roux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

int	pwd(int print)
{
	char	cwd[1024];

	getcwd(cwd, sizeof(cwd));
	if (print == 0)
		printf("%s\n", cwd);
	return (0);
}
