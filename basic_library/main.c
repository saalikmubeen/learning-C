/*******************************************************************************
*
* Program: Basic Library Example
*
* Description: An example of the basics of creating a library in C to split our
* program across multiple files.
*
*******************************************************************************/

#include <stdio.h>
#include "library.h"

int main()
{
  printf("%d + %d = %d\n", 4, 5, add(4,5));
  printf("%d - %d = %d\n", 10, 7, sub(10,7));


  // we are able to access the global variable from library.c in main.c
  // because we have declared it in library.h using the extern keyword
  printf("Global varibale from library.c: %d\n", global_variable);

  return 0;
}