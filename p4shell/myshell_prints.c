#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/types.h> 

#define MAX_LEN 512

void myPrint(char *msg) 
{
    write(STDOUT_FILENO, msg, strlen(msg));
}

void errorPrint(){
/* 
    If a command does not exist or a command is over the length limit 
*/ 
    char error_message[30] = "An error has occured\n"; 
    write(STDOUT_FILENO, error_message, strlen(error_message)); 
}

void remove_leading_space(char* str){
  int index, i;

    index = 0;
    /* Find last index of whitespace character */
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }
    if(index != 0)
    {
        /* Shift all trailing characters to its left */
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        str[i] = '\0'; // Make sure that string is NULL terminated
    }
}

int my_cd(char *args){
/*
    changes directory
    args[0] is "cd".  args[1] is the directory.
    always returns 1, to continue executing.
 */
  char* cd_path = strchr(args, ' '); 
  if(cd_path == NULL){ 
    chdir(getenv("HOME")); 
  } else {
    remove_leading_space(cd_path); 
    if (chdir(cd_path) != 0) {
        errorPrint();
    } 
  } 
  return 1;
}

char** split_line(char* pinput, int* num_args) {
/*  
    splits line into tokens 
    returns null terminated array of tokes 
*/ 
  int buffsize = MAX_LEN; 
  int position = 0;
  char delim[5] = {'\t', '\r', '\n', ';'}; 
  char **tokens = malloc(buffsize * sizeof(char *));
  char *token;

  if (!tokens) {
    exit(0);
  }

  //first tokenize 
  token = strtok(pinput, delim);
  
  //iterate through getting every token 
  while (token != NULL) {
    remove_leading_space(token); 
    tokens[position] = token;
    position++;
    //tokenize again 
    token = strtok(NULL, delim);
  }
  tokens[position] = NULL;
  *num_args = position;
  for(int i = 0; i < position; i++){
    printf("split line - tokens[i] = (%s)\n", tokens[i]); 
  } 
  return tokens;
}

char** split_space(char* pinput) {
/*  
    splits line into tokens 
    returns null terminated array of tokes 
*/ 
  int buffsize = MAX_LEN; 
  int position = 0;
  char delim[5] = {' '}; 
  char **tokens = malloc(buffsize * sizeof(char *));
  char *token;

  if (!tokens) {
    exit(0);
  }

  //first tokenize 
  token = strtok(pinput, delim);
  
  //iterate through getting every token 
  while (token != NULL) {
    remove_leading_space(token); 
    tokens[position] = token;
    position++;
    //tokenize again 
    token = strtok(NULL, delim);
  }
  tokens[position] = NULL;
  for(int i = 0; i < position; i++){
    printf("split space - tokens[i] = (%s)\n", tokens[i]); 
  } 
  return tokens;
}

int execute(char **args)
{
/*
  executes 
  takes null terminated list of arguments (including program)
  always returns 1, to continue execution.
*/
  pid_t pid;
  int status;
  printf("from exec -- arg[0] = (%s)\n", args[0]); 
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      printf("this error?\n"); 
      errorPrint(); 
    }
    exit(0);
  } else if (pid < 0) {
    // Error forking
    printf("**error fork\n"); 
    errorPrint();
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}

int main(int argc, char *argv[]) 
{
    char cmd_buff[MAX_LEN];
    char *pinput;
    char **args;
    
    FILE* batch_file;
    int batch_flag = 0; 
    int num_args = 0; 


    if(argc == 2){
        batch_file = fopen(argv[1], "r"); 
        if(batch_file == NULL){
          errorPrint(); 
          exit(0); 
        }
        batch_flag = 1;
    }

    
    while (1) {
      //BATCH MODE 
        if(batch_flag){
          pinput = fgets(cmd_buff, MAX_LEN, batch_file);
          if(!pinput){
            errorPrint(); 
            exit(0); 
          }
          myPrint(pinput); 
          remove_leading_space(pinput); 
        }else{
      //INTERACTIVE MODE 
        myPrint("myshell> ");
        pinput = fgets(cmd_buff, MAX_LEN, stdin);        
        //exit if no input 
        if (!pinput) {
            errorPrint(); 
            exit(0);
        }
      } 
        //PARSE 
        char* ptr = pinput; 
        args = split_line(ptr, &num_args); 
        for(int i =0; i < num_args; i++){
    
          //CHECKING FOR BUILT INS 
          if(strcmp(args[i], "exit") == 0){ 
            exit(0); 
          }
          else if(strcmp(args[i], "pwd") == 0){ 
            char wd[MAX_LEN]; 
            myPrint(getcwd(wd, MAX_LEN));
            myPrint("\n");
          }
          else if(strstr(args[i], "cd") != NULL){
            my_cd(args[i]);
          }
          else { 
            //EXECUTION 
            char **exec_args;
            //exec_args= malloc (4 * sizeof (char *));
            exec_args = split_space(args[i]); 
            //exec_args[0] = malloc (514 * sizeof (char));
            //strcpy(exec_args[0], args[i]);
            //exec_args[1] = NULL;
            printf("execargs[0] = %s\n, execargs[1] = %s\n", exec_args[0], exec_args[1]); 
            execute(exec_args); 
            free(exec_args);

          }
        }
                      
    }
  if(batch_flag){
    fclose(batch_file); 
  }
} 

/* 
TO DO: 
- redirection 
*/ 