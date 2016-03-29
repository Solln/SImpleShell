/******************************************************************************
@file         SimgpleShell.c
@author       Darren Lowe, Jacqueline Catlin, Georgia Greegoriou, Cameron Turner, Ethan Dickson
@date         Thursday, 11/02/2016
@brief
*******************************************************************************/

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

#define MAXIN 512
#define MAXPARA 50
#define MAXPATH 120
#define HISTORY_COUNT 20
#define ALLIAS_COUNT 10

#define NoPError "function takes no parameters"
#define UnExPError "unexpected parameters"
#define PathError "path expected"
#define InValError "invalid input"
#define RangeError "command out of range"

typedef struct {
    int number;
    char command[MAXIN];
} history_t;

typedef struct {
    char name[MAXIN];
    char command[MAXIN];
} allias_t;

void invokeHistory(history_t * hist[], char* line, int current) {
    char *input = strdup(line);
    int number;
    if (strcspn(input, "!") == 0) {
        if (sscanf(++input, "%d", &number) == 1) {
            if (number < 0) {
                if (number > (0-HISTORY_COUNT) && number >= (0-current)) {
                    strncpy(line, hist[(current+number)%HISTORY_COUNT]->command, MAXIN);
                }
                else printf("error: %s\n", RangeError);
            }
            else if (number > (current-HISTORY_COUNT) && number <= current) {
                strncpy(line, hist[(number-1)%HISTORY_COUNT]->command, MAXIN);
            }
            else printf("error: %s\n", RangeError);
        }
        else printf("error: %s\n", InValError);
    }
}

void getLine(char *line) {
    if (fgets(line, MAXIN, stdin) != NULL)
        (line[strlen(line)-1] == '\n')? line[strlen(line)-1] = '\0' : 0;
    else {
        strcpy(line, "exit");
    }
}

void splitLine(char *line, char* strings[]) {
    char delim[] = " |><,&;\t";
    char* token;
    token = strtok(line, delim);
    int i = 0;
    while (token != NULL) {
        strings[i++] = token;
        token = strtok(NULL, delim);
    }
}

int execute(char* strings[]) {
    pid_t pid = fork();
    if (pid < 0) {
        perror(strings[0]);
        return 1;
    }
    else if (pid == 0) {
        execvp(strings[0], strings);
        perror(strings[0]);
        exit(0);
    }
    else {
        wait(NULL);
        return 1;
    }
}

char* getPath(char* strings[]) {
    char* path;
    if (strings[1] != NULL) {
	printf("getpath error: %s\n", NoPError);
	return "";
    }
    printf("%s\n",(path = getenv("PATH")));
    return path;
}

int setPath(char* strings[]) {
    if (strings[2] != NULL) {
	printf("setpath error: %s\n", UnExPError);
    }
    else if (strings[1] == NULL) {
	printf("setpath error: %s\n", PathError);
    }
    else {
	char* newPath = strings[1];
    	if (setenv("PATH", newPath, 1) == 0) return 1;
    	else perror("setpath error");
    }
    return 0;
}

void changeDirectory(char* strings[]) {
   if (strings[2] != NULL) {
        printf("cd error: %s\n", UnExPError);
   }
   else if (strings[1] == NULL) {
        chdir(getenv("HOME"));
   } else if (chdir(strings[1]) != 0) perror("cd");
}

void printHistory(history_t *hist[], char* strings[]) {
    if (strings[1] != NULL) printf("history error: %s\n", UnExPError);
    else {
        int current = 0;
        int i = 0;
        while (hist[i] != NULL && i < HISTORY_COUNT) {
            if (current < (hist[i]->number)) {
                current = hist[i]->number;
            }
            i++;
        }
        i = (current % HISTORY_COUNT);
        do {
            if (hist[i] != NULL) {
                printf("%d: %s\n", hist[i]->number, hist[i]->command);
            }
            i = ((i + 1) % HISTORY_COUNT);
        } while (i != (current % HISTORY_COUNT));
    }
}

void addAllias(allias_t* allias[], char* strings[]) {
    if (strings[3] != NULL) printf("allias error: %s", UnExPError);
    for (int i = 0; i < ALLIAS_COUNT; i++) {
        if (allias[i] != NULL) {
            if (strcmp(strings[1], allias[i]->name) == 0) {
                printf("overwriting allias %s\n", strings[1]);
                strncpy(allias[i]->command, strings[2], MAXIN);
                return;
            }
            else if (i == 9) printf("Alliases full!\n");
        }
        else {
            strncpy(allias[i]->name, strings[1], MAXIN);
            strncpy(allias[i]->command, strings[2], MAXIN);
        }
    }
}

