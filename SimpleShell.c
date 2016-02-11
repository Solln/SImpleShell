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
    
    char line [512];
    char delim[] = " ";
    char* token;
    
    do{
    
    printf(">");
    
    fgets(line, MAXIN, stdin);
        
    
    } while (strlen(line) <= 1);
    
    
    for (token = strtok(line, delim); token; token = strtok(NULL, delim))
    {
        printf("token=%s\n", token);
    }
    
    
    
    
}
