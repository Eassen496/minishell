/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:13:23 by abitonti          #+#    #+#             */
/*   Updated: 2023/04/13 17:13:26 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char	*ft_strcpy(char *str)
{
	int 	i;
	char	*res;

	if (!str)
		return (0);
	i = 0;
	while (str[i])
		i++;
	res = malloc(i++ + 1);
	while (i--)
		res[i] = str[i];
	return (res);
}

int ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s1 == *s2)
	{
		s1++;
		s2++;
	}
	if (*s1 == *s2)
		return (1);
	return (0);
}

int	ft_isamong(char c, char *sep)
{
	if (!c)
		return (1);
	while(*sep)
	{
		if (c == *sep)
			return (1);
		sep++;
	}
	return (0);
}


void	sig_handler()
{
	char	c;

	c = rl_end + '0';
	write(1, "\nploc>", 6);
	write(1, &c, 1);
	return ;
	//readline("");

	//rl_redisplay();
}

int	ft_isspace(char c)
{
	if (c == ' ' || c == '\f' || c == '\t' || c == '\n' || c == '\r' || c == '\v')
		return (1);
	return (0);
}

int	ft_isseparator(char c)
{
	if (ft_isspace(c) || !c || c == '<' || c == '>' || c == '|')
		return (1);
	return (0);
}

void	ft_getquote(t_env *env, char **line, char c, int fd)
{
	int	i;

	i = 1;
		while(*(*line + i) && *(*line + i) != c)
			i++;
		if (*(*line + i) == c && (*line)++)
		{
			while (**line != c)
			{
				if (c == '\"' && **line == '$' && ++(*line))
					ft_getenv(env, line, fd);
				else
					write(fd, (*line)++, 1);
			}
			(*line)++;
		}
		else
			write(fd, (*line)++, 1);
}


char	*ft_getfilename(char **line, t_env *env)
{
	char	*res;
	int		mypipe[2];
	int		j;

	res = 0;
	pipe(mypipe);
	while (ft_isspace(**line))
		(*line)++;
	while (!ft_isseparator(**line))
	{
		if (**line == '\"' || **line == '\'')
			ft_getquote(env, line, **line, mypipe[1]);
		if (**line == '$')
			ft_getenv(env, line, mypipe[1]);
	}
}

t_cmd   *cmd_init(int fdin, int fdout)
{
	t_cmd   *res;

	res = malloc(sizeof(t_cmd));
	res->fdin = fdin;
	res->fdout = fdout;
	res->next = 0;
	res->tokens = 0;
	return (res);
}

void	parse_line(char *line, t_env *env)
{
	t_cmd   *cmd;
	t_cmd	*firstcmd;
	int		mypipe[2];
	int		arg[2];

	cmd = cmd_init(0, 1);
	pipe(arg);
	firstcmd = cmd;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '|' && line++)
		{
			pipe(mypipe);
			cmd->fdout = mypipe[1];
			cmd->next = cmd_init(mypipe[0], 1);
			cmd = cmd->next;
		}
		else if (*line == '>' && *(++line) != '>')
			cmd->fdout = open(ft_getfilename(&line, env), O_WRONLY);
		else if (*line == '>' && line++)
			cmd->fdout = open(ft_getfilename(&line, env), O_WRONLY || O_APPEND);
		else if (*line == '<' && *(++line) != '<')
			cmd->fdin = open(ft_getfilename(&line, env), O_RDONLY);
		else if (*line == '\"' || *line == '\'')
			ft_getquote(env, &line, *line, mypipe[1]);
		else
			add_word(cmd, ft_getword(&line, env));
	}
}

/*void	parse_line(char *line, t_env *env)
{
	t_cmd   *cmd;
	t_cmd	*firstcmd;
	int		mypipe[2];
	int		arg[2];

	cmd = cmd_init(0, 1);
	pipe(arg);
	firstcmd = cmd;
	while (*line)
	{
		while (ft_isspace(*line))
			line++;
		if (*line == '|' && line++)
		{
			pipe(mypipe);
			cmd->fdout = mypipe[1];
			cmd->next = cmd_init(mypipe[0], 1);
			cmd = cmd->next;
		}
		else if (*line == '>' && *(++line) != '>')
			cmd->fdout = open(ft_getfilename(&line, env), O_WRONLY);
		else if (*line == '>' && line++)
			cmd->fdout = open(ft_getfilename(&line, env), O_WRONLY || O_APPEND);
		else if (*line == '<' && *(++line) != '<')
			cmd->fdin = open(ft_getfilename(&line, env), O_RDONLY);
		else if (*line == '\"' || *line == '\'')
			ft_getquote(env, &line, *line, mypipe[1]);
		else
			add_word(cmd, ft_getword(&line, env));
	}
}*/


void	getval(char *nameval, int fd)
{
	while (*nameval != '=')
		nameval++;
	nameval++;
	while(*nameval)
		write(fd, nameval++, 1);
}

int main(int argc, char **argv, char **envp)
{
	char	*line;
	int	 historic;
	t_env   *env;

	(void) argc;
	(void) argv;
	env = load_env(envp);
	add_env(&env, env, "plop=ca marche");
	add_env(&env, env, "plop2=ca marche pas");
	add_env(&env, env, "plop2=");
	print_env(env);
	signal(SIGQUIT, SIG_IGN);
	signal(SIGINT, sig_handler);
	historic = load_historic();
	line = 0;
	while (1)
	{
		line = readline("plop>");
		if (line && *line)
		{
			add_history(line);
			write_historic(line, historic);
			parse_line(line, env);
			printf("%s %d\n", line, rl_end);
			free(line);
		}
		else if (line)
			free(line);
		else
		{
			//printf("NULL\n");
			return (0);
		}
	}
}