void unAllias(allias_t* allias[], char* strings[]) {
    if (strings[2] != NULL) printf("unallias error: %s\n", UnExPError);
    else for (int i = 0; i < ALLIAS_COUNT; i++) {
        if (allias[i] != NULL) {
            if (strcmp(strings[1], allias[i]->command) == 0) {
                allias[i] == NULL;
            }
        }
    }
}

void processTokens(allias_t* allias[], history_t* hist[], char* strings[], bool* exitShell) {
    if ((strcmp("exit", strings[0]) == 0))  {
            if (strings[1] != NULL) printf("exit error: %s\n", NoPError);
            else *exitShell = true;
    }
    else if (strcmp("getpath", strings[0]) == 0) {
        getPath(strings);
    }
    else if (strcmp("setpath", strings[0]) == 0) {
        setPath(strings);
    }
    else if (strcmp("cd", strings[0]) == 0){
        changeDirectory(strings);
    }
    else if (strcmp("history", strings[0]) == 0)
        printHistory(hist, strings);
    else if (strcmp("allias", strings[0]) == 0) {
        allias(allias, strings);
    }
    else if (strcmpe("unallias", strings[0]) == 0) {
        unAllias(allias, strings);
    }
    else execute(strings);
}

void addHistory(history_t* hist[], int* current, char* line) {
    history_t *newCommand = malloc(sizeof(history_t));
    newCommand->number = *current + 1;
    strncpy(newCommand->command, line, MAXIN);
    hist[*current % HISTORY_COUNT] = newCommand;
    *current = *current + 1;
}

void checkAllias(allias_t* allias[], char* strings[]) {
    for (int i = 0; i < ALLIAS_COUNT; i++) {
        if (allias[i] != NULL) {
            for (int j = 0; j < MAXPARA) {
                if (strings[j] != NULL) {
                    if (strcmp(allias[i]->name, strings[j]) == 0) {
                        strncpy(strings[j], allias[i]->command, MAXIN);
                    }
                }
            }
        }
    }
}

void runShell(history_t* hist[], int* current) {
    char *currentDir = malloc(sizeof(char)*MAXPATH);
    char *line = malloc(sizeof(char)*MAXIN);
    char *strings[MAXPARA];
    memset(strings, 0, sizeof(strings));
    allias_t *allias[ALLIAS_COUNT];
    memset(allias, 0, sizeof(allias));
    bool exitShell = false;
    while(!exitShell) {
        printf("%s>", getcwd( currentDir, MAXPATH));
        getLine(line);
        if (strcmp("", line) == 0) continue;
        invokeHistory(hist, line, *current);
        addHistory(hist, current, line);
        splitLine(line, strings);
        checkAllias(allias, strings);
        processTokens(hist, strings, &exitShell);
        memset(strings, 0, sizeof(strings));
    }
    free(line);
}

int loadHistory(history_t* hist[], int* current){
    if( access( "hist_list.txt", F_OK ) != -1 ) {
        char* line = malloc(sizeof(int)+(sizeof(char)*MAXIN));
        FILE *fp = fopen("hist_list.txt", "r");
        if (fp == 0) {
            fprintf(stderr, "failed to open hist_list.txt\n");
            perror("load");
            return 0;
        }
        while (fgets(line, MAXIN, fp)) {
            char* strings[2];
            char* tempChar = malloc(sizeof(char)*MAXIN);
            splitLine(line, strings);
            sscanf(strings[0], "%d", current);
            sscanf(strings[1], "%s", tempChar);
            addHistory(hist, current, tempChar);
            free(tempChar);
        }
        fclose(fp);
        free(line);
        return 1;
    } else {
        fopen("hist_list.txt", "w");
        return 0;
    }
}

void writeHistory(history_t* hist[]){
    FILE *fp;
    fp=fopen("hist_list.txt","w");
    for(int n=0;n<HISTORY_COUNT;n++) {
        if (hist[n] != NULL) fprintf(fp,"%d: %s\n",hist[n]->number, strcat(hist[n]->command, " "));
    }
    fclose(fp);
}

int main() {
    char* path = getenv("PATH");
    chdir(getenv("HOME"));
    history_t *hist[HISTORY_COUNT];
    memset(hist, 0, sizeof(hist));
    int current = 0;
    loadHistory(hist, &current);
    runShell(hist, &current);
    setenv("PATH", path, 1);
    writeHistory(hist);
}
