/*
 * =====================================================================================
 *
 *       Filename:  pwd.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2023 13:45:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ale-roux (Eassen496), 
 *   Organization:  42
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

char    *pwd(int print)
{
    char cwd[1024];
    getcwd(cwd, sizeof(cwd));
    if (print == 0)
        printf("%s\n", cwd);
    return (cwd);
}

int main()
{
    pwd(0);
}
