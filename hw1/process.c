#include "process.h"
#include "shell.h"
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <termios.h>

void redirect_process_io(process *p)
{
  dup2(p->stdin, STDIN_FILENO);
  dup2(p->stdout, STDOUT_FILENO);
}

/**
 * Executes the process p.
 * If the shell is in interactive mode and the process is a foreground process,
 * then p should take control of the terminal.
 */
void launch_process(process *p)
{
  redirect_process_io(p);
  if (p->background)
  {
    // printf("background pid: %d\n", p->pid);
  }
  execv(p->argv[0], p->argv);
}

/* Put a process in the foreground. This function assumes that the shell
 * is in interactive mode. If the cont argument is true, send the process
 * group a SIGCONT signal to wake it up.
 */
void put_process_in_foreground(process *p, int cont)
{
  /** YOUR CODE HERE */
}

/* Put a process in the background. If the cont argument is true, send
 * the process group a SIGCONT signal to wake it up. */
void put_process_in_background(process *p, int cont)
{
  /** YOUR CODE HERE */
}
