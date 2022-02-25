#include<stdio.h>

#include "shell.h"

char *find_program(char *name);

int main(int argc, char* argv[])
{
  return shell(argc, argv);
}
