//***************************************************************
// Chet Lockwood
// Last Update: 25 NOV 2020
// Description: Custom C Shell Program
//***************************************************************

#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>

#define clearScreen() printf("\033[2J\033[1;1H")  //Clear screen ASCII escape code
#define clearLine() printf("\33[2K\r")        //Clear current line ASCII escape code
#define c "\033[38;5;44m"
#define g "\033[92m"
#define y "\033[38;5;226m"
#define b "\033[38;5;75m"
#define r "\033[38;5;196m"
#define dr "\033[31m"
#define w "\033[0m"

int exitStatus = 0;                           //Global variable for whether exit command has been called
int batchMode = 0;
int calledNewShell = 0;

//--------------------------------------------------------------------------------------------------
// void sleepMS 
//  Arguments:
//      long ms: time passed in milliseconds
//  Returns:
//      Nothing, executes a nanosleep function with appropriate math for millisecons
//  Purpose:
//      Millisecond version of sleep for smaller steps
//--------------------------------------------------------------------------------------------------
void sleepMS(long ms)
{
  struct timespec s;
  s.tv_sec = 0;               //seconds
  s.tv_nsec = ms * 1000000;   //milliseconds, multiplied by 1x10^6 to get nanoseconds

  nanosleep(&s, NULL);        //nanosleep function, like sleep but with nanoseconds
}

//--------------------------------------------------------------------------------------------------
// void display 
//  Arguments:
//      argc: Holds amount of arguments to determine Interactive vs Batch for display
//  Returns:
//      Nothing, displays a shell splash screen
//  Purpose:
//      Looks great and gives some pizzaz to the shell start up.  Version number,
//      mode changes from interactive to batch when loading commands from a file
//      names of authors, and goes through a loading percentage to simulate loading the shell
//--------------------------------------------------------------------------------------------------
void display(int argc)
{
  clearScreen();
  printf("%s* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n", g);
  printf("*                                                                                           *\n");
  printf("*                                            %s*****%s                                          *\n", c, g);
  printf("*                                        %s*************%s                                      *\n", c, g);
  printf("*                                   %s***********************%s                                 *\n", c, g);
  printf("*                               %s*******************************%s                             *\n", c, g);
  printf("*                           %s(/***********************************%s                           *\n", c, g);
  printf("*                           %s#####/**************************.....%s                           *\n", c, g);
  printf("*                           %s#########(*****************..........%s                           *\n", c, g);
  printf("*                           %s##############*********..............%s                           *\n", c, g);
  printf("*                           %s##################*..................%s                           *\n", c, g);
  printf("*                           %s##################*..................%s                           *\n", c, g);
  printf("*                          %s*%s##################*..................%s*%s                          *\n",y,c,b,g);
  printf("*                      %s*****%s##################*..................%s*****%s                      *\n",y,c,b,g);
  printf("*                  %s/********%s##################*..................%s*********%s                  *\n",y,c,b,g);
  printf("*             %s****************%s/###############*...............%s*****************%s             *\n",y,c,b,g);
  printf("*         %s#************************%s###########*...........%s*************************%s         *\n",y,c,b,g);
  printf("*        %s#####*************************%s#######*. ...../%s/************************....%s        *\n",y,c,b,g);
  printf("*        %s########**********************....%s###*....%s#************************........%s        *\n",y,c,b,g);
  printf("*        %s############/************...........*%s/#########****************............%s        *\n",y,b,g);
  printf("*        %s#################****...............*%s/##############******.................%s        *\n",y,b,g);
  printf("*        %s###################*................*%s/##################*..................%s        *\n",y,b,g);
  printf("*        %s###################*................*%s/##################*..................%s        *\n",y,b,g);
  printf("*        %s###################*................*%s/##################*..................%s        *\n",y,b,g);
  printf("*        %s###################*................*%s/##################*..................%s        *\n",y,b,g);
  printf("*          %s#################*................(%s (#################*.................%s         *\n",y,b,g);
  printf("*             %s##############*............           %s#############*.............%s             *\n",y,b,g);
  printf("*                 %s##########*........                   %s#########*.........%s                 *\n",y,b,g);
  printf("*                     %s######*....                           %s#####*....%s                      *\n",y,b,g);
  printf("*                         %s#&&                                    %s&%s                          *\n",y,b,g);
  printf("*                                                                                           *\n");
  printf("*%s   ______   __  __     ______        ______     __  __     ______     __         __        %s*\n",r,g);
  printf("*%s  /\\__  _\\ /\\ \\_\\ \\   /\\  ___\\      /\\  ___\\   /\\ \\_\\ \\   /\\  ___\\   /\\ \\       /\\ \\       %s*\n",r,g);
  printf("*%s  \\/_/\\ \\/ \\ \\  __ \\  \\ \\  __\\      \\ \\___  \\  \\ \\  __ \\  \\ \\  __\\   \\ \\ \\____  \\ \\ \\____  %s*\n",r,g);
  printf("*%s     \\ \\_\\  \\ \\_\\ \\_\\  \\ \\_____\\     \\/\\_____\\  \\ \\_\\ \\_\\  \\ \\_____\\  \\ \\_____\\  \\ \\_____\\ %s*\n",r,g);
  printf("*%s      \\/_/   \\/_/\\/_/   \\/_____/      \\/_____/   \\/_/\\/_/   \\/_____/   \\/_____/   \\/_____/ %s*\n",r,g);
  printf("*                                                                                           *\n");
  printf("*%s                              *** CUSTOM SHELL PROGRAM ***                                 %s*\n",w,g);
  printf("*%s    version 1.0.0                                                                          %s*\n",w,g);
  if(batchMode != 1) 
    printf("*%s    Interactive Mode                                                                       %s*\n",w,g);
  else
  {
    printf("*%s    Batch Mode                                                                             %s*\n",w,g);
  }
  printf("*%s    by Chet Lockwood                                                                       %s*\n",w,g);
  printf("*                                                                                           *\n");
  printf("* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *\n");
    
  //Mock loading sequence
  for (int i = 0; i <= 100; i++)
  {
    sleepMS(17);
    clearLine();                      //call to ASCII escape code #define'd for clearing current line 
    printf("LOADING ... %d%%", i);
    fflush(stdout);                   //flushing so display is proper in conjunction with sleep functions
  }
  clearLine();
  printf("LOADING COMPLETE");
  fflush(stdout);
  sleep(1);
  clearLine();
  fflush(stdout);
}

