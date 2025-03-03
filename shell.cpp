// ACADEMIC INTEGRITY PLEDGE
//
// - I have not used source code obtained from another student nor
//   any other unauthorized source, either modified or unmodified.
//
// - All source code and documentation used in my program is either
//   my original work or was derived by me from the source code
//   published in the textbook for this course or presented in
//   class.
//
// - I have not discussed coding details about this project with
//   anyone other than my instructor. I understand that I may discuss
//   the concepts of this program with other students and that another
//   student may help me debug my program so long as neither of us
//   writes anything during the discussion or modifies any computer
//   file during the discussion.
//
// - I have violated neither the spirit nor letter of these restrictions.
//
//
//
// Signed: Jacob Neuman Date: 3/4/2025

// 3460:426 Lab 1 - Basic C shell rev. 9/10/2020

/* Basic shell */

/*
 * This is a very minimal shell. It finds an executable in the
 * PATH, then loads it and executes it (using execv). Since
 * it uses "." (dot) as a separator, it cannot handle file
 * names like "minishell.h"
 *
 * The focus on this exercise is to use fork, PATH variables,
 * and execv. 
 */

 #include <stdio.h>
 #include <string.h>
 #include <unistd.h>
 #include <sys/wait.h>
 #include <stdlib.h>
 
 #define MAX_ARGS		64
 #define MAX_ARG_LEN		16
 #define MAX_LINE_LEN	80
 #define WHITESPACE		" ,\t\n"
 
 struct command_t {
    char *name;
    int argc;
    char *argv[MAX_ARGS];
 };
 
 /* Function prototypes */
 int parseCommand(char *, struct command_t *);
 void printPrompt();
 void readCommand(char *);
 void printHelp();
 
 int main(int argc, char *argv[]) {
    int pid;
    int status;
    char cmdLine[MAX_LINE_LEN];
    struct command_t command;
 
    while (true) {
       printPrompt();
       /* Read the command line and parse it */
       readCommand(cmdLine);
       parseCommand(cmdLine, &command);
       command.argv[command.argc] = NULL;
      
       if(strcmp(command.name, "C") == 0) {
         command.name = (char *) malloc(3 * sizeof(char));
         command.argv[0] = (char *) malloc(3 * sizeof(char));
         strcpy(command.name, "cp");
         strcpy(command.argv[0], "cp");
       }

       else if(strcmp(command.name, "D") == 0) {
         command.name = (char *) malloc(3 * sizeof(char));
         command.argv[0] = (char *) malloc(3 * sizeof(char));
         strcpy(command.name, "rm");
         strcpy(command.argv[0], "rm");
       }

       else if(strcmp(command.name, "E") == 0) {
         command.name = (char *) malloc(5 * sizeof(char));
         command.argv[0] = (char *) malloc(5 * sizeof(char));
         strcpy(command.name, "echo");
         strcpy(command.argv[0], "echo");
       }

       else if(strcmp(command.name, "H") == 0) {
            printHelp();
       }

       else if(strcmp(command.name, "L") == 0) {
         printf("\n");

         char *pwdargs[] = {(char*)"pwd", NULL};
         if ((pid = fork()) == 0) {
            execvp("pwd", pwdargs);
         }
         wait(&status);

         printf("\n");

         command.name = (char *) malloc(3 * sizeof(char));
         command.argv[0] = (char *) malloc(3 * sizeof(char));
         command.argv[1] = (char *) malloc(3 * sizeof(char));
         strcpy(command.name, "ls");
         strcpy(command.argv[0], "ls");
         strcpy(command.argv[1], "-l");
         command.argc = 2;
         command.argv[command.argc] = NULL;
       }

       else if(strcmp(command.name, "M") == 0) {
         command.name = (char *) malloc(4 * sizeof(char));
         command.argv[0] = (char *) malloc(4 * sizeof(char));
         strcpy(command.name, "nano");
         strcpy(command.argv[0], "nano");
       }

       else if(strcmp(command.name, "P") == 0) {
         command.name = (char *) malloc(4 * sizeof(char));
         command.argv[0] = (char *) malloc(4 * sizeof(char));
         strcpy(command.name, "more");
         strcpy(command.argv[0], "more");
       }

       else if(strcmp(command.name, "Q") == 0) {
         break;
       }

       else if(strcmp(command.name, "W") == 0) {
         command.name = (char *) malloc(6 * sizeof(char));
         command.argv[0] = (char *) malloc(6 * sizeof(char));
         strcpy(command.name, "clear");
         strcpy(command.argv[0], "clear");
       }

       else if(strcmp(command.name, "X") == 0) {
         command.name = (char *) malloc(2 * sizeof(char) + sizeof(command.argv[1]));
         strcpy(command.name, "./");
         strncat(command.name, command.argv[1], strlen(command.argv[1]));

         for (int i = 0; i < command.argc; i++) {
            if (i == command.argc - 1) {
               command.argc--;
               command.argv[command.argc] = NULL;
            }
            else if (i == 0) {
               command.argv[0] = (char *) malloc(sizeof(command.name));
               strcpy(command.argv[0], command.name);
            }
            else {
               strcpy(command.argv[i], command.argv[i + 1]);
            }
         }
       }
       
       /* Create a child process to execute the command */
       pid = fork();
       if (pid == 0) {
          /* Child executing command */
          if (execvp(command.name, command.argv) == -1) {
            perror("Error executing command");
          }
          exit(EXIT_FAILURE);
       }
       else if (pid != 0) {
         waitpid(pid, &status, 0);
       }
       else {
         perror("Failed fork");
       }
    }
 
    /* Shell termination */
    printf("\n\nshell: Terminating successfully\n");
    return 0;
 }
 
 /* End basic shell */
 
 /* Parse Command function */
 
 /* Determine command name and construct the parameter list.
  * This function will build argv[] and set the argc value.
  * argc is the number of "tokens" or words on the command line
  * argv[] is an array of strings (pointers to char *). The last
  * element in argv[] must be NULL. As we scan the command line
  * from the left, the first token goes in argv[0], the second in
  * argv[1], and so on. Each time we add a token to argv[],
  * we increment argc.
  */
 int parseCommand(char *cLine, struct command_t *cmd) {
    int argc;
    char **clPtr;
    /* Initialization */
    clPtr = &cLine;	/* cLine is the command line */
    argc = 0;
    cmd->argv[argc] = (char *) malloc(MAX_ARG_LEN);
    /* Fill argv[] */
    while ((cmd->argv[argc] = strsep(clPtr, WHITESPACE)) != NULL) {
       cmd->argv[++argc] = (char *) malloc(MAX_ARG_LEN);
    }
 
    /* Set the command name and argc */
    cmd->argc = argc-1;
    cmd->name = (char *) malloc(sizeof(cmd->argv[0]));
    strcpy(cmd->name, cmd->argv[0]);
    return 1;
 }
 
 /* End parseCommand function */
 
 /* Print prompt and read command functions - Nutt pp. 79-80 */
 
 void printPrompt() {
    /* Build the prompt string to have the machine name,
     * current directory, or other desired information
     */
    char promptString[] = "linux(jsn32)|>";
    printf("%s ", promptString);
 }
 
 void readCommand(char *buffer) {
    /* This code uses any set of I/O functions, such as those in
     * the stdio library to read the entire command line into
     * the buffer. This implementation is greatly simplified,
     * but it does the job.
     */
    fgets(buffer, 80, stdin);
 }
 
 /* End printPrompt and readCommand */

 void printHelp() {
    printf("### cp (copy) command:\n");
    printf("Usage: cp <source> <destination>\n");
    printf("The 'cp' command is used to copy files or directories.\n\n");

    printf("### rm (remove) command:\n"); 
    printf("Usage: rm <file>\n");
    printf("The 'rm' command is used to remove files or directories.\n");
    printf("Be careful: It permanently deletes files without moving them to trash.\n\n");

    printf("### echo command:\n");
    printf("Usage: echo <text>\n");
    printf("The 'echo' command prints text to the terminal.\n\n");
 
    printf("### pwd (print working directory) command:\n");
    printf("Usage: pwd\n");
    printf("The 'pwd' command shows the current directory you are working in.\n\n");

    printf("### ls (list) command:\n");
    printf("Usage: ls\n");
    printf("The 'ls' command lists files and directories in the current directory.\n\n");
   
    printf("### nano command:\n");
    printf("Usage: nano <filename>\n");
    printf("The 'nano' command is a simple text editor in the terminal.\n\n");
 
    printf("### more command:\n");
    printf("Usage: more <file>\n");
    printf("The 'more' command allows you to view a file's content page by page.\n\n");

    printf("### clear command:\n");
    printf("Usage: clear\n");
    printf("The 'clear' command clears the terminal screen.\n\n");
 
    printf("### man (manual) command:\n");
    printf("Usage: man <command>\n");
    printf("The 'man' command shows the manual pages for any command.\n\n");
 }