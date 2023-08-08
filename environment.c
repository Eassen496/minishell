/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/12 19:01:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/05 01:48:00 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	getval(char *nameval, int fd, int *n, int *error)
{	
	int	i;

	i = 0;
	while (*nameval != '=' && ++i)
		nameval++;
	nameval++;
	while (*nameval)
	{
		if (error && ft_isspace(*nameval))
			*error = 1;
		*n += write(fd, nameval++, 1);
	}
	return (i);
}

int	ft_printlastreturnvalue(int fd)
{
	char	*tmp;
	int		ret;

	tmp = ft_itoa(g_minishell.return_value);
	ret = write(fd, tmp, ft_strlen(tmp));
	free(tmp);
	return (ret);
}

int	ft_getenv(t_env *env, char **line, int fd, int *error)
{
	int	i;

	i = 0;
	if (ft_isamong(**line, "\t\n\f\r\v <>|/!$"))
		return (write(fd, "$", 1));
	else if (ft_isamong(**line, "\'\""))
		return (0);
	else if (**line == '?' && ++(*line))
		return (ft_printlastreturnvalue(fd));
	while (env)
	{
		if (namecmp(env->nameval, *line) == 1)
		{
			(*line) += getval(env->nameval, fd, &i, error);
			return (i);
		}
		env = env->next;
	}
	if (error)
	{
		while (**line && !ft_isamong(**line, "\t\n\f\r\v <>|/!\'\"$"))
			(*line)++;
		return (0);
	}
	return (-1);
}

void	remove_env(t_env **env, t_env *current, char *name)
{
	t_env	*next;

	if (!current)
		return ;
	if (namecmp(current->nameval, name) == 1)
	{
		*env = current->next;
		free(current->nameval);
		free(current);
		return ;
	}
	next = current->next;
	while (next)
	{
		if (namecmp(next->nameval, name) == 1)
		{
			current->next = next->next;
			free(next->nameval);
			free(next);
			return ;
		}
		next = next->next;
		current = current->next;
	}
}

void	add_env(t_env **env, t_env *current, char *nameval)
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