//--------------------------------------------------------------------------------------------------
// void prompt 
//  Arguments:
//      None
//  Returns:
//      Nothing
//  Purpose:
//      This is a function for the user prompt.  Just like Bash, displays the current working
//      directory using the getcwd function
//--------------------------------------------------------------------------------------------------

void prompt()
{
  char pwd [512];
  getcwd(pwd, sizeof(pwd));       //Gets the current working directory
  printf("%s: ", pwd);
}

//--------------------------------------------------------------------------------------------------
// void readCmdParser NEEDS FIXING
//  Arguments:
//      *input: array of commands from user input
//      **cmds: 2d array (pointer to pointers) for holding command strings
//  Returns:
//      Nothing 
//  Purpose:
//      Parses multiple commands from the batch file into a 2d array for later parsing (and command reading)
//      ,separates by new-line
//--------------------------------------------------------------------------------------------------

void readCmdParser (char *input, char **cmds)
{
  int i = 0;
  do
  {
    cmds[i] = strtok_r(input, "\n", &input);    //strtok_r used for easy tokening by specific delimiters
    i++;
  }  while (cmds[i-1] != NULL);                  //a NULL terminator will be placed in last position, check to exit
}

//--------------------------------------------------------------------------------------------------
// void cmdParser 
//  Arguments:
//      *input: array of commands from user input
//      **cmds: 2d array (pointer to pointers) for holding command strings
//  Returns:
//      Nothing 
//  Purpose:
//      Parses multiple commands from the user input into a 2d array for later parsing, separates by semi-colon
//--------------------------------------------------------------------------------------------------

