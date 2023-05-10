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

void    cd(char *str)
{
    if (!str)
        chdir(getenv("HOME"));
    else
        chdir(str);
}
