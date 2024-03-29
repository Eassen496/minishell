/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abitonti <abitonti@student.42mulhouse.f    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/04/13 17:14:15 by abitonti          #+#    #+#             */
/*   Updated: 2023/08/08 06:00:55 by abitonti         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include <stdio.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <stdlib.h>
# include <signal.h>
# include <fcntl.h>
# include <unistd.h>
# include "get_next_line.h"
# include <errno.h>

typedef struct s_env
{
	char			*nameval;
	struct s_env	*next;
}	t_env;

typedef struct s_token
{
	int				token;
	char			*line;
	struct s_token	*next;
}	t_token;

typedef struct s_cmd
{
	t_token			*tokens;
	int				fdin;
	int				fdout;
	int				err;
	struct s_cmd	*next;
	int				forked;
	int				pid;
}	t_cmd;

typedef struct s_minishell
{
	t_cmd	*command;
	t_env	*env;
	char	*line;
	int		historic;
	int		return_value;
}	t_minishell;

t_minishell	g_minishell;

void	print_env(t_env *env, int fd);
int		ft_getenv(t_env *env, char **line, int fd, int *error);
void	free_env(t_env *current);
t_env	*load_env(char **envp);
void	remove_env(t_env **env, t_env *current, char *name);
void	add_env(t_env **env, t_env *current, char *nameval);
int		load_historic(void);
void	write_historic(char *line, int fd);
int		ft_isamong(char c, char *sep);
char	*ft_strcpy(char *str);
void	ft_echo(t_token *token, t_cmd *cmd);
void	ft_export(t_env **env, t_token *token);
int		ft_strlen(char *str);
char	*pipetostr(int fd[2], int i, int toclose);
void	ft_cd(t_env **env, t_token *token, int tofree, char *home);
void	ft_pwd(int fdout);
void	ft_unset(t_env **env, t_token *token);
char	*ft_strjoin(char *s1, char *s2, int tofree);
int		ft_isspace(char c);
int		parse_line(char *line, t_env **env);
int		ft_error(char *str1, int freestr1, char *str2, int freestr2);
int		ft_strcmp(char *s1, char *s2);
t_token	*newtoken(int token, char *line, t_token *current);
int		namecmp(char *nameval, char *name);
int		ft_free(void *pt);
char	*ft_filename(char *line, t_env *env, int *error);
t_token	*token_to_word(t_token *token, char *line, t_env *env);
t_cmd	*cmd_init(int fdin, int fdout);
void	cmdexec(t_cmd *cmd, t_env **env, t_token *token);
t_token	**ft_gettoken(t_token **nexttoken, char **line);
void	ft_printtokenlist(t_cmd *cmd);
int		checktokenlist(t_token *token, int lasttoken);
int		freecmd(t_cmd *cmd, int freeline);
int		ft_isseparator(char c);
char	*ft_itoa(int n);
int		ft_printlastreturnvalue(int fd);
int		ft_isdigit(int c);
int		ft_atoiexit(char *str);
void	ft_exit(t_token *token);
int		ft_exec(t_cmd *cmd, t_env *env, t_token *token, char *path);
int		ft_contain(char *str, char c);
void	ft_resetline(int plop);
t_token	*ft_input(t_cmd *cmd, t_token *token);
int		ft_changefd(int newfd, int oldfd);

#endif
