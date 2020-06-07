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
/* 
  removes leading spaces 
*/ 
  int index, i;

    index = 0;
    //find last index 
    while(str[index] == ' ' || str[index] == '\t' || str[index] == '\n')
    {
        index++;
    }
    if(index != 0)
    {
        //shift 
        i = 0;
        while(str[i + index] != '\0')
        {
            str[i] = str[i + index];
            i++;
        }
        // null terminated 
        str[i] = '\0'; 
    }
}

int my_cd(char *args){
/*
    changes directory
    args[0] is "cd".  args[1] is the directory.
    always returns 1, to continue executing.
 */
  char* cd_path = strchr(args, ' '); 
  //if no path or $HOME 
  if(cd_path == NULL){ 
    chdir(getenv("HOME")); 
  } else {
    // go to path 
    remove_leading_space(cd_path);
    if(strcmp(cd_path, "$HOME") == 0){
      chdir(getenv("HOME"));
    } 
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
    errorPrint(); 
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
  pid = fork();
  if (pid == 0) {
    // Child process
    if (execvp(args[0], args) == -1) {
      errorPrint(); 
    }
    exit(0);
  } else if (pid < 0) {
    // Error forking
    errorPrint();
  } else {
    // Parent process
    do {
      waitpid(pid, &status, WUNTRACED);
    } while (!WIFEXITED(status) && !WIFSIGNALED(status));
  }
  return 1;
}



int to_redirect_or_not(char* arg){
/* 
    returns 1 if normal redirect
    returns 2 if advanced 
    returns 0 if none 
*/ 
  int i = 0;
  while(cmd[i] != '\0'){
      //normal 
      if(cmd[i] == '>'){
        //advanced 
        if(cmd[i + 1] == '+' ){
          return 2; 
        }
        return 1; 
      }
  }
  return 0;  
}

int invalid_redirect(char* arg, int num_pipe, int num_files, int no_cmd, int built_in){ 
/* 
  checks: 
    no more than one < 
    no more than one file 
    no command 
    built in commands
*/ 
  int pos_redir = 0;
  int redirect_type = 0;  
  num_pipe = 0; 
  num_files = 0; 
  no_cmd = 0; 
  built_in = 0; 

  redirect_type = to_redirect_or_not(arg); 

  for(int i = 0; arg[i] != '\0'; i++){
    // checking num pipes 
    if(arg[i] == '>'){
      num_pipe++;
      // setting position of redirect for both cases 
      pos_redir = i; 
      if(redirect_type == 2){
        pos_redir = i + 1; 
      }
      i++;  
    }
    
  }
}

  if(num_pipe == 1 && num_files == 1 && no_cmd == 0 && built_in == 0){
    return 1; 
  }
  else{
    return 0; 
  }
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
          //exit if no input 
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
          //REDIRECTION 
          int redirect_flag; 
          redirect_flag = to_redirect_or_not(args[i]); 
          if(redirect_flag == 1){
            //normal redirection 
          }
          else if(redirect_flag == 2){
            //advanced 
          }
          else{
            //everything else 
          }
    
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
            execute(exec_args); 
            free(exec_args);

          }
        }
                      
    }
  if(batch_flag){
    fclose(batch_file); 
  }
} 