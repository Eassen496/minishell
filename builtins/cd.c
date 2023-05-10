/*
 * =====================================================================================
 *
 *       Filename:  cd.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  10/05/2023 13:45:27
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  Ale-roux (Eassen496), 
 *   Organization:  
 *
 * =====================================================================================
 */

#include <stdlib.h>
#include<unistd.h>

int    cd(char *str)
{
    int code;

    if (!str)
        code = chdir(getenv("HOME"));
    else
        code = chdir(str);
    if (code == -1)
        printf("bash: cd: %s: No such file or directory\n", str);
    return (0);
}
