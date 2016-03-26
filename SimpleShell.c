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

#define NoPError "function takes no parameters"
#define UnExPError "unexpected parameters"
#define PathError "path expected"

const char* currentDir;
char* path;
int current = 0;
char *hist[HISTORY_COUNT];


char* getPath(char** strings) {
    if (strings[1] != NULL) {
	printf("getpath error: %s\n", NoPError);
	return "";
    }
    printf("%s\n", getenv("PATH"));
    return path;
}

int setPath(char** strings) {
    int returnVal = 0;
    if (strings[2] != NULL) {
	printf("setpath error: %s\n", UnExPError);
    }
    else if (strings[1] == NULL) {
	printf("setpath error: %s\n", PathError);
    }
    else {
	char* newPath = strings[1];
    	returnVal = setenv("PATH", newPath, 1);
    }
    return returnVal;
}

int changeDirectory(char** strings) {
   int returnVal = 1;
   if (strings[2] != NULL) {
        printf("cd error: %s\n", UnExPError);
   }
   else if (strings[1] == NULL) {
        chdir(getenv("HOME"));
        returnVal = 0;
   } else if (chdir(strings[1])) perror("cd");
   return returnVal;
}

int execute(char** strings) {
    // Fork process
    pid_t pid = fork();
    // Error
    if (pid < 0) {
        perror(strings[0]);
        return 1;
    }
    // Child process
    else if (pid == 0) {
        // Execute command
        execvp(strings[0], strings);
        // Error occurred
        perror(strings[0]);
        exit(0);
    }
    // Parent process
    else {
        // Wait for child process to finish
        wait(NULL);
        return 1;
    }
}

int printHistory()
{
    int i = ((current)  % HISTORY_COUNT);
    int order = 1;
    do {
        if (strcmp("", hist[i]) != 0) {
            printf("%4d  %s\n", order, hist[i]);
            order++;
        }
        i = ((i + 1) % HISTORY_COUNT);
    } while (i != (current % HISTORY_COUNT) && hist[i] != NULL);
    return 0;
}

int clear_history()
{
    for (int i = 0; i < HISTORY_COUNT; i++)
        hist[i] = malloc(MAXIN);
    return 0;
}

char* getHistory(char* line) {
    int number;
    ++line;
    if (line[0] == '-') {
        if (sscanf(++line, "%d", &number) == 1) {
            if (number > 0 && number < 20 && number < current)
                return hist[(current % HISTORY_COUNT) - (number)];
        }
    }
    else if (sscanf(line, "%d", &number) == 1) {
        if (number > 0 && number < 20 && number < current)
        return hist[number-1];
    }
    printf("Invalid input\n");
    return "";
}

void executeLine(char** strings) {
    if (strcmp("getpath", strings[0]) == 0) {
        getPath(strings);
    }
    else if (strcmp("setpath", strings[0]) == 0) {
        setPath(strings);
    }
    else if (strcmp("history", strings[0]) == 0)
        printHistory();
    else if (strcmp("hc", strings[0]) == 0)
        clear_history();
    else if (strcmp("cd", strings[0]) == 0){
        changeDirectory(strings);
    }
    else execute(strings);
}
void writeHistory(){
    
    FILE *fp;
    
    //char str[80];
    //strcpy(str, getenv("HOME"));
    //strcat(str, "hist_list.txt");
    
    int n;
    
    fp=fopen("hist_list.txt","w");
    
    for(n=0;n<HISTORY_COUNT;n++) {
        
        fprintf(fp,"%s\n",hist[n]);
        
    }
    
    fclose(fp);
    
}

void loadHistory(){
    
    //char str[80];
    //strcpy(str, getenv("HOME"));
    //strcat(str, "hist_list.txt");
    
    
    if( access( "hist_list.txt", F_OK ) != -1 ) {
        // file exists
        int i = 0;
        
        FILE *fp = fopen("hist_list.txt", "r");
        
        if (fp == 0)
        {
            fprintf(stderr, "failed to open hist_list.txt\n");
            exit(1);
        }
        while (i < HISTORY_COUNT && fgets(hist[i], sizeof(hist[0]), fp))
        {
            hist[i][strlen(hist[i])-1] = '\0';
            i = i + 1;
        }
        fclose(fp);
        
    } else {
        // file doesn't exist
        
        FILE *fp = fopen("hist_list.txt", "w");
        
    }   
}

