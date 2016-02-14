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

    //strings for comparison
    char str1[15];
    char str2[15];
    //newline char as enter is typed
    strcpy(str1, "exit\n");
    strcpy(str2, "<ctrl>-D\n");

    int i = 1;
    const char* token;
    while (i == 1) {
        do {
            printf(">");
            fgets(line, MAXIN, stdin);
        }  while (strlen(line) <= 1);
        token = strtok(line, delim);
        while (token != NULL){
            if ((strcmp(str1, token) == 0) || (strcmp(str2, token) == 0))  {
                    printf("Quitting\n");
                    return 1;
            }
            printf("token = %s\n", token);
            token = strtok(NULL, delim);
        }
    }
    return 0;
}

