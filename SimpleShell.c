/******************************************************************************
@file         SimgpleShell.c
@author       Darren Lowe, Jacqueline Catlin, Georgia Greegoriou,
@date         Thursday, 11/02/2016
@brief
*******************************************************************************/

#define SIMPLE_TOK_BUFSIZE 64
#define SIMPLE_TOK_DELIM " \t\r\n\a"

int main(int argc, char **argv)
{
    // Load config files, if any.
    
    // Run command loop.
    simple_loop();
    
    // Perform any shutdown/cleanup.
    
    return EXIT_SUCCESS;
}


void simple_loop(void)
{
    char *line;
    char **args;
    int status;
    
    do {
        printf("> ");
        line = simple_read_line();
        args = simple_split_line(line);
        status = simple_execute(args);
        
        free(line);
        free(args);
    } while (status);
}



char *simple_read_line(void)
{
    char *line = NULL;
    ssize_t bufsize = 0; // have getline allocate a buffer for us
    getline(&line, &bufsize, stdin);
    return line;
}



char **simple_split_line(char *line)
{
    int bufsize = SIMPLE_TOK_BUFSIZE, position = 0;
    char **tokens = malloc(bufsize * sizeof(char*));
    char *token;
    
    if (!tokens) {
        fprintf(stderr, "lsh: allocation error\n");
        exit(EXIT_FAILURE);
    }
    
    token = strtok(line, SIMPLE_TOK_DELIM);
    while (token != NULL) {
        tokens[position] = token;
        position++;
        
        if (position >= bufsize) {
            bufsize += SIMPLE_TOK_BUFSIZE;
            tokens = realloc(tokens, bufsize * sizeof(char*));
            if (!tokens) {
                fprintf(stderr, "simple: allocation error\n");
                exit(EXIT_FAILURE);
            }
        }
        
        token = strtok(NULL, SIMPLE_TOK_DELIM);
    }
    tokens[position] = NULL;
    return tokens;
}


int simple_launch(char **args)
{
    pid_t pid, wpid;
    int status;
    
    pid = fork();
    if (pid == 0) {
        // Child process
        if (execvp(args[0], args) == -1) {
            perror("simple - error");
        }
        exit(EXIT_FAILURE);
    } else if (pid < 0) {
        // Error forking
        perror("simple - error");
    } else {
        // Parent process
        do {
            wpid = waitpid(pid, &status, WUNTRACED);
        } while (!WIFEXITED(status) && !WIFSIGNALED(status));
    }
    
    return 1;
}