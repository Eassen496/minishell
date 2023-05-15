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

char    *ft_strcpy(char *str)
{
    int i;
    char    *res;

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

int load_historic()
{
    int     fd;
    char    *line;

    fd = open("historic", O_CREAT | O_RDWR | O_APPEND, 0777);
    if (fd == -1)
        return (fd);
    line = get_next_line(fd);
    while (line)
    {
        add_history(line);
        free(line);
        line = get_next_line(fd);
    }
    return (fd);
}

void    write_historic(char *line, int fd)
{
    int l;

    l = 0;
    while (line[l])
        l++;
    write(fd, line, l);
    write(fd, "\n", 1);
}

void sig_handler()
{
    char    c;

    c = rl_end + '0';
    write(1, "\nploc>", 6);
    write(1, &c, 1);
    return ;
    //readline("");

    //rl_redisplay();
}

void    parse_line(char *line)
{
    int i;
    int j;

    i = 0;
    while (line[i])
    {
        if (line[i] == ' ')
            line[i] = '_';
        else if (line[i] == '\"' || line[i] == '\'')
        {
            j = 1;
            while(line[i + j] && line[i + j] != line[i])
                j++;
            if (line[i + j] == line[i])
            {
                line[i] = '_';
                line[i + j] = '_';
                i += j;
            }
        }
        i++;
    }
    //execute(line);
}

void    print_env(t_env *env)
{
    while (env)
    {
        printf("%s=%s\n", env->name, env->val);
        env = env->next;
    }
}

void    free_env(t_env *current)
{
    free(current->val);
    free(current->name);
    free(current);
}

t_env    *load_env(char **envp)
{
    t_env   *first;
    t_env   *current;

    if (*envp)
    {
        first = malloc(sizeof(t_env));
        current = first;
        current->name = ft_strcpy(*envp);
        current->val = ft_strcpy(getenv(current->name));
        current->next = 0;
    }
    else
        return (0);
    while(*(++envp))
    {
        current->next = malloc(sizeof(t_env));
        current = current->next;
        current->name = ft_strcpy(*envp);
        current->val = ft_strcpy(getenv(current->name));
        current->next = 0;
    }
    return (first);
   
}

void    remove_env(t_env  **env, t_env *current, char *name)
{
    t_env   *next;

    if (!current)
        return ;
    if (ft_strcmp(current->name, name))
    {
        *env = current->next;
        free_env(current);
        return ;
    }
    next = current->next;
    while (next)
    {
        if (ft_strcmp(next->name, name))
        {
            current->next = next->next;
            free_env(next);
            return ;
        }
        next = next->next;
        current = current->next;
    }
}

void    add_env(t_env  **env, t_env *current, char *name, char *value)
{
    while (current)
    {
        if (ft_strcmp(current->name, name))
        {
            current->val = ft_strcpy(value);
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
    current->name = ft_strcpy(name);
    current->val = ft_strcpy(value);
}

int main(int argc, char **argv, char **envp)
{
    char    *line;
    int     historic;
    t_env   *env;

    (void) argc;
    (void) argv;
    env = load_env(envp);
    printf("ok\n");
    add_env(&env, env, "plop", "ca marche");
    printf("ok\n");
    add_env(&env, env, "plop2", "ca marche pas");
    printf("ok\n");
    remove_env(&env, env, "plop2");
    printf("ok\n");
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
            parse_line(line);
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
