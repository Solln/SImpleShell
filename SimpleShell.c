/******************************************************************************
@file         SimgpleShell.c
@author       Darren Lowe, Jacqueline Catlin, Georgia Greegoriou, Cameron Turner, Ethan Dickson
@date         Thursday, 11/02/2016
@brief
*******************************************************************************/

#include <stdio.h>
#define MAXIN 512
#define INERR -1


int read_line()
{
    char line[512];
    printf(">");

    fgets(&line, MAXIN, stdin);
    printf("%s\n", line);
    return 0;
}

int main() {
    while(read_line() == 0);
    return 0;
}