alias() {
	if (1) {
		for(int r=0; r < 10; r++) {
			if (strcmp(strings[0], alias[r][0])) {
				//execute
				//code to execute alias command
				break;
			}
		}
	}
	else if (strcmp("alias", strings[0]) == 0) && sizeof(strings) == 1) {
		if (alias[0][0] == null)
			printf("No Aliases");
		else
			for(int r=0; r < 10; r++) {
				if (alias[r][0] != null) {
					for(int c=0; c < 2; c++)
						printf("%s", alias[r][c]);
					printf("\n");
				}
			}
	}
	else if (strcmp("alias", strings[0]) == 0) && sizeof(strings) == 3) {
		int placed = 0;
		for (int r=0; r < 10; r++) {
			if (strcmp(strings[2], alias[r][1]) == 0) {
				printf("Alias %s Has Been Replaced With %s For Command %s", alias[r][0], strings[1], alias[r][2]);
				alias[r][0] = strings[1];
				placed = 1;
				break;
			}
		}
		for (int r=0; r < 10; r++) {
			if (strcmp(strings[1], alias[r][0]) == 0) {
				printf("Command %s Has Been Replaced With %s For Alias %s", alias[r][1], strings[2], alias[r][1]);
				alias[r][1] = strings[2];
				placed = 1;
				break;
			}
		}
		if (sizeof(alias) == 10) {
			printf("Alias List Is Full");
			placed = 1;
		}
		if (placed == 0) {
			alias[sizeof(alias)][0] = strings[1];
			alias[sizeof(alias)][1] = strings[2];
		}
	}
	else if (strcmp("unalias", strings[0]) == 0) && sizeof(strings) == 2) {
		int removed = 0;
		for(int r=0; r < 10; r++) {
			if (strcmp(strings[1], alias[r][0])) {
				alias[r][0] = null;
				alias[r][1] = null;
				while(!(r == 9)) {
					alias[r][0] = alias[r + 1][0];
					alias[r][1] = alias[r + 1][0];
					r = r + 1;
				}
				removed = 1;
				break;
			}
		}
		if (removed == 0)
			printf("No Such Alias");
	}
	else if (strcmp("alias", strings[0]) == 0) && sizeof(strings) > 3) {
		printf("Invalid Input");
	}
}


int main()
{
    path = getenv("PATH");
    char line[MAXIN];
    char delim[] = " |><,&;\t";
    char* token;
    char* strings[MAXPARA];
	char* alias[10][2];
    memset(strings, 0, sizeof(strings));
    char filePath[MAXPATH];
    chdir(getenv("HOME"));
    int i;
    for (i = 0; i < HISTORY_COUNT; i++)
        hist[i] = malloc(MAXIN);
    loadHistory();
    i = 0;
    current = 0;
    bool exitShell = false;
	//alias() you need to check for an alias first
    while(!exitShell) {
        currentDir = (getcwd( filePath, MAXPATH ) != NULL)? filePath : "ERROR";
        printf("%s>", currentDir);
        if (fgets(line, sizeof(line), stdin) == NULL) break;
        (line[strlen(line)-1] == '\n')? line[strlen(line)-1] = '\0' : 0;
        if (line[0] == '!'){
            strncpy(line, getHistory(line), MAXIN);
        }
        else if (strcmp("", line) == 0) continue;
        strncpy(hist[current % HISTORY_COUNT], strdup(line), MAXIN);
        ++current;
        token = strtok(line, delim);
        i = 0;
        while (token != NULL){
            strings[i++] = token;
            token = strtok(NULL, delim);
        }
        if ((strcmp("exit", strings[0]) == 0))  {
            if (strings[1] != NULL) printf("exit error: %s\n", NoPError);
            else exitShell = true;
        }
        executeLine(strings);
        memset(strings, 0, sizeof(strings));
   }
   memset(strings, 0, sizeof(strings));
   strings[1] = path;
   setPath(strings);
   strings[1] = NULL;
   getPath(strings);
   writeHistory();
   clear_history();
   printf("Quitting\n");
   return 0;
}

/*

   Need to error trap for White space input (can trap other error input here as well)
   Need to add the exit command to break the loop
    Added 14/02/2016
*/
