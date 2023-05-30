#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include "minishell.h"

char	*get_next_line(int fd);

int load_historic()
{
    int     fd;
    char    *line;

    fd = open(".history", O_CREAT | O_RDWR | O_APPEND, 0777);
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