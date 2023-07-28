/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:31:50 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/17 00:40:06 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env, int fd)
{
	while (env)
	{
		write(fd, env->nameval, ft_strlen(env->nameval));
		write(fd, "\n", 1);
		env = env->next;
	}
}

t_env	*load_env(char **envp)
{
	t_env	*first;
	t_env	*current;

	if (*envp)
	{
		first = malloc(sizeof(t_env));
		current = first;
		current->nameval = ft_strcpy(*envp);
		current->next = 0;
	}
	else
		return (0);
	while (*(++envp))
	{
		current->next = malloc(sizeof(t_env));
		current = current->next;
		current->nameval = ft_strcpy(*envp);
		current->next = 0;
	}
	return (first);
}
