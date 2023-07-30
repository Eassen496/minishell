/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   token.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/17 02:01:45 by abitonti          #+#    #+#             */
/*   Updated: 2023/07/30 04:17:12 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	ft_getword(char *line)
{
	int	i;
	int	j;

	i = 0;
	while (line[i])
	{
		if (line[i] == '\"' || line[i] == '\'')
		{
			j = 1;
			while (line[i + j] && line[i + j] != line[i])
				j++;
			if (line[i + j] == line[i])
				i += j + 1;
			else
				i++;
		}
		else if (!line[i] || ft_isseparator(line[i]))
			break ;
		else
			i++;
	}
	return (i);
}

t_token	*newtoken(int token, char *line, t_token *nexttoken)
{
	t_token	*res;

	res = malloc(sizeof(t_token));
	res->token = token;
	res->line = line;
	res->next = nexttoken;
	return (res);
}

t_token	**token_init(int n, t_token **token, char **line, char *str)
{
	*token = malloc(sizeof(t_token));
	(*token)->token = n;
	(*token)->line = str;
	if (n)
	{
		if (!(n % 2))
			*((*line)++) = 0;
		*((*line)++) = 0;
	}
	else
	{
		(*line) += ft_getword(*line);
	}
	(*token)->next = 0;
	return (&((*token)->next));
}

t_token	**ft_gettoken(t_token **nexttoken, char **line)
{
	if (**line == '|') // && *(*line + 1) != '|')
		return (token_init(1, nexttoken, line, "|"));
	//else if (**line == '|' && *(*line + 1) == '|')
	//	return (token_init(2, nexttoken, line, "||"));
	else if (**line == '>' && *(*line + 1) != '>')
		return (token_init(3, nexttoken, line, ">"));
	else if (**line == '>' && *(*line + 1) == '>')
		return (token_init(4, nexttoken, line, ">>"));
	else if (**line == '<' && *(*line + 1) != '<')
		return (token_init(5, nexttoken, line, "<"));
	else if (**line == '<' && *(*line + 1) == '<')
		return (token_init(6, nexttoken, line, "<<"));
	//else if (**line == '&' && *(*line + 1) != '&') // token & erreur ?
	//	return (token_init(7, nexttoken, line, "&"));
	//else if (**line == '&' && *(*line + 1) == '&')
	//	return (token_init(8, nexttoken, line, "&&"));
	else
		return (token_init(0, nexttoken, line, *line));
}

int	checktokenlist(t_token *token, int lasttoken)
{
	while (token)
	{
		if (token->token == 1 || token->token == 2 || token->token == 8)
			if (lasttoken == 1 || lasttoken == -1 || lasttoken == 8)
				break ;
		if (token->token)
			if (lasttoken == 3 || lasttoken == 4 || lasttoken == 5
				|| lasttoken == 6)
				break ;
		lasttoken = token->token;
		token = token->next;
	}
	if (token || lasttoken > 0)
	{
		write(2, "minishell: syntax error near unexpected token \'", 47);
		if (token)
			write(2, token->line, ft_strlen(token->line));
		else
			write(2, "newline", 7);
		write(2, "\'\n", 2);
		return (1);
	}
	if (lasttoken == 1 || lasttoken == 2 || lasttoken == 8)
		return (2);
	return (0);
}