void cmdParser (char *input, char **cmds)
{
  int i = 0;
  do
  {
    cmds[i] = strtok_r(input, ";\n", &input);    //strtok_r used for easy tokening by specific delimiters
    i++;
  }  while (cmds[i-1] != NULL);                  //a NULL terminator will be placed in last position, check to exit
}

//--------------------------------------------------------------------------------------------------
// void parser 
//  Arguments:
//      *input: array of commands from user input/previous command chains that were parsed
//      **argv: 2d array (pointer to pointers) for holding command strings
//  Returns:
//      Nothing
//  Purpose:
//      Parses out individual commands from the user input into a 2d array for later 
//      parsing, separates by whitespace
//--------------------------------------------------------------------------------------------------

void parser(char *input, char **argv)
{
  int i = 0;
  
  do
  {
    argv[i] = strtok_r(input, " \n;", &input);  //strtok_r used for easy tokening by specific delimiters
    i++;
  }  while (argv[i-1] != NULL);                 //a NULL terminator will be placed in last position, check to exit
}

//--------------------------------------------------------------------------------------------------
// void pipeParser 
//  Arguments:
//      *input: array of commands from user input/previous command chains that were parsed
//      **cmds: 2d array (pointer to pointers) for holding command strings
//  Returns:
//      Nothing
//  Purpose:
//      Parses out multiple commands from the user input into a 2d array for later 
//      parsing, separates by pipe '|'
//--------------------------------------------------------------------------------------------------

void pipeParser (char *input, char **cmds)
{
  int i = 0;

  //Prints off the piped command chain for batch mode
  if (batchMode == 1 )
  {
    int i = 0;
    sleepMS(800);
    printf("\n%s", input);
    fflush(stdout);
    sleepMS(800);
    printf("\n\n");
  }

  do
  {
    cmds[i] = strtok_r(input, "|", &input);     //strtok_r used for easy tokening by specific delimiters
    i++;
  }  while (cmds[i-1] != NULL);                 //a NULL terminator will be placed in last position, check to exit
}

//--------------------------------------------------------------------------------------------------
// int isExit 
//  Arguments:
//      *cmd: array of a single command
//  Returns:
//      0 if it is an exit command, 1 if it is not an exit command
//  Purpose:
//      checks to see whether an individual command is the exit command
//--------------------------------------------------------------------------------------------------

int isExit(char *cmd)
{
  char exit[] = {"exit"};         //char array containing exit to check against
  char copy[512] = {'\0'};        //a copy array to hold the passed cmd array to prevent changes
  *copy = *cmd;                   //Copys the first position to the copy array
  int j = 0;

  for (int i = 0; i < strlen(cmd); i++)
  {
    if (cmd[i] != ' ')            //Checks for not whitespace
    {
      copy[j]=cmd[i];             //copies if not
      j++;                        //iterates j for the copy array
    }
  }
  
  if (strcmp(copy, exit) == 0)    //Compares finished de-whitepaced strings for the exit command
    return 0;
  else
    return 1;
}

//--------------------------------------------------------------------------------------------------
// int isMixed
//  Arguments:
//      *cmd: array of a single command (in this case, with pipes)
//  Returns:
//      0 if there is redirection, 1 if there is not
//  Purpose:
//      Checks to see if there are I/O redirection operators mixed with piped commands
//--------------------------------------------------------------------------------------------------

int isMixed(char *cmd)
{
  for (int i = 0; i < strlen(cmd); i++)
  {
    if (cmd[i] == '>' || cmd[i] == '<')
      return 0;               
  }
  return 1;
}

//--------------------------------------------------------------------------------------------------
// int isPiped 
//  Arguments:
//      *cmd: array of a single command
//  Returns:
//      pipeCount: the count for the number of pipes in a command chain
//  Purpose:
//      Checks to see if there are pipes present in a given set of commands (after separating by semi-colon)
//--------------------------------------------------------------------------------------------------

