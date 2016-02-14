/******************************************************************************
@file         SimgpleShell.c
@author       Darren Lowe, Jacqueline Catlin, Georgia Greegoriou, Cameron Turner, Ethan Dickson
@date         Thursday, 11/02/2016
@brief
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#define MAXIN 512
#define INERR -1


int main() {

    char line [MAXIN];
    char delim[] = " ";
    const char* token;
    while (1) {
        do {
            printf(">");
            fgets(line, MAXIN, stdin);
        }  while (strlen(line) <= 1);
        token = strtok(line, delim);
        while (token != NULL){
            if ((strcmp("exit\n", token) == 0) || (strcmp("<ctrl>-D\n", token) == 0))  {
                    printf("Quitting\n");
                    return 1;
            }
            printf("token = %s\n", token);
            token = strtok(NULL, delim);
        }
    }
    return 0;
}

