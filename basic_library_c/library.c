

// library.c contains functions definitions of our library used by main.c

// we can include libraries, including the standard libraries, in our library
#include <stdio.h>



int global_varibale = 99999;


// when we use the static keyword in front of a global variable, it makes the
// variable 'statically allocated' which means it is only accessible from the
// file it is declared in.  This is useful for 'hiding' variables from other
// files that include this file.
static int  global_varibale = 99999;


int add(int x, int y)
{
  printf("add function\n");
  return x + y;
}

int sub(int x, int y)
{
  return x - y;
}