int isPiped(char *cmd)
{
  char pipe = {'|'};
  int pipeCount = 0;
 
  for (int i = 0; i < strlen(cmd); i++)
  {
    if (cmd[i] == '|')
      pipeCount++;                //iterate pipeCount if there is a pipe in command chain
  }
  return pipeCount;
}

//--------------------------------------------------------------------------------------------------
// int isNewShell FIX
//  Arguments:
//      *cmd: array of a single command
//  Returns:
//      0 if it is an exit command, 1 if it is not an exit command
//  Purpose:
//      checks to see whether an individual command is the exit command
//--------------------------------------------------------------------------------------------------

int isNewShell(char *cmd)
{
  char newShell[] = {"newshell"};         //char array containing exit to check against
  char copy[512] = {'\0'};        //a copy array to hold the passed cmd array to prevent changes
  *copy = *cmd;                   //Copys the first position to the copy array
  int j = 0;

  for (int i = 0; i < strlen(cmd); i++)
  {
    if (cmd[i] != ' ')            //Checks for not whitespace
    {
      copy[j]=cmd[i];             //copies if not
      j++;                        //iterates j for the copy array
    }
  }
  
  if (strcmp(copy, newShell) == 0)    //Compares finished de-whitepaced strings for the exit command
    return 0;
  else
    return 1;
}

//--------------------------------------------------------------------------------------------------
// void newShellParser FIX
//  Arguments:
//      *input: array of commands from user input/previous command chains that were parsed
//      **argv: 2d array (pointer to pointers) for holding command strings
//  Returns:
//      Nothing
//  Purpose:
//      Parses out individual commands from the user input into a 2d array for later 
//      parsing, separates by whitespace
//--------------------------------------------------------------------------------------------------

void newShellParser(char *input, char **argv)
{
  int i = 0;
  
  do
  {
    argv[i] = strtok_r(input, " \n", &input);  //strtok_r used for easy tokening by specific delimiters
    i++;
  }  while (argv[i-1] != NULL);                 //a NULL terminator will be placed in last position, check to exit
  if (argv[0] == NULL)
    argv[0] = "none";
}

//--------------------------------------------------------------------------------------------------
// void execFile
//  Arguments:
//      **argv: array containing commands
//  Returns:
//      Nothing
//  Purpose:
//      Creates a child by forking off the parent (shell) to run execvp from the argv/argument list
//--------------------------------------------------------------------------------------------------
void execFile (char **argv)
{
  pid_t pid;                          //variable for process ID
  int status;                         //int for status of the process

  //Prints off the command for batch mode (including options and additional arguments)
  if (batchMode == 1 )
  {
    int i = 0;
    sleepMS(800);
    printf("\n");
    while (argv[i] != NULL)
    {
      printf("%s ", argv[i]);
      i++;
    }
    fflush(stdout);
    sleepMS(800);
    printf("\n\n");
  }

  if ((pid = fork()) < 0)             //check to ensure the fork was successful, returns -1 if not
    perror("fork error");
  else if (pid == 0)                  //child
  {
    if(execvp(*argv, argv) < 0)       //check to ensure the execution was successful, returns -1 if not
    {
      fprintf(stderr, "%s %s not a valid command", "ERROR:", argv[0]);
      exit(1);                        //Need to for exit the process to prevent undefined behaviour
    }
  }
  else
    wait(&status);                    //Parent(shell) waiting for the child process to finish
}

//--------------------------------------------------------------------------------------------------
// void onePipe
//  Arguments:
//      *input: the chain of commands, including the pipe
//      **argv: a 2d array for later parsing commands
//  Returns:
//      Nothing
//  Purpose:
//      Function that acts like Bash pipe for a single pipe/ two commands chain
//--------------------------------------------------------------------------------------------------

