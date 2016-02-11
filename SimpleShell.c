/******************************************************************************
@file         SimgpleShell.c
@author       Darren Lowe, Jacqueline Catlin, Georgia Greegoriou, Cameron Turner, Ethan Dickson
@date         Thursday, 11/02/2016
@brief
*******************************************************************************/

#include <stdio.h>
<<<<<<< HEAD
#define MAXIN 512
#define INERR -1

=======
#include <stdout.h>
#include <stderr.h>
>>>>>>> origin/master

int read_line()
{
    char line[512];
    printf(">");
<<<<<<< HEAD

    fgets(&line, MAXIN, stdin);
    printf("%s\n", line);
    return 0;
}

int main() {
    while(read_line() == 0);
    return 0;
}
=======
    
    fgets(line, sizeof(line), stdin);
    
    return line;
}
>>>>>>> origin/master
