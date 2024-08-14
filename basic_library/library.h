// library.h is the header file that defines the 'interface' for our library



// This is a declaration of a global variable that will be defined in library.c
// extern make available the global variable "global_variable" to other files
// that include this header file (or this library).
extern int global_variable;


int add(int x, int y);
int sub(int x, int y);