void onePipe(char *input, char **argv)
{
  char *args[512];                      //2d array to break down piped commands into individual commands
  char *cmds1[512];                     //First command and options
  char *cmds2[512];                     //Second command and options
  int i = 0;
  
  //Catching if I/O redirection is mixed in with piping and preventing it
  if(isMixed(input) == 0)
  {
    printf("%s: Pipe and I/O redirection are not to be mixed", input);
    return;
  }

  pipeParser(input, args);              //Parses the piped commands into individual commands
  
  parser(args[0], cmds1);               //Parses commands and options into separate arguments
  parser(args[1], cmds2);
  
  int status;
  int fd1[2];                           //file descriptor for single pipe
  pid_t pid;

  if (pipe(fd1) < 0)                    //Error checking to ensure pipe worked
  {
    fprintf(stderr, "%s", "ERROR: pipe did not open");
    return;
  }

  if ((pid =fork()) < 0)                //Error checking to ensure the fork was successful
  { 
    perror("fork error");
  }
  else
  {
    if(pid == 0)              
    {
      dup2(fd1[1], 1);                  //Duplicate write end of first command to stdout

      close(fd1[0]);                    //Close both ends of pipe
      close(fd1[1]);

      if(execvp(*cmds1, cmds1) < 0)
      {
        fprintf(stderr, "%s %s not a valid command", "ERROR:", cmds1[0]);
        exit(1);                        //Need to for exit the process to prevent undefined behaviour
      }
    }
    else
    {
      if ((pid = fork()) < 0)
      {
        perror("fork error");
      }
      else
      {
        if(pid == 0)
        {
          dup2(fd1[0], 0);              //Duplicate read end from the first command to stdin

          close(fd1[0]);
          close(fd1[1]);

          if(execvp(*cmds2, cmds2) < 0)
          {
            fprintf(stderr, "%s %s not a valid command", "ERROR:", cmds2[0]);
            exit(1);                     //Need to for exit the process to prevent undefined behaviour
          }
        }
      }
    } 
  }

  close(fd1[0]);
  close(fd1[1]);

  for (i = 0; i < 2; i++)                //Parent (shell) waits for all processes to end
  {
    wait(&status);
  }
}

//--------------------------------------------------------------------------------------------------
// void twoPipe 
//  Arguments:
//      *input: the chain of commands, including the pipe
//      **argv: a 2d array for later parsing commands
//  Returns:
//      Nothing
//  Purpose:
//      Function that acts like Bash pipe for a two pipe/ three commands chain
//--------------------------------------------------------------------------------------------------

