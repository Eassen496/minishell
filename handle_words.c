/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_words.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ale-roux <ale-roux@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 00:44:31 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 03:25:34 by ale-roux         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	ft_getquote(t_env *env, char **line, char c, int fd)
{
	int	i;
	int	n;
	int	osef;

	i = 1;
	n = 0;
	while (*(*line + i) && *(*line + i) != c)
		i++;
	if (*(*line + i) == c)
	{
		(*line)++;
		while (**line != c)
		{
			if (c == '\"' && **line == '$' && ++(*line))
				n += ft_getenv(env, line, fd, &osef);
			else
				n += write(fd, (*line)++, 1);
		}
		(*line)++;
	}
	else
		n += write(fd, (*line)++, 1);
	return (n);
}

t_token	*getvalsplit(char **line, char *nameval, int pipe[3], t_token *token)
{
	while (*nameval != '=' && ++(*line))
		nameval++;
	nameval++;
	while (*nameval)
	{
		if (ft_isspace(*nameval))
		{
			while (ft_isspace(*nameval))
				nameval++;
			if (pipe[2])
			{
				if (token->line)
				{
					token->next = newtoken(0, 0, token->next);
					token = token->next;
				}
				token->line = pipetostr(pipe, pipe[2], 0);
				pipe[2] = 0;
			}
		}
		else
			pipe[2] += write(pipe[1], nameval++, 1);
	}
	return (token);
}

t_token	*ft_getenvsplit(t_env *env, char **line, int pipe[3], t_token *token)
{
	if (ft_isamong(**line, "\t\n\f\r\v <>|/!") && ++pipe[2])
	{
		write(pipe[1], "$", 1);
		return (token);
	}
	else if (ft_isamong(**line, "\'\""))
		return (token);
	else if (**line == '?' && ++(*line))
	{
		pipe[2] += ft_printlastreturnvalue(pipe[1]);
		return (token);
	}
	while (env)
	{
		if (namecmp(env->nameval, *line) == 1)
			return (getvalsplit(line, env->nameval, pipe, token));
		env = env->next;
	}
	return (token);
}

t_token	*token_to_word(t_token *token, char *line, t_env *env)
{
	int	mypipe[3];

	pipe(mypipe);
	mypipe[2] = 0;
	token->line = 0;
	while (*line)
	{
		if (*line == '\"' || *line == '\'')
			mypipe[2] += ft_getquote(env, &line, *line, mypipe[1]);
		else if (*line == '$' && ++line)
			token = ft_getenvsplit(env, &line, mypipe, token);
		else
			mypipe[2] += write(mypipe[1], line++, 1);
	}
	if (token->line == 0)
		token->line = pipetostr(mypipe, mypipe[2], 1);
	else if (mypipe[2])
	{
		token->next = newtoken(0, 0, token->next);
		token = token->next;
		token->line = pipetostr(mypipe, mypipe[2], 1);
	}
	return (token->next);
}

char	*ft_filename(char *line, t_env *env, int *error)
{
	char	*res;
	int		i;
	int		mypipe[2];

	i = 0;
	pipe(mypipe);
	while (*line)
	{
		if (*line == '\"' || *line == '\'')
			i += ft_getquote(env, &line, *line, mypipe[1]);
		else if (*line == '$' && ++line)
			i += ft_getenv(env, &line, mypipe[1], error);
		else
			i += write(mypipe[1], line++, 1);
	}
	res = pipetostr(mypipe, i, 1);
	return (res);
}
