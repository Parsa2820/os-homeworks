#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/wait.h>

#define FALSE 0
#define TRUE 1
#define INPUT_STRING_SIZE 80
#define FILE_SEPARATOR "/"
#define REDIRECT_IN "<"
#define REDIRECT_OUT ">"

#include "io.h"
#include "parse.h"
#include "process.h"
#include "shell.h"

tok_t *get_paths()
{
  static tok_t *paths = NULL;
  if (paths == NULL)
  {
    char *env_path = getenv("PATH");
    paths = getToks(env_path);
  }
  return paths;
}

char *combine_path(char *path, char *file)
{
  char *combined_path = malloc(strlen(path) + strlen(file) + 2);
  strcat(combined_path, path);
  strcat(combined_path, FILE_SEPARATOR);
  strcat(combined_path, file);
  return combined_path;
}

int file_exists(char *path)
{
  return access(path, F_OK) == 0;
}

int cmd_quit(tok_t arg[])
{
  printf("Bye\n");
  exit(0);
  return 1;
}

int cmd_help(tok_t arg[]);

int cmd_pwd(tok_t arg[])
{
  char *cwd = getcwd(NULL, 0);
  printf("%s\n", cwd);
  free(cwd);
  return 1;
}

int cmd_cd(tok_t arg[])
{
  if (chdir(arg[0]) != 0)
  {
    printf("%s\n", strerror(errno));
  }
  return 1;
}

/* Command Lookup table */
typedef int cmd_fun_t(tok_t args[]); /* cmd functions take token array and return int */
typedef struct fun_desc
{
  cmd_fun_t *fun;
  char *cmd;
  char *doc;
} fun_desc_t;

fun_desc_t cmd_table[] = {
    {cmd_help, "?", "show this help menu"},
    {cmd_quit, "quit", "quit the command shell"},
    {cmd_pwd, "pwd", "print working directory"},
    {cmd_cd, "cd", "change directory"},
};

int cmd_help(tok_t arg[])
{
  int i;
  for (i = 0; i < (sizeof(cmd_table) / sizeof(fun_desc_t)); i++)
  {
    printf("%s - %s\n", cmd_table[i].cmd, cmd_table[i].doc);
  }
  return 1;
}

void redirect_io(tok_t arg[])
{
  // FILE *out = fopen("./out.txt", "w");
  // dup2(fileno(out), fileno(stdout));
  // fclose(out);
  int old_file_no;
  FILE *new_file;
  int index;
  if (index = isDirectTok(arg, REDIRECT_IN))
  {
    old_file_no = fileno(stdin);
    new_file = open(arg[index + 1], "r");
  }
  else if (index = isDirectTok(arg, REDIRECT_OUT))
  {
    old_file_no = fileno(stdout);
    new_file = open(arg[index + 1], "w");
  }
  else
  {
    return;
  }
  dup2(fileno(new_file), old_file_no);
  // close(new_file);
  arg[index] = NULL;
}

int execute_and_wait(char *path, tok_t arg[])
{
  if (path == NULL)
  {
    printf("Command '%s' not found\n", arg[0]);
    return 1;
  }
  pid_t pid = fork();
  if (pid == 0)
  {
    // child
    redirect_io(arg);
    execv(arg[0], arg);
  }
  else
  {
    // parent
    wait(NULL);
  }
  return 1;
}

char *find_program(char *name)
{
  tok_t *paths = get_paths();
  for (int i = 0; i < MAXTOKS && paths[i] != NULL; i++)
  {
    char *path = combine_path(paths[i], name);
    if (file_exists(path))
    {
      return path;
    }
    free(path);
  }
  return NULL;
}

int run_program(tok_t arg[])
{
  char *path;
  if (strstr(arg[0], FILE_SEPARATOR))
  {
    path = file_exists(arg[0]) ? arg[0] : NULL;
  }
  else
  {
    path = find_program(arg[0]);
    if (path != NULL)
    {
      arg[0] = path;
    }
  }
  execute_and_wait(path, arg);
}

int lookup(char cmd[])
{
  int i;
  for (i = 0; i < (sizeof(cmd_table) / sizeof(fun_desc_t)); i++)
  {
    if (cmd && (strcmp(cmd_table[i].cmd, cmd) == 0))
      return i;
  }
  return -1;
}

void init_shell()
{
  /* Check if we are running interactively */
  shell_terminal = STDIN_FILENO;

  /** Note that we cannot take control of the terminal if the shell
      is not interactive */
  shell_is_interactive = isatty(shell_terminal);

  if (shell_is_interactive)
  {

    /* force into foreground */
    while (tcgetpgrp(shell_terminal) != (shell_pgid = getpgrp()))
      kill(-shell_pgid, SIGTTIN);

    shell_pgid = getpid();
    /* Put shell in its own process group */
    if (setpgid(shell_pgid, shell_pgid) < 0)
    {
      perror("Couldn't put the shell in its own process group");
      exit(1);
    }

    /* Take control of the terminal */
    tcsetpgrp(shell_terminal, shell_pgid);
    tcgetattr(shell_terminal, &shell_tmodes);
  }
  /** YOUR CODE HERE */
}

/**
 * Add a process to our process list
 */
void add_process(process *p)
{
  /** YOUR CODE HERE */
}

void remove_redirect(tok_t *t, char *redirect_symbol)
{
  int index = isDirectTok(t, redirect_symbol);
  if (index == 0)
  {
    return;
  }
  for (int i = index + 2; t[i] != NULL; i++)
  {
    t[i - 2] = t[i];
    t[i] = NULL;
  }
}

/**
 * Initialize a newly created process
 */
void init_process(process *p, tok_t *t)
{
  p->stdin = redirect_in(t);
  p->stdout = redirect_out(t);
  p->stderr = STDERR_FILENO;
  remove_redirect(t, REDIRECT_IN);
  remove_redirect(t, REDIRECT_OUT);
  p->argv = t;
  p->argc = countToks(t);
}

/**
 * Creates a process given the inputString from stdin
 */
process *create_process(char *inputString)
{
  /** YOUR CODE HERE */
  tok_t *t = getToks(inputString);
  process *p = malloc(sizeof(process));
  init_process(p, t);
  return NULL;
}

int shell(int argc, char *argv[])
{
  char *s = malloc(INPUT_STRING_SIZE + 1); /* user input string */
  tok_t *t;                                /* tokens parsed from input */
  int lineNum = 0;
  int fundex = -1;
  pid_t pid = getpid();   /* get current processes PID */
  pid_t ppid = getppid(); /* get parents PID */
  pid_t cpid, tcpid, cpgid;

  init_shell();

  // printf("%s running as PID %d under %d\n",argv[0],pid,ppid);

  lineNum = 0;
  // fprintf(stdout, "%d: ", lineNum);
  while ((s = freadln(stdin)))
  {
    t = getToks(s);        /* break the line into tokens */
    fundex = lookup(t[0]); /* Is first token a shell literal */
    if (fundex >= 0)
      cmd_table[fundex].fun(&t[1]);
    else
      run_program(t);
    // fprintf(stdout, "%d: ", lineNum);
  }
  return 0;
}