void twoPipe(char *input, char **argv)
{
  char *args[512];
  char *cmds1[512];
  char *cmds2[512];
  char *cmds3[512];
  int i = 0;
  
  //Catching if I/O redirection is mixed in with piping and preventing it
  if(isMixed(input) == 0)
  {
    printf("%s: Pipe and I/O redirection are not to be mixed", input);
    return;
  }
  
  pipeParser(input, args);
  
  parser(args[0], cmds1);
  parser(args[1], cmds2);
  parser(args[2], cmds3);

  int status;
  int fd1[2], fd2[2];                         //Two file descriptors for two pipes
  pid_t pid;

  if(pipe(fd1) < 0)                           //First pipe for command 1 -> command 2
  {
    fprintf(stderr, "%s", "ERROR: first pipe did not open");
    return;
  }
  if(pipe(fd2) < 0)                           //Second pipe for command 2 -> command 3
  {
    fprintf(stderr, "%s", "ERROR: second pipe did not open");
    return;
  }

  if ((pid = fork()) < 0)
  {
    perror("fork error");
  }
  else
  {
    if (pid == 0)
    {
      dup2(fd1[1], 1);                        //Duplicate write end of command 1 -> command 2 to stdout

      close(fd1[0]);                          //Need to close all pipes
      close(fd1[1]);
      close(fd2[0]);
      close(fd2[1]);

      if(execvp(*cmds1, cmds1) < 0)
      {
        fprintf(stderr, "%s %s not a valid command", "ERROR:", cmds1[0]);
        exit(1);                              //Need to for exit the process to prevent undefined behaviour
      }
    }
    else
    {
      if ((pid = fork()) < 0)
      {
        perror("fork error");
      }
      else
      {
        if (pid == 0)
        {
          dup2(fd1[0], 0);                    //Duplicate read end of command 1 -> command 2 to stdin

          dup2(fd2[1], 1);                    //Duplicate write end of command 2 -> command 3 to stdout

          close(fd1[0]);                      //Need to close all pipes again
          close(fd1[1]);
          close(fd2[0]);
          close(fd2[1]);

          if(execvp(*cmds2, cmds2) < 0)
          {
            fprintf(stderr, "%s %s not a valid command", "ERROR:", cmds2[0]);
            exit(1);                          //Need to for exit the process to prevent undefined behaviour
          }
        }
        else
        {
          if ((pid = fork()) < 0)
          {
            perror("fork error");
          }
          else
          {
            if (pid == 0)
            {
              dup2(fd2[0], 0);                //Duplicate the read end of command 2 -> command 3 to stdin

              close(fd1[0]);                  //Need to close all pipes
              close(fd1[1]);
              close(fd2[0]);
              close(fd2[1]);

              if(execvp(*cmds3, cmds3) < 0)
              {
                fprintf(stderr, "%s %s not a valid command", "ERROR:", cmds3[0]);
                exit(1);                        //Need to for exit the process to prevent undefined behaviour
              }
            }
          }
        }
      }
    }  
  }

  close(fd1[0]);
  close(fd1[1]);
  close(fd2[0]);
  close(fd2[1]);

  for (i = 0; i < 3; i++)                       //Parent (shell) waiting for all three processes to finish
  {
    wait(&status);
  }
}

//--------------------------------------------------------------------------------------------------
// void processor
//  Arguments:
//      *input: array containing input
//      **argv: 2D array containing input commands
//  Returns:
//      Nothing
//  Purpose:
//      Central logic location for handling commands from both interactive and batch mode of all types
//--------------------------------------------------------------------------------------------------

void processor(char *input, char **argv)
{
  char *cmds[512];                        //Another 2D array for holding more separations of commands
  int i = 0;
  
  cmdParser(input, cmds);                 //Tokenizer for commands separated by semi-colon/multiple commands
  while (cmds[i] != NULL)                 //While not out of commands
  {
    if (isExit(cmds[i]) == 0)             //Check to see if any of the commands are an exit command
    {
      exitStatus++;
      i++;                                //skip the exit command
      continue;
    }
    if (isPiped(cmds[i]) == 1)            //Check to see if any of the commands are piped
    {
      onePipe(cmds[i], argv);
      i++;
      printf("\n");
      continue;
    }
    else if (isPiped(cmds[i]) == 2)
    {
      twoPipe(cmds[i], argv);
      i++;
      printf("\n");
      continue;
    }
    else
    {
      parser(cmds[i], argv);               //Further separate the commands into individual arguments
      execFile(argv);                      //Execute the commands
      i++;                                 //iterate to next command
      printf("\n");
    }  
  }
}

//--------------------------------------------------------------------------------------------------
// void bMode FIX
//  Arguments:
//      *input: array containing input
//      **argv: 2D array containing input commands
//  Returns:
//      Nothing
//  Purpose:
//      Central logic location for handling commands from both interactive and batch mode of all types
//--------------------------------------------------------------------------------------------------

void bMode(char **args)
{
  char input[512] = {'\0'};
  char *readCmds[512];
  
  FILE *file = fopen(args[1], "r");      //Attempt to open batch file
    if(file==0)
    {
      fprintf(stderr, "%s", "ERROR: file does not exist\n");
      return;
      //exit(1);
    }
    else
    {
      int i = 0;
      char m;
      fscanf(file, "%c", &m);
      while(!feof(file))
      {       
        //fread(input, 512, 1, file);      //read batch file into input array
        input[i] = m;
        fscanf(file, "%c", &m);
        i++;
      }
      fclose(file);                        //Close batch file
    }
    
    readCmdParser(input, readCmds);        //Breaks down command lines to be displayed and executed

    int i = 0;
    while (readCmds[i] != NULL)
    {
      printf("Executing command: %s", readCmds[i]);
      processor(readCmds[i], args);                //call the central logic to execute batch before prompt
      printf("\n");
      i++;
    }
    printf("%s", w);
    exit(0);
}

