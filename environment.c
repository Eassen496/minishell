/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 19:01:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/06/12 19:05:35 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_env(t_env *env)
{
	while (env)
	{
		printf("%s\n", env->nameval);
		env = env->next;
	}
}

int namecmp(char *nameval, char **name)
{
	while (**name && *nameval == **name && *nameval != '=')
	{
		(*name)++;
		nameval++;
	}
	if (*nameval == '=' && ft_isamong(**name, "\t\n\f\r\v <>&|\'\"?!$"))
		return (1);
	if (*nameval == '=' && **name == '=')
		return (2);
	return (0);
}

void	ft_getenv(t_env *env, char **line, int fd)
{
	int	i;

	i = 0;
	if (ft_isamong(**line, "\t\n\f\r\v <>&|"))
	{
		write(fd, "$", 1);
		return ;
	}
	else if (ft_isamong(**line, "\'\"!"))
		return ;
	else if (**line == '?' && ++(*line))
	{
		ft_printlastreturnvalue(fd);
		return ;
	}
	while (env)
	{
		if (namecmp(env->nameval, line) == 1)
		{
			getval(env->nameval, fd);
			return ;
		}
		env = env->next;
	}
	return ;
}

void	free_env(t_env *current)
{
	free(current->nameval);
	free(current);
}


t_env	*load_env(char **envp)
{
	t_env   *first;
	t_env   *current;

	if (*envp)
	{
		first = malloc(sizeof(t_env));
		current = first;
		current->nameval = ft_strcpy(*envp);
		current->next = 0;
	}
	else
		return (0);
	while(*(++envp))
	{
		current->next = malloc(sizeof(t_env));
		current = current->next;
		current->nameval = ft_strcpy(*envp);
		current->next = 0;
	}
	return (first);
   
}

void	remove_env(t_env  **env, t_env *current, char *name)
{
	t_env   *next;

	if (!current)
		return ;
	if (namecmp(current->nameval, name) == 1)
	{
		*env = current->next;
		free_env(current);
		return ;
	}
	next = current->next;
	while (next)
	{
		if (namecmp(next->nameval, name) == 1)
		{
			current->next = next->next;
			free_env(next);
			return ;
		}
		next = next->next;
		current = current->next;
	}
}

void	add_env(t_env  **env, t_env *current, char *nameval)
{
	while (current)
	{
		if (namecmp(current->nameval, nameval) == 2)
		{
			free(current->nameval);
			current->nameval = ft_strcpy(nameval);
			return ;
		}
		if (!current->next)
			break ;
		current = current->next;
	}
	if (current)
	{
		current->next = malloc(sizeof(t_env));
		current = current->next;
	}
	else
	{
		*env = malloc(sizeof(t_env));
		current = *env;
	}
	current->next = 0;
	current->nameval = ft_strcpy(nameval);
}