/////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////int main/////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////

int main(int argc, char **argv)
{
  if(argc == 2)
  {
    batchMode++;
    display(argc);                           //Call splash screen
  }
    

  char input[512] = {'\0'};                //char array to hold input of commands
  char *readCmds[512];

  if (argc > 2)
  {
    fprintf(stderr, "%s", "ERROR: Too many arguments\n");
    fprintf(stderr, "%s", "Usage: newshell [batchFile]\n");
    exit(1);
  }
  if (argc == 2)                            //Check for batch file
  {
    FILE *file = fopen(argv[1], "r");      //Attempt to open batch file
    if(file==0)
    {
      fprintf(stderr, "%s", "ERROR: file does not exist\n");
      exit(1);
    }
    else
    {
      int i = 0;
      char m;
      fscanf(file, "%c", &m);
      while(!feof(file))
      {       
        //fread(input, 512, 1, file);      //read batch file into input array
        input[i] = m;
        fscanf(file, "%c", &m);
        i++;
      }
      fclose(file);                        //Close batch file
    }
    
    readCmdParser(input, readCmds);        //Breaks down command lines to be displayed and executed

    int i = 0;
    while (readCmds[i] != NULL)
    {
      printf("Executing command: %s", readCmds[i]);
      processor(readCmds[i], argv);                //call the central logic to execute batch before prompt
      printf("\n");
      i++;
    }
    printf("%s", w);
    exit(0);
  }

  ////////////////////////////////////////////END EXECUTION BATCH FILE//////////////////////////////////////////////////

  int iteration = 0;
  char *nShellArg[512];

  while (1)
  {
    if (iteration == 0)
    {
      prompt();                              //Displays prompt cursor
      fgets(input, 512, stdin);              //Using gets instead of scanf to get the full line of commands
      iteration++;
    }
    
    newShellParser(input, nShellArg);

    if(isNewShell(nShellArg[0]) == 0 )
    {
      if(nShellArg[2] != NULL)
      {
        fprintf(stderr, "%s", "ERROR: Too many arguments\n");
        fprintf(stderr, "%s", "Usage: newshell [batchFile]\n");
        printf("%s", w);
        
        sleep(1);
        iteration = 0;
        memset(nShellArg, 0, sizeof(nShellArg));
        clearScreen();
        continue;
        
        //exit(1);
      }
      else if (nShellArg[1] != NULL)
      {
        batchMode++;
        display(argc);
        bMode(nShellArg);

        sleep(1);
        iteration = 0;
        memset(nShellArg, 0, sizeof(nShellArg));
        clearScreen();
        batchMode--;
        continue;
      }
      display(argc);
      
      while (1)
      {
        char inputcpy[512] = {'\0'};           //char array to hold a copy of input of commands
        
        prompt();                              //Displays prompt cursor
        fgets(input, 512, stdin);              //Using gets instead of scanf to get the full line of commands

        strcpy(inputcpy, input);
        
        newShellParser(inputcpy, nShellArg);

        if(isNewShell(nShellArg[0]) == 0 )
          calledNewShell++;

        if (calledNewShell > 0)
          {
            calledNewShell = 0;
            break;
          }
        
        if ( strlen(input) > 1)                //Check that a command has been input/not only enter key hit
        {
          processor(input, argv);              //Call central logic to execute user input
          printf("\n");
          
          if (exitStatus > 0)                  //exit now after all commands have been executed, if exit was called
          {
            printf("%s", w);
            exit(0);
          }
        }
        memset(nShellArg, 0, sizeof(nShellArg));
      }      
    }
    else
      iteration--;
  }

  return 0;
}