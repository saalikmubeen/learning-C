// gcc -E -o out main.c       // generate preprocessed code
// gcc -S -o out main.c       // generate assembly code


#define NDEBUG // to disable assert() in release mode


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>  // memcpy, memset, strcpy, strcat, strlen, memccpy, memchr, memmove
#include <time.h>
#include <unistd.h>  // for sleep(sec), usleep(sec), usleep(microsec)
#include <stdbool.h> // for bool, true, false
#include <stdarg.h>  // for va_list, va_start, va_arg, va_end
#include <errno.h>   // for errno
#include <assert.h>  // for assert

// The limits.h library includes an constant INT_MAX defining the largest
// value an int can store... this may be different from one system to the next.
#include <limits.h>

// allows us to access errno from errno.h in this file, errno will store a
// number representing the error that has taken place
extern int errno;



// Another way of defining a constant
// During preprocessing, the preprocessor will replace all occurrences of
// ROWS with 3 in the code before it is compiled.
#define ROWS 3  // object like macro
#define COLS 4

#define TRUE 1
#define FALSE 0


#define greet_world printf("Hello world!");

#define LENGTH(a) (sizeof(a) / sizeof(a[0]))





// We can cause a compilation error to occur on purpose and provide a message with the
// #error directive (allows us to purposely cause the errorr during preprocessing phase)

// use like this:
// #error ERROR MESSAGE GOES HERE!

// If our program depends on some condition in order to function correctly
// and that condition is verifiable by preprocessor directives at compile-time,
// we can use #error to cause a compilation error when that condition is NOT
// met to prevent our program from compiling but functioning incorrectly.  For
// example here we cause a compilation error to occur if INT_MAX does not
// exceed a threshold value.
#if INT_MAX < 2147483648
  #error INT_MAX Not Large Enough!
#endif













// Anything that is 0 is considered false and anything that is not 0 is considered true

bool is_even(int number) {
  return number % 2 == 0;
}


enum Color {
  RED,
  GREEN,
  BLUE
};


enum Color my_color = RED;
enum Color another_color = BLUE;



enum Day {
  MONDAY = 100,
  TUESDAY,
  WEDNESDAY,
  THURSDAY,
  FRIDAY,
  SATURDAY,
  SUNDAY
};


typedef enum months {
  JANUARY,
  FEBRUARY,
  MARCH,
  APRIL,
  MAY,
  JUNE,
  JULY,
  AUGUST,
  SEPTEMBER,
  OCTOBER,
  NOVEMBER,
  DECEMBER
} Month;

Month current_month = AUGUST;


struct Point {
  int x;
  int y;
};


typedef struct Person {
  char *name;
  int age;
  struct person *child;
} Person;

typedef struct {
  char first_name[20];
  char last_name[20];
  int age;
} Student;



// Student is pass by value
void print_student(Student student) {
  printf("First name: %s\n", student.first_name);
  printf("Last name: %s\n", student.last_name);
  printf("Age: %i\n", student.age);
};


void increment_student_age(Student *student) {
  student->age++;  // (*student).age += 1;
};



void print_size(int array[])
{
  int length = sizeof(array) / sizeof(array[0]);

  // Note that even though when an array is passed to a function what is really
  // passed is a pointer to the array, we can still use array notation to access
  // the array elements.
  array[0] = 5;

  printf("LENGTH: %d\n", length);
}





// cleanup function that is scheduled to run at exit will record the end time,
// find the difference between start and end time, and output that difference

time_t start, end;

void cleanup()
{
  time(&end);
  double diff = difftime(end, start);
  printf("Execution time: %f\n", diff);
}



// char* argv[] is equivalent to char **argv
//
int main(int argc, char* argv[]) {  // char* argv[] -> argv is array of strings

    // record the start time of the program's execution
    time(&start);


    printf("Hello, World!");
    greet_world // macro expansion, replaced with printf("Hello world!") at compile time

    char *secret = getenv("SECRET"); // get the value of the environment variable SECRET

    system("ls -l"); // execute a command in the shell

    if (system("ls -l") != 0) {
      printf("Error executing command\n");
    }


  // set the cleanup function to run when the program exits
  atexit(cleanup);



  // We call the abort function with no arguments, notice how in comparison
  // to exit it does not run the cleanup function.  Depending on the C
  // implementation abort may also not close files, delete temporary files,
  // or flush the stream buffer.  It will however do a core dump which may
  // be useful for debugging our program. It's a hard/immediate exit of the program.
  // Dumping the core means that the current state of the program is saved to a file
  // on the disk so that it can be analyzed later for debugging purposes.
  abort();


  assert(1 == 1); // if the condition is false, the program will abort




  // declares a register variable, the variable will be kept in a CPU register
  // instead of memory, unless the compiler ignores it!
  register int x = 5;

  // We can't get the memory address of a register variable with &x because
  // the variable is not in memory!  This is a big difference between register
  // variables and other variables. The variable is not stored in memory where
  // we have memory addresses to access it. So we can't do something like:
  // int *y = &x;

  // we can print out register variables and perform calculations with them
  // the same as regular variables
  printf("x: %d\n", x);



  // sizeof() returns the storage size of a type or an
  // expression in char-sized units
  //
  // i.e. sizeof(char) will always be 1
  //
  // Technically speaking the size of a char in bits is
  // specified by a constant CHAR_BITS and could be
  // something other than a byte.
  //
  // But practically speaking, size of a char is virtually
  // always '1 byte' (8 bits).
  //
  // So we can effectively say that sizeof() returns the
  // number of bytes to store the type or expression.
  size_t size = sizeof(int); // 4 bytes on most systems
  printf("Size of int: %zu\n", size); // %zu is the format specifier for size_t (unsigned integer)

  double numbers[] = {1.1, 2.2, 3.3, 4.4, 5.5};
  printf("Size of double: %zu\n", sizeof numbers); // 40 bytes on most systems

  size_t length = sizeof(numbers) / sizeof(numbers[0]);
  printf("Length of array: %zu\n", length);



  // Dynamically allocate an array of 100 ints using malloc(), dynamic_array
  // will be a pointer to this block of memory on the heap.
  int *dynamic_array = malloc(sizeof(int) * 100);

  // sizeof(dynamic_array) will NOT give us the size of the block of memory
  // on the heap, it will actually give us the size of the pointer, i.e. the
  // size of a pointer to an int.
  printf("sizeof(dynamic_array): %d\n", sizeof(dynamic_array)); // 8 bytes on most systems

  // So if we try to use the LENGTH function-like macro to get the size of
  // this dynamically allocated array, it's not going to work, because it
  // will try to use sizeof(dynamically_array) to get the size in bytes of the
  // array (and this will not work, instead it will result in the size in
  // bytes of the pointer).
  int dynamic_length = LENGTH(dynamic_array);

  // Output the incorrect dynamic_length value
  printf("dynamic_length: %d\n", dynamic_length);

  // Free the dynamically allocated array to prevent a memory leak
  free(dynamic_array);



  // Declare an array of length 9 and initialize it with int values
  int array[] = {1,2,3,4,5,6,7,8,9};


  // If we call print_size and pass it array, what will actually be passed
  // to the array is a pointer to the array, i.e. the memory address of
  // the first element in the array.
  print_size(array);


  for (int i = 0; i < 10; i++) {
    printf("Hello, World!\n");
  }

  struct Point p1;

  p1.x = 10;
  p1.y = 20;

  struct Point p2 = {10, 20};
  struct Point p3 = {.x = 10, .y = 20};

  Person john = {"John", 30};

  Student student;
  strcpy(student.first_name, "John");
  strcpy(student.last_name, "Doe");
  student.age = 30;


Student *student1;
student1 = calloc(1, sizeof(Student));

(*student1).age = 30; // student1->age = 30;


Student s1 = { "John", "Doe", 30 };
Student s2 = { "Jane", "Doe", 25 };


// copy by value i.e all the fields of s2 are copied to s1
// so if we change s2 it will not affect s1
s1 = s2;

s2.age = 1000;
// s2.first_name = "Changed"; // this will not work because first_name is an array (pointer)
strcpy(s2.first_name, "Changed");

printf("First name: %s, Last name: %s, Age: %i\n", s1.first_name, s1.last_name, s1.age);
printf("First name: %s, Last name: %s, Age: %i\n", s2.first_name, s2.last_name, s2.age);


  int numbers[] = {1, 2, 3, 4, 5};
  int numbers2[4];


  char *name = "John";
  printf("Hello, %s!\n", name);

  // fullname
  char fullname[] = "John Doe";


  char *greet = "Hello, ";
  printf("%p\n", greet); // prints the memory address of the first character
  printf("%c\n", *greet);  // *greet == greet[0]
  printf("%c\n", *(greet + 1)); // *(greet + 1) == greet[1]
  printf("%c\n", *(greet + 2)); // *(greet + 2) == greet[2]
  printf("%c\n", *(greet + 3));
  printf("%c\n", *(greet + 4));


  char firstName[5];

  firstName[0] = 'J';
  firstName[1] = 'o';
  firstName[2] = 'h';
  firstName[3] = 'n';
  firstName[4] = '\0'; // null terminator



  // Though '\0' looks like two characters, this is only the case in the
  // text editor used to write the program.  It is actually one character and
  // '\0' is used to represent the character.  When we output the integer
  // that represents the null terminator '\0' we will find it is 0.  So the
  // null terminator is equivalent to 0, i.e. the byte with all bits set to 0.
  printf("\\0 = %d\n\n", '\0');



  // We can declare another char array and initialize it with another string
  char string2[] = "Portfolio";

  // We can output the string
  printf("string2: %s\n", string2);

  // If we set the index 4 in the char array to 0, this is equivalent to
  // inserting a null terminator at index 4, and so the string will now only
  // by "Port", the first 4 characters in the char array from indexes 0...3.
  string2[4] = 0;

  // Output the string again to verify is it now only "Port"
  printf("string2: %s\n\n", string2);

  // The integer 0 is "false" in C and all non-zero integers are "true" in C.
  // So we can use the null terminator in if-statement and loop conditions to
  // detect if a null terminator character is present or not.  For example if
  // we have !character as a condition and the character IS the null
  // terminator, the condition will be true (otherwise it will be false).  We
  // utilize this below to output that the character at the index 4 is the
  // null terminator...
  if (!string2[4])
  {
    printf("Character is the null terminator.\n");
  }
  else
  {
    printf("Character is NOT the null terminator.\n");
  }




  // Declare a char array and initialize it with a string
  char string3[] = "Courses";

  // We can loop through the characters in the string using a char pointer
  // variable.  We set the char pointer variable p to point to the string, this
  // will have p point to the first character in the string.
  char *p = string3;

  printf("\n");

  // We can de-reference p and access the character it is pointing to with *p,
  // so by having *p as a loop condition, the loop will continue so long as p
  // is NOT pointing to the null terminator (as all other characters are
  // considered true).  And the loop will STOP once p reaches the end of the
  // string and IS pointing to the null terminator (which is considered false).
  while (*p)
  {
    // De-reference p and output the character that p is currently pointing to
    printf("%c\n", *p);

    // Use pointer arithmetic to increment p and have it point to the next
    // character in the string, in this way we'll "loop through" each character
    // in the string.
    p++;
  }
  printf("\n");



  // statically obtain or allocate memory
  int my_number = 10;
  // dynamically allocate memory
  int *my_number_ptr = malloc(sizeof(int));


  // calloc() initializes the memory to zero
  // The difference between malloc() and calloc() is that malloc() does not initialize
  // the memory allocated, while calloc() initializes the allocated memory to zero.
  // In simple words, calloc() is used to allocate memory and set all bytes to zero.
  // So callco ensures that the memory is clean and ready to use.
  // int *my_number_ptr = calloc(1, sizeof(int));


  // statically allocated memory for an array of 5 floats
  // array of floats on the stack
  float array[5];
  // array of floats on the heap
  float *array_ptr = malloc(5 * sizeof(float));


  char *string1 = "Hello";
  char *copy = malloc(6); // allocate memory for 6 bytes

  if (copy == NULL) {
    printf("Memory allocation failed\n");
    return 1;
  }

  for (int i = 0; i < 6; i++) {
    copy[i] = string1[i];
  }


    free(copy); // free the memory



  int *special_numbers;
  int size = 5;
  special_numbers = malloc(size * sizeof(int));

  if (special_numbers == NULL) {
    return 1;
  }

  int input = 0, i = 0;

  do {

    if (i == size) {
      size += 2;
       special_numbers = realloc(special_numbers, size  * sizeof(int));


      printf("Memory reallocated to %i integers\n", size);
    }


    printf("Enter a number or -1 to quit: ");
    scanf("%i", &input);
    fgets(&input, 4, stdin);
    if (input == -1) {
      special_numbers[i] = input;
      i++;
    }

  } while (input != -1);

  int num_elements = i;
  int total = 0;
  for (int j = 0; j < num_elements; j++) {
    total += special_numbers[j];
  }
  printf("The average is %.2f\n", total / (float) num_elements);


  // All free does is that it makes availabe the memory or space again that was
  // allocated by malloc() or calloc() so that it can be used again by another
  // malloc() or calloc() call.
  // But it does not delete the data that was stored in the memory.
  free(special_numbers);



  char buffer[10];

  // We use memset by passing in a memory address where we want to begin setting
  // the value (the array buffer decays to a pointer / memory address when we
  // use it like this), the value to set ('c'), and the number of bytes to set
  // that value (sizeof(char) * 5, which will be 5 chars of memory, the first 5
  // chars of the array.
  memset(buffer, 'c', sizeof(char) * 5);

  // equivalent to:
  // for (int i = 0; i < 5; i++) {
  //   buffer[i] = 'c';
  // }

  // We use pointer arithmetic here to set the NEXT 5 chars in the buffer to
  // 'd', when we add '5' to 'buffer' the expression results in the memory
  // address shifted over 5 ints from buffer (since buffer stores the memory
  // address of the first element in the array, this results in buffer+5
  // pointing to the 5th element of the array).
  memset(buffer+5, 'd', sizeof(char) * 5);

  // print out the contents of buffer
  for (int i = 0; i < 10; i++)
    printf("%c", buffer[i]); // cccccdddddd
  printf("\n");



  char s[] = "abcdefghijklmnopqrstuvwxyz";
  char d[27];

  // copy 27 chars worth of data from s to do... the 26 letters + the null
  // terminator
  memcpy(d, s, sizeof(char) * 27);
  printf("d: %s\n", d);

  // copy 9 chars worth of data from the memory address of s shifted over by
  // 8 ints worth of memory addresses (using pointer arithmetic with s+8)
  char e[10];
  memcpy(e, s+8, sizeof(char) * 9);

  // append the null terminator manually to the end of e since this time we
  // did not copy it from s
  e[9] = '\0';
  printf("e: %s\n", e);





  // create a test source array (i.e. a block of memory of 10 ints from 1...10)
  int source[10] = {1,2,3,4,5,6,7,8,9,10};

  // create a test destination array (i.e. a block of memory 10 ints in size)
  int destination[10];

  // Use memmove() to copy 10 ints from the source to the destination.  When
  // we pass 'destination' and 'source' like this, they 'decay to a pointer',
  // and what we're REALLY passing is the memory addresses of destination
  // and source (the memory address of the first element in the arrays).  For
  // the 3rd argument we supply the number of bytes to copy from the source
  // to the destination... sizeof(int) gives us the size in bytes of an int
  // value, and we multiply this by 10 to copy all 10 ints from the source to
  // the destination.
  memmove(destination, source, sizeof(int) * 10);

  // Note that memcpy() will ALSO copy a block of memory from source to
  // destination.  We could comment out the above function call, and uncomment
  // this function call, and we'll get the same result!
  // memcpy(destination, source, sizeof(int) * 10);

  // The behaviour of memcpy() is undefined when the source and destination
  // blocks of memory overlap, which means it may or may not work.  This is
  // why we would use memmove() instead of memcpy(), as memmove() will
  // safely perform the copy even if the blocks of memory overlap.  If we're
  // not sure if the blocks of memory will overlap or not, we should use
  // memmove() as it will be safe.  If we're sure they will not overlap we
  // should use memcpy() as its performance may be higher.  In the below
  // function call we attempt to copy 8 ints from source (i.e. the
  // beginning of the array) to 'source+2', i.e. the source array from
  // index 2 (the 3rd element) onwards.
  //
  // memmove(source+2, source, sizeof(int) * 8);
  // source now contains: 1, 2, 1, 2, 3, 4, 5, 6, 7, 8

  // Output the destination array to verify the copy was successful
  for (int i = 0; i < 10; i++)
    printf("destination[%d]=%d\n", i, destination[i]);

  // Note that the data is COPIED from source to destination, the data in the
  // source is not somehow removed (the word 'move' might give this impression).
  // We can print out the source array to verify this.
  for (int i = 0; i < 10; i++)
    printf("source[%d]=%d\n", i, source[i]);





  // we will copy from the source to destination
  char source[] = "john.smith@microsoft.com";
  char destination[20];

  // memccpy will copy chars from the source into the destination until either
  // 20 chars have been copied OR until the '@' char is encountered in the
  // source string, whichever occurs first.  It will return a pointer to the
  // next available position in the destination char array after the copy has
  // occurred if the '@' character *was* encountered.  If the '@' character
  // is NOT encountered and all 20 characters are copied from the source to
  // destination, memccopy will return NULL instead.  When the copy does occur
  // up until the '@' character, the copy will INCLUDE the '@' character too!
  char *pos = memccpy(destination, source, '@', 20);

  // char *pos = memccpy(destination, source, 'q', 20); // pos will be NULL

  // memccpy does not insert a null terminator after performing the copy into
  // destination, so if we want to treat destination as a string, we need to
  // insert it ourselves... we do so by inserting the null terminator at
  // pos[0], which is the next character in the destination character array
  // after the characters copied over from the source
  pos[0] = '\0';

  // output each char in the destination char array individually so we can see
  // what it looks like
  for (int i = 0; i < 20; i++)
    printf("destination[%d] = %c\n", i, destination[i]);

  // because we have inserted the null terminator, we can also print out
  // destination as a string
  printf("destination: %s\n", destination);






  // test char array
  char data[] = {'q', 'r', 's', 't', 'p', 'a', 'x'};

  // memchr will search 7 chars deep into the data array for the char 't'
  // and return a pointer to the first occurrence of the character
  char *pos = memchr(data, 't', 7);

  // if memchr can't find the character it will return NULL, so for example
  // if we searched for 'z' in the above char array we would expect a
  // return value of NULL
  if (pos == NULL)
  {
    printf("Character not found!\n");
  }
  // we should get 't' and 'p' as output if use array indexing with the pointer
  else
  {
    printf("pos[0] = %c\n", pos[0]);
    printf("pos[1] = %c\n", pos[1]);
  }

  // an obvious use case for the function is working with strings, for example:
  char str[] = "john.smith@microsoft.com";

  // let's find the first occurrence of the '@' char in the email address above,
  // giving us the domain of the e-mail
  char *domain = memchr(str, '@', strlen(str));

  // we can increment the pointer by 1 to skip over the @ character so that
  // domain will point to the string "microsoft.com" instead of "@microsoft.com"
  domain++;

  // we will get "microsoft.com" as output
  printf("domain: %s\n", domain);



  // 2D Arrays

  const int rows = 2;
  const int cols = 4;

  // int matrix[rows][cols] = {0}; // initialize all elements to 0
  int matrix[rows][cols] = {
    {1, 2, 3, 4},
    {5, 6, 7, 8}
  }; // or int matrix[2][4] = { 1, 2, 3, 4, 5, 6, 7, 8 };


  for (int i = 0; i < rows; i++) {
    for (int j = 0; j < cols; j++) {
      printf("Enter a number for row %i, column %i: ", i, j);
      scanf("%i", &matrix[i][j]);
    }
    printf("\n");
  }



  // char buffer[4];
  // to allow user to enter strings with spaces -> fgets(buffer, 4, stdin);
  // alternative to -> scanf("%s", buffer);



  // random number generator
  srand(time(NULL)); // seed the random number generator
  int random_number = rand() % 100; // generate a random number between 0 and 99
  printf("Max random number: %i\n", RAND_MAX);



  // goto statement
  printf("Statement 1\n");

  printf("Statement 2\n");

  goto end;

  printf("Statement 3\n");

  end:
  printf("Statement 4\n");


  for (int i = 0; i < 10; i++) {
    for (int j = 0; j < 10; j++) {
      for (int k = 0; k < 10; k++) {
        if (k == 5) {
          // break; // break only breaks the innermost loop
          goto end_loop; // end the whole 3 loops.
        }
        printf("i: %i, j: %i, k: %i\n", i, j, k);
      }
    }
  }

  end_loop:
  printf("Loop ended\n");



  // Multiline String
  char multiline[] = "This is a multiline\n"
                    "string\n"
                    "in C\n";

  return 0;  // exit(0); exit(EXIT_SUCCESS); exit(1); exit(EXIT_FAILURE);
}


int dynamic_lists() {
    int *list = malloc(3 * sizeof(int));

    if (list == NULL) {
      return 1;
    }

    list[0] = 1;
    list[1] = 2;
    list[2] = 3;

    int *tmp = malloc(4 * sizeof(int));

    if (tmp == NULL) {
      free(list);
      return 1;
    }

    for (int i = 0; i < 3; i++) {
      tmp[i] = list[i];
    }

    tmp[3] = 4;

    // free the memory allocated for the list as we no longer need it
    // because we have copied all the values to the new array tmp
    free(list);

    list = tmp;

    for (int i = 0; i < 4; i++) {
      printf("%i \t", list[i]);
    }
    printf("\n");


    free(list);

    return 0;
}


// int matrix[][COLS] also will work as compiler knows the number of columns and thus
// can calculate the size of the array and the number of rows
void print_matrix(int matrix[ROWS][COLS]) {
  for (int i = 0; i < ROWS; i++) {
    for (int j = 0; j < COLS; j++) {
      printf("%i\t", matrix[i][j]);
    }
    printf("\n");
  }
}

typedef uint8_t byte;
typedef char* string;

// typedef <old_name> <new_name>;

typedef struct
{
  char* name;
  int age;
}
person;


float average(int length, int numbers[])
  {
    int sum = 0;
    for (int i = 0, l = length; i < l; i++) {
        sum += numbers[i];
    }
    return sum / (float) length;
  }



// char name[] = "John\0"
  int string_length(char string[])
  {
    int length = 0;
    while (string[length] != '\0') {
        length++;
    }
    return length;
  }


char* upper_case(char string[])
  {
    int length = string_length(string);
    char result[length];
    for (int i = 0; i < length; i++) {
        char c = string[i];
        if (c >= 'a' && c <= 'z') {
            // c -= 32;
            c -= ('a' - 'A');
        }
        result[i] = c;
    }
    return result;
  }


  isPdf(char *fileName)
  {

    FILE *file = fopen(fileName, "r");

    if (file == NULL) {
        return 0;
    }

    int header[4];
    int blocks_rerad = fread(header, 1, 4, file);

    for (int i = 0; i < 4; i++) {
        printf("%i\n", header[i]);
    };

    printf("Blocks read: %i\n", blocks_rerad);

    fclose(file);

    // 37, 80, 68, 70
    return header[0] == 0x25 && header[1] == 0x50 && header[2] == 0x44 && header[3] == 0x46;
  }













  // Variable arguments function

  // math_max(4, 100, 200, 300, 400);
  int math_max(int num_args, ...)
  {
    va_list args; // va_list is a type to hold information about variable arguments
    va_start(args, num_args);

    int max = va_arg(args, int);

    for (int i = 1; i < num_args; i++) {
        int current = va_arg(args, int);
        if (current > max) {
            max = current;
        }
    }

    va_end(args);

    return max;
  }


// the first argument is the number of int arguments from which to find the
// max int value
int max_fn(int num_args, ...)
{
  // va_list variables contain the data necessary for the relevant functions to
  // extract the arguments
  va_list args;

  // we call va_start with our va_list variable args, as well as the last
  // parameter variable preceeding the beginning of the list of arguments of
  // variable length
  va_start(args, num_args);

  // keep track of the current max number as we extract each argument
  int max = 0;
  for (int i = 0; i < num_args; i++)
  {
    // we call va_arg with our va_list variable args as the first argument, and
    // the second argument is the type of the argument we are extracting next,
    // in this case it will always be an int
    int x = va_arg(args, int);

    // if its the first number, set it as our max, update the max whenever a
    // new max is found throughout the remainder of the list of arguments
    if (i == 0) max = x;
    else if (x > max) max = x;
  }

  // va_end ends our access to the list of arguments, again using our va_list
  // variable args
  va_end(args);

  return max;
}

// Prints out an arbitrary sequence of int and double values much like printf
// is capable of doing, by looking at a string of place holders where the 'd'
// char represents an int and the 'f' char represents a double value.  The
// sequence of placeholder characters identifies the number of arguments and
// sequence of argument types to be expected... so print("ddfd",1,2,2.4,3)
// means print two ints, a double, and an int, and the arguments are provided
// in this sequence following the placeholder string.
void print(char *placeholders, ... )
{
  // the number of arguments to be expected is equal to the number of
  // placeholder string chars as returns by strlen()
  int num_args = strlen(placeholders);

  va_list args;
  va_start(args, placeholders);

  for (int i = 0; i < num_args; i++)
  {
    // if the next char in the placeholder string is a 'd', extract the next
    // argument as an int and print it out
    if (placeholders[i] == 'd')
    {
      int x = va_arg(args, int);
      printf("%d\n", x);
    }
    // if the next char in the placeholder string is a 'f', extract the next
    // argument as a double and print it out
    else if (placeholders[i] == 'f')
    {
      double x = va_arg(args, double);
      printf("%f\n", x);
    }
  }

  va_end(args);
}







// error_handling() will attempt to open a file that doesn't exist, and then
// print out the error message associated with the error that took place
int error_handling(void)
{
  FILE *fh;

  // before any error takes place, errno will be set to zero by default
  printf("Errno before: %d\n", errno); // 0

  // try to open a file that doesn't exist
  fh = fopen("dne.txt", "r");

  if (fh == NULL)
  {
    // now errno will be set to '2', a number representing the error, we use
    // fprintf to print to stderr which by default may be the terminal but
    // could instead perhaps be a file
    fprintf(stderr, "Errno after: %d\n", errno);

    // perror will append a ":"" and space and then output the error message
    // associated with errno
    perror("Error msg");

    // if we call perror with a blank string it will simply output the
    // error message
    //  perror("");

    // Instead of perror, we can use also pass strerror the errno in order to get the error
    // message string
    // stderr by default is the terminal, but could be a file also. So the best practice is to
    // use fprintf(stderr, ...) to print to the terminal or file as needed with the error message
    // instead of just using printf() which will print to stdout by default.
    fprintf(stderr,  "Error msg: %s\n", strerror(errno) );

  // we know the file doesn't exist, but just in case it does let's be proper
  // and close the file handle
  } else fclose(fh);

  return 0;
}


























// DANGLING POINTERS

// A function-like macro that makes it easier to call safe_free_mem(), we
// can just supply the pointer variable as we do with free() this way.
#define safe_free(ptr) safe_free_mem((void **) &(ptr))

// Increments the value pointed to by ptr by 1.  Returns 0 if successful, -1
// otherwise in the case ptr is NULL.
int do_work(int *ptr)
{
  // if ptr is NULL it points to nothing so we cannot de-reference it and the
  // function returns an error code -1, "defensively" handling a NULL pointer
  if (ptr == NULL) return -1;

  // Increments the value pointed to by ptr
  *ptr = *ptr + 1;

  // Returns 0 if successful
  return 0;
}

// A safe version of the free function that accesses the pointer that pointer
// to a pointer parameter ptr points to and 1) frees the block of memory the
// pointer points to, 2) sets the pointer to NULL to prevent and dangling
// pointer.
void safe_free_mem(void **ptr)
{
  // It does not make sense for ptr to be set to NULL and it is undefined
  // behaviour to try to de-reference NULL so we use an assert() to
  // ensure ptr is not set to NULL.
  assert(ptr);

  // Free the memory pointed to by the pointer that ptr points to
  free(*ptr);

  // Set the pointer that ptr points to to NULL so it points to nothing
  *ptr = NULL;
}

int dangling_pointers(void)
{
  // Allocate a block of memory to store a single int value and have ptr
  // store the memory address of this block of memory (i.e. point to it)
  int *ptr = malloc(sizeof(int));

  // We can set the value of this space in memory to 4 by de-referencing
  // ptr.
  *ptr = 5;

  // Output the memory address ptr stores, and the value stored at that
  // memory adress.
  printf("  ptr before: %p\n", ptr); // %p will print the memory address in hexadecimal
  printf(" *ptr before: %d\n", *ptr);

  printf("  ptr before: %zu\n", ptr); // zu will print the memory address in decimal


  // We need to the free block of dynamically allocated memory when we are
  // done working wit hit.  If we JUST call free() and pass it the pointer
  // the block of memory will be free...
  //
  // free(ptr);
  //
  // BUT ptr will still store the memory address for this block of memory.
  // The free() function does not set it to NULL so that it points to
  // nothing.  As a result, our program can still access this memory and
  // modify it.  We call this situation a "dangling pointer":
  // https://en.wikipedia.org/wiki/Dangling_pointer
  //
  // *ptr = 99;
  // printf("   ptr after: %p\n", ptr);
  // printf("  *ptr after: %d\n", *ptr);
  //
  // This can cause serious bugs that can be difficult to trace and can be a
  // security issue as well!
  //


  // We could instead set ptr to NULL after calling free()...
  //
  // free(ptr);
  // ptr = NULL;
  //
  // This is considered a best practice to eliminate the dangling pointer,
  // our program can no longer use ptr to access the block of memory.


  // We could also create a "safe" version of the free function as we have done
  // so above that sets the pointer variable to NULL for us.
  //
  // safe_free_mem((void **) &ptr);

  // If we want to be able to call our safe free function by just passing the
  // pointer varible as we do with free(), we could use a function-like macro
  // safe_free defined above to achieve this effect:
  //
  safe_free(ptr);

  // One advantage of setting pointers to NULL after free() is that often
  // functions will defensively check if pointer parameters are NULL before
  // using them.  Here are do_work() function will recognize that ptr has
  // been set to NULL and return an error status allowing our program to
  // handle the issue gracefully.
  if (do_work(ptr) != 0)
  {
    printf("Error doing work.\n");
    return -1;
  }

  return 0;
}





















// Returning Arrays from functions:

// What we can do is return a pointer to an array... i.e. a pointer to an int
// that stores the memory address of the first element in the array.  We
// can then use that pointer to effectively access the array using array
// index syntax (see the main function).  BUT the pointer returned cannot
// be a pointer to an array that is a local variable of the function, as in
// the below example.  The local_array[] array has the lifetime of each
// function call to set_array(), once the function returns, the variable
// no longer exists.  So when we try to use the pointer that is returned
// from this function call in the main function, it's pointing to memory that
// we don't have access to anymore!
//
/*
int *set_array(int value)
{
  int local_array[5];

  for (int i = 0; i < 5; i++)
    local_array[i] = value;

  return local_array;
}
*/

// One common approach to "return" an array from a function is to actually
// accept a pointer to an array as a parameter.  The function can then use
// array index notation like array[i] to manipulate the values of the array,
// effectively "returning" a new array of values.  The array itself can be
// declared in the main function (or another function) and when the pointer
// to the array is passed to the function as an argument, and the function
// then returns, the array will still exist!  This and the other set_array()
// functions will set all array elements to a provided value.
void set_array1(int value, int *array)
{
  for (int i = 0; i < 5; i++)
    array[i] = value;
}

// We could also use dynamic memory allocation to dynamically allocate space
// for the array on the heap.  Here we use malloc to allocate space for an
// array of 5 ints, we set the values, and then we return a pointer to this
// block of memory which we can then treat as an array.  Dynamically allocated
// memory is in a different part of memory called the heap that we need to
// manage ourselves by later calling free to the free dynamically allocated
// memory (in order to make the memory available to our program again for
// later use).  Local varibles are kept in a different part of memory called
// the stack, and they will cease to exist when the function returns.
int *set_array2(int value)
{
  int *array = malloc(sizeof(int) * 5);

  for (int i = 0; i < 5; i++)
    array[i] = value;

  return array;
}

// We could also use the static keyword to create a static array of ints.  A
// static variable has the lifetime of the entire program's execution, instead
// of each function call to set_array3().  This means even after the function
// returns, the array will still exist in memory, and if we return a pointer to
// this array to main as we do here, the main function can use the pointer
// to access the array!
int *set_array3(int value)
{
  static int array[5];

  for (int i = 0; i < 5; i++)
    array[i] = value;

  return array;
}

// Returning constant strings from functions:

// We can return a pointer to a string literal.  A string literal like "string"
// below is immutable and cannot be modified, so we make the return type of the
// function a const char * to make this clear and help enforce this.  This
// technique would be OK if we're sure we would never want to modify the
// string being returned.
const char *method1()
{
  return "string";
}











// Static functions

// Making print() a static function will limit its visiblity (or 'scope') to
// this file (or more accurately, "translation unit").  This means that the
// rest of the program (aka "the outside world") CAN'T use the print function.
// This is a form of information hiding, where changes to print() will now
// only require changes to this library because this is the only place it
// can be used.

// Outputs the value some number of times
static void print(double value, int times)
{
  for (int i = 0; i < times; i++)
    printf("%.3f\n", value);
}









// Different ways of using pointers

int different_ways_of_using_pointer()
{
  // Declare an int variable num and assign it the value 5
  int num = 5;

  // ALL of these approaches will declare a variable of type pointer to an int!
  //
  // int *p1;
  // int* p2;
  // int * p3;
  //
  // The second approach might seem the "best" approach as it clearly expresses
  // the type "pointer to an int" with int* all together.
  //


  // We might think that the below statement will declare two pointer to int
  // variables p1 and p2.  But it doesn't.  Only p1 will be a pointer to an
  // int, p2 will be an int variable!
  //
  int* p1, p2;

  // We could declare the variables like this to declare two pointer to an int
  // variables.  That's why using the "int *p1;" approach might make more sense,
  // though opinions may differ.
  //
  // int *p1, *p2;

  // Assign to p1 and p2 the memory address of num, i.e. have them point to num
  p1 = &num;
  p2 = &num;

  // Output the size of a pointer to an int and the size of an int, in bytes.
  // On the machine this code was written on the size of a pointer to an int
  // was 8 bytes and the size of an int was 4 bytes.
  printf("sizeof(int*): %d\n", sizeof(int *));
  printf("sizeof(int): %d\n", sizeof(int));

  // If we output the size in bytes of p1 and p2, we'll find that p1 is the
  // same size as a pointer to an int because it IS a pointer to an int.  And
  // p2 will match the size of an int because it IS an int.
  printf("sizeof(p1): %d\n", sizeof(p1));
  printf("sizeof(p2): %d\n", sizeof(p2));

  // We can de-reference p1 to output the value 5, but we'll get an error if we
  // try to de-reference p2 because it's not a pointer to an int... it's an int!
  printf("*p1: %d\n", *p1);
  // printf("*p2: %d\n", *p2); // will cause an error: operand of '*' must be a pointer but has type "int"

  return 0;
}











// Storing functions in a variable

// A simple function with a void return type and no parameters
void function()
{
  printf("Function!\n");
}

int main()
{
  // Assign to the pointer to a function variable fnc_ptr1 a pointer to
  // 'function' above, without using the & operator
  void (*fnc_ptr1)() = function;

  // We can call the function using the pointer to the function successfully
  (*fnc_ptr1)();
  fnc_ptr1();

  // Assign to the pointer to a function variable fnc_ptr2 a pointer to
  // 'function' above, this time using the & operator
  void (*fnc_ptr2)() = &function;

  // We can again call the function using the pointer to the function
  (*fnc_ptr2)();
  fnc_ptr2();

  // Both fnc_ptr1 and fnc_ptr2 will store the exact same memory address as
  // function and &function both give us the same pointer to the function!
  printf("%p\n", fnc_ptr1);
  printf("%p\n", fnc_ptr2);

  return 0;
}













// array vs &array Pointers Difference Explained

int array_vs_arrayPointers()
{
  // declare an array of 5 ints
  int array[5];

  // We can access array elements using the index operator, but if we just use
  // the array name 'array' instead the array will "decay to a pointer"
  array[2] = 5;

  // When we just use the array name 'array' the array 'decays to a pointer' and
  // what we get is a pointer to the first element in the array... i.e. the first
  // int in the array.  On the machine/compiler this program was first run on
  // int values take up 4 bytes.  If we use the pointer arithmetic '+ 1' with
  // the resulting pointer, we'll get 4 bytes added to the memory address
  // because the pointer we get when array decays to pointer points to something
  // of that size.  We output the memory addresses below using zu instead of p
  // so we can see the effect more clearly (p will give us a hexidecimal number
  // as output).  We'll find that array results in a certain memory address and
  // array + 1 results in a memory address 4 bytes higher.
  printf("     array: %zu\n", array);       // 6171045364
  printf(" array + 1: %zu\n\n", array + 1); // 6171045368

  // &array will give us a pointer to the ENTIRE ARRAY of 5 ints.  The memory
  // address will be the same as when array decays to pointer in the above
  // example because the array starts at the same memory address as the first
  // element. But because &array is a pointer to the entire array, when we use
  // pointer arithmetic with '+ 1' we will get the size of what &array is
  // pointing to added to the memory address.... and &array is pointing to an
  // array of 5 ints so we have 5 x 4 bytes per int = 20 bytes.  So the memory
  // address resulting from &array + 1 should be 20 bytes more than the memory
  // address of &array!  This is the difference between the two pointers!
  printf("    &array: %zu\n", &array);        // 6171045364
  printf("&array + 1: %zu\n\n", &array + 1);  // 6171045384


  // This effect will show up when working with 2D arrays, here we declare a
  // 2D array of 3 rows and 5 columns.
  int matrix[3][5] =
  {
    {0,1,2,3,4},
    {5,6,7,8,9},
    {10,11,12,13,14}
  };

  // matrix[1] will give us a pointer to the first element of the 2nd row of
  // the 2D array.  And again because this is an int, when we use pointer
  // arithmetic with matrix[1] + 1 we'll get a pointer to the 2nd element of
  // the 2nd row of the 2D array which will be 4 bytes more than matrix[1].
  // If we de-reference the pointer we'll then output 6.
  printf("        matrix[1]: %zu\n", matrix[1]);            // 6171045324
  printf("    matrix[1] + 1: %zu\n", matrix[1] + 1);        // 6171045328
  printf(" *(matrix[1] + 1): %zu\n\n", *(matrix[1] + 1));   // 6

  // With &matrix[1] we'll get a pointer to the entire 2nd row of the 2d array
  // which is 5 ints in length as we have 5 columns in the 2d array.  So when
  // we output &matrix[1] + 1 we expect it to be 20 bytes more than &matrix[1].
  // The + 1 will result in a pointer to the 3rd row of the 2d array.
  // When we de-reference &matrix[1] + 1 with * we might expect it to output
  // 10, as &matrix[1] + 1 does result in the memory address that element is
  // stored at as it is the first element of the 3rd row.  But &matrix[1] + 1
  // is a pointer to the entire 3rd row of the 2d array (an array of 5 elements)
  // and so when we apply the * operator we actually get back a pointer to the
  // first element of that array... the element 10.  So we actually ouput a
  // memory address - the same memory address - as a result of that de-reference
  // operator.
  printf("       &matrix[1]: %zu\n", &matrix[1]);             // 6171045324
  printf("   &matrix[1] + 1: %zu\n", &matrix[1] + 1);         // 6171045344
  printf("*(&matrix[1] +1 ): %zu\n\n", *(&matrix[1] + 1));    // 6171045344

  // If we were to type cast the memory address (pointer) that results from
  // &matrix[1] + 1 to be a pointer to an int, THEN we can output the element
  // 10 as de-referencing the pointer will access that value as it IS now a
  // pointer to an int instead of a pointer to an array of 5 ints!
  // type cast the pointer to an array of 5 ints to be a pointer to an int
  int *pointer = (int *) (&matrix[1] + 1);

  // If we output the memory address stored in pointer, it will be the exact
  // same as &matrix[1] + 1.  BUT now when we de-reference the pointer with
  // *pointer because it is a pointer to an int we actually will output the
  // int value 10 that's stored at this memory address.
  printf("pointer: %zu\n", pointer);      // 6171045344
  printf("*pointer: %zu\n", *pointer);    // 10

  // &matrix[1] + 1 returns a pointer to the 3rd row of the array, by applying
  // the de-reference operator once we get a pointer to the first element
  // of that array, and if we apply it AGAIN we get the value of that first
  // element, 10!
  printf("*(*(&matrix[1] + 1)): %zu\n", *(*(&matrix[1] + 1)));  // 10

  return 0;



  // Result:

  /*
    array: 6171045364
array + 1: 6171045368

    &array: 6171045364
&array + 1: 6171045384

        matrix[1]: 6171045324
    matrix[1] + 1: 6171045328
 *(matrix[1] + 1): 6

       &matrix[1]: 6171045324
   &matrix[1] + 1: 6171045344
*(&matrix[1] +1 ): 6171045344

pointer: 6171045344
*pointer: 10
*(*(&matrix[1] + 1)): 10

*/
}


















// Unions in C


// define a union data type called Data with 3 members... all of the members
// occupy the SAME space in memory, so assigning to one of them will effect
// (overwrite) the others!
union Data
{
  int x;
  double y;
  char z[32];
};

// Because union members use the same memory, the union will take up less
// space in memory than a struct with the same 3 members
struct Data2
{
  int x;
  double y;
  char z[32];
};

// Because it is often the case that only one union member can be used
// effectively at any given time, one pattern is to wrap a union in a
// struct where the struct has a member (in this case type) that signifies
// which union member is currently 'in use'.  So if type == 0 it could
// signify that buffer member x is in use, if type == 1 it could signify
// that buffer member y is in use, and so on...
typedef struct
{
  int type;
  union {
    char x[1024];
    float y[1024];
    double z[1024];
  } buffer;
} Info;

int union_example()
{
  // declare a union variable
  union Data mydata;

  // initialize the members of the union
  mydata.x = 10;
  mydata.y = 20.5;
  strcpy(mydata.z, "test 123");

  // mydata.x and mydata.z will not ouput 10 and 20.5 because the union members
  // all occupy the same space in memory, when we write "test 123" into mydata.z
  // we overwrite whatever was in that space in memory before (20.5)
  printf("mydata.x = %d\n", mydata.x);
  printf("mydata.y = %f\n", mydata.y);
  printf("mydata.z = %s\n", mydata.z);

  // if we set and output the members one at a time, we do output the value
  mydata.x = 10;
  printf("mydata.x = %d\n", mydata.x);

  mydata.y = 20.5;
  printf("mydata.y = %f\n", mydata.y);

  strcpy(mydata.z, "test 123");
  printf("mydata.z = %s\n", mydata.z);

  // the union will be allocated enough space to store the largest member, in
  // this case the 32 byte character array z
  printf("mydata size: %d\n", sizeof(mydata));
  printf("char size: %d\n", sizeof(char));

  // a struct allocates enough space to store ALL members, and so a struct with
  // the same members will take up more space (though all members can be used
  // simultaneously without issue)
  struct Data2 mydata2;
  printf("mydata2 size: %d\n", sizeof(mydata2));

  // we can access the members of a union when we have a pointer to that union
  // using the arrow notation -> the same as with structs
  union Data *ptr = &mydata;
  printf("ptr->z = %s\n", ptr->z);

  // the struct info contains a type member
  Info info;

  // we can set the type to '0' to signify that the x member of the buffer
  // is the one currently in use
  info.type = 0;

  // write all 't' characters into the buffer.x member
  memset(info.buffer.x, 't', 1024);

  // we could check to see that a certain member is in use before doing anything
  // with it, as below
  if (info.type == 0)
  {
    for (int i = 0; i < 1024; i++)
      printf("info.buffer.x[%d] = %c\n", i, info.buffer.x[i]);
  }

  return 0;
}


























// Void Pointers


int void_pointers()
{
  // Declare test int and double variables.
  int a = 5;
  double b = 3;

  // Declares a pointer variable x and assigns the memory address of 'a' to it,
  // the pointer is associated with the type int, so it's supposed to point to
  // int values like 'a'.
  int *x = &a;

  // We can use *x to dereference x and get the value 'pointed to' by x, i.e.
  // the value at the memory address that x stores.
  printf("%d\n", *x);

  // An advantage of having pointers with an associated type is that the
  // compiler can warn us if we're potentially making a mistake.  For example
  // if above we tried to assign the memory address of the double variable 'b'
  // to x the compiler will produce a warning, because x is supposed to point
  // to int values and 'b' is a double value.
  //
  // int *x = &b;


  // Here we declare a void pointer p, a 'generic pointer' that can store
  // memory addresses of any type of value without a warning.
  void *p;

  // p can store the memory address of 'a' OR 'b' without a compiler warning!
  p = &a;
  p = &b;

  // We cannot dereference void pointers.  When we dereference a pointer we
  // get an 'object' of the type associated with the pointer, but with a void
  // pointer the type 'void' is an incomplete type and we can't use it such
  // as an argument to printf() below...
  //
  // printf("%d\n", *p);



  //  Void pointers can be used to make data structures and functions which are
  //  "more generic" in terms of the types of data they can work with.
  //
  //  For example the malloc() function returns a void pointer to a block of
  //  allocated memory, allowing us to use the function to allocate blocks of
  //  memory for "anything", e.g. arrays of ints, or doubles or structs.
  //  In C++ we have to cast the void pointer to the type we want to use it as
  //  before we can use it, but in C we can use the void pointer directly.
  //  In C++, we have to do something like this:
  //  int *a = (int *) malloc(sizeof(int) * 10);
  //
  //  malloc() prototype: void *malloc(size_t size);
  //
  //  We can use malloc to declare space for an array of 10 ints...
  //
  //  int *a = malloc(sizeof(int) * 10);
  //
  //  Or we can use malloc to allocate space for a double...
  //
  //  double * a = malloc(sizeof(double));
  //
  //  And it's OK too!


  //  The qsort() function in the C standard library which performs quicksort
  //  also uses void pointers to make the function "more generic" in terms of
  //  what types of data the function can work with.  The qsort() function
  //  accepts a void pointer to an array of elements of "any type" as the first
  //  argument to the function.  The 4th argument to the function is a function
  //  pointer to a function that accepts two void pointers as arguments, this
  //  function is used by qsort() to carry out the quicksort algorithm by
  //  comparing elements.  The usage of void pointers allows this function
  //  to work with "any type" of data, e.g. we can supply functions to compare
  //  ints or functions to compar some type of struct.
  //
  //  void qsort(void *base,
  //             size_t nitems,
  //             size_t size,
  //             int (*compar)(const void *, const void*))


  // We can't dereference void pointers. When we dereference a pointer, we say we get
  // an object of the type that is assocaited with that pointer. But in case of void pointer,
  // void is an incomplete type, so if we dereference the pointer, it's not going to work.

  // Non-void pointers support what's called pointer arithmetic, where we can
  // manipulate what a pointer is pointing to using arithmetic operations.
  char string[] = "String";

  // The char pointer variable c will point to the 2nd character 't' in the
  // string
  char *c = &string[1];

  // We can dereference c to output 't'
  printf("%c\n", *c);

  // We can use pointer arithmetic to "increment" the pointer and have it point
  // to the next character (+ 1) in the string.  This is not adding the value
  // 1 to the pointer, it is incrementing the pointer by one char-sized unit
  // to point to the next char in memory.
  c = c + 1;

  // We can dereference c and now output 'r'
  printf("%c\n", *c);

  // We can set our void pointer to point to the 2nd character 't' in the
  // string as well.
  p = &string[1];

  // In the official C standard we are not allowed to use pointer arithmetic
  // with void pointers, but GNU C and other C compilers allow it.  In GNU C
  // the pointer will be incremented by one char-sized unit (i.e. a byte in
  // practice), so p will point to 'r'
  p = p + 1;

  // We cannot directly dereference void pointers, but we could cast this void
  // point to a char pointer (knowing that it is pointing to a char) by char *,
  // and then dereference THAT pointer to output the character 'r'.
  printf("%c\n", *((char *) p));

  return 0;
}































// Function pointers in C (Pointers to functions)


// prints int argument x
void function(int x)
{
  printf("x: %d\n", x);
}

// adds two double arguments and returns result
double add(double x, double y) { return x + y; }

// similar functions for subtract, multiply, divide operations
int subtract(int x, int y) { return x - y; }
int multiply(int x, int y) { return x * y; }
int divide(int x, int y) { return x / y; }

// select_operatio is a function that *returns* a pointer to a function
// (specifically, a pointer to a function with two int parameters and an int returns value).
int (*select_operation())(int, int)
{
  // let the user select an option
  int option = 0;
  printf("Select An Operation: \n");
  printf("1) Subtract\n");
  printf("2) Multiply\n");
  printf("3) Divide\n");
  printf("Enter: ");
  scanf("%d", &option);

  // return a function pointer to subtract, multiply or divide (or NULL if
  // invalid input)
  if (option == 1) return subtract;
  else if (option == 2) return multiply;
  else if (option == 3) return divide;
  else return NULL;
}

// returns true if the temperature int value is below freezing in Celsius,
// and false otherwise
bool freeze_C(int temperature) {
  if (temperature <= 0) return true;
  else return false;
}

// returns true if the temperature int value is below freezing in Fahrenheit,
// and false otherwise
bool freeze_F(int temperature) {
  if (temperature <= 32) return true;
  else return false;
}

// Function parameter free_check is a function pointer, specically for a
// function that with a single int parameter that returns a bool.  We call
// this type of parameter a "callback function":
//   https://en.wikipedia.org/wiki/Callback_(computer_programming)
// The function is_freezing will use the function freeze_check to check if
// the temperature is freezing.
// is_freezing is a function that accepts freeze_check as a parameter, and then
// returns void. freeze_check is a pointer to a function that accepts an int
// parameter and returns a bool.
void is_freezing( bool (*freeze_check)(int) ){

  // get a temperature value from the user
  int temperature = 0;
  printf("Enter Temperature: ");
  scanf("%d", &temperature);

  // use the provided freeze_check function to determine if it is freezing
  if (freeze_check(temperature)) printf("It's freezing!\n");
  else printf("It's NOT freezing!\n");
}

// Use function pointers with the above functions
int function_pointers()
{

  // ***** Example 1 - basic function pointer example *****

  // function_pointer is a pointer to a function that accepts an int parameter
  // and returns void.  We set it to point to the function called function.
  void (*function_pointer)(int);

  // set function_pointer to point to the function called function
  function_pointer = &function;

  // de-reference the function_pointer to call the function called function with
  // the argument 4!
  (*function_pointer)(4);



  // ***** Example 2 - alternative syntax *****

  // add_pointer will be a pointer to a function that accepts to double
  // parameters and returns a double... set it to point to add, notice how we
  // don't really need the & operator to give us the memory address of the add
  // function, it will actually work find without it!
  double (*add_pointer)(double, double) = add;

  // create two double variables
  double a = 20;
  double b = 30;

  // call the function pointed to be add_pointer by just using the regular
  // function call syntax... i.e. de-referencing the pointer to the function as
  // we did in our first example is actually optional
  double result = add_pointer(a,b);

  // output the result of calling the add_pointer function (we will get 50!)
  printf("result: %f\n", result);



  // ***** Example 3 - array of pointers to functions *****

  // declare and initialize an *array* of 'pointers to functions that accept
  // two int parameters and return an int'.  We do this by using [] to
  // declare the array and the usual {,} array initalization syntax
  int (*array[])(int, int) = {subtract, multiply, divide};

  // call the function at index 1 in the array (i.e. multiply)
  int product = (*array[1])(3,15);

  // output the result of calling the function
  printf("product: %d\n", product);



  // ***** Example 4 - function that returns a pointer to a function  *****

  // Call select_operation to ask the user for the operation they wish to
  // run and return a pointer to the function to carry it out (either
  // subtract, multiply, or divide).
  // select_operation returns a pointer to a function that accepts two int
  // parameters and returns an int.
  int (*operation)(int,int) = select_operation();

  // output the return value from calling the function pointed to be operation
  printf("answer: %d\n", operation(20,5));


  // ***** Example 5 - function pointer as an argument (callback function) *****

  // pass the freeze_C function as an argument to is_freezing
  printf("\nCelsius...\n\n");
  is_freezing(freeze_C);
  is_freezing(freeze_C);

  // pass the freeze_F function as an argument to is_freezing
  printf("\nFahrenheit...\n\n");
  is_freezing(freeze_F);
  is_freezing(freeze_F);

  printf("\n");

  return 0;
}



























// Macros and Function-like Preprocessor Macros

// We can't have a pointer to a macro like we can to a C function.

// What we call "object-like macros" are often used to define constant values in
// a program, such as a maximum value or PI
#define MAX 20
#define PI 3.14

// Macros are really just a text-replacement operation done by the C compiler
// as part of its first preprocessor phase (there are several more phases).  So
// for example we can replace the text "print" with a call to printf().
#define print printf("PRINT THIS!\n");

// We can also create "function-like macros" that accept and use parameters,
// here we add 1 to a parameter x.  Really all that will occur is "text
// replacement" though, so if we pass in the variable a, inc(a) will be
// replaced with a + 1.  It's important that there is no space between the
// macro name 'inc' and the beginning of the parameters (... otherwise we will
// have an error as it appears to be an object-like macro.
#define inc(x) x + 1

// Macros can accept multiple parameters.  One benefit of using macros compared
// to regular C functions is that macros can be "generic", so for example this
// macro will work with both ints and doubles as all that really occurs is
// text replacement!
#define area(base,height) 0.5 * base * height

// We can use a macro as an argument to another macro, so for example with the
// below function-like macro to find the min of two numbers, we could use it
// like: min(3,min(1,2)).  We wrap the expression in () brackets to give it a
// higher order of evaluation.  We do this because when the preprocessor
// expands macros we may get large expressions as a result, and we want the
// result of this macro to be evaluated FIRST before being used as part of a
// larger expression in order to get the desired behaviour.
#define min(x,y) ((x < y) ? x : y)

// We can use the # operator, sometimes called the stringizer operator, to turn
// any function-like macro argument into a string literal (e.g. s1).  It will
// also concatenate the string with the adjacent string literal.  We can also
// output a string using printf() in the usual way as we do with s2.
#define output(s1,s2) printf(#s1 " middle %s\n", s2);

// We can define a function-like macro across multiple lines by using ({ ... })
// and ending each line with the \ character.  Here we find the largest element
// in an array.  We use typeof() to find the type of elements in the array so
// that we can keep the function generic, such that it will work with both ints
// and doubles for example.
#define find_max(array,length) ({ \
  typeof(array[0]) current_max = array[0]; \
  for (int i = 1; i < length; i++) \
    if (array[i] > current_max) \
      current_max = array[i]; \
  current_max; \
})

// Prints a number 3 times.  We make sure to evaluate the parameter using (num)
// before using the paramter, otherwise we may get unwanted behaviors if an
// expression is passed as a parameter to the function-like macro and that
// parameter is evaluated 3x (once inside each printf function call).

/*
#define print_number_3x(num) ({ \
  printf("number: %d\n", num); \
  printf("number: %d\n", num); \
  printf("number: %d\n", num); \
})
*/

// int nume = (num); -> evaluates the parameter num once and assigns it to nume
// Evaluate the number before we use it as an argument to printf() in the macro
#define print_number_3x(num) ({ \
  int nume = (num);   \
  printf("number: %d\n", nume); \
  printf("number: %d\n", nume); \
  printf("number: %d\n", nume); \
})

// Create a global variable number and a function which increments and returns
// the value of the number, such that we can demostrate a possible issue that
// may occur when a function-like macro argument is an expression.
int number = 0;
int increment()
{
  number++;
  return number;
}


// Remember that macros are really just text-replacement operations done by the
// C compiler as part of its first preprocessor phase. No function is ever
// truly called, and the computation is done with an inline expression.  This
// is an advantage of using function-like macros compared to regular C functions.
// Another advantage is a lack of overhead, i.e. extra work, from having to call
// a function as the computation is done with an inline expression.
int macros_examples()
{
  // Print out the MAX constant value (i.e. object-like macro), the compiler
  // will replace MAX with 20 during its first preprocessor phase, so the actual
  // line of code that is truly compiled will be printf("Max: %d\b", 20);
  printf("Max: %d\n", MAX);

  // While it may seem unusual to have a line of code with no ; this will work
  // because print is a macro and this will be replaced with a function call
  // to printf() as defined above.
  print

  // Test out the inc() function-like macro, the code 'a = inc(a);' will be
  // replaced with the code 'a = a + 1;' and a will be incremented.
  int a = 2;
  a = inc(a);
  printf("a: %d\n", a);

  // Test out the area() function-like macro, the code 'area(base1,height1)'
  // will be replaced with the code '0.5 * base1 * height1' as per the macro
  // definition, and the area of the triangle will be calculated.
  int base1 = 20;
  int height1 = 20;
  int area1 = area(base1,height1);
  printf("area1: %d\n", area1);

  // We can also use the macro with double variables and unlike a regular C
  // function it will work fine!  Because again "text-replacement" will occur,
  // no function is ever truly called.  This is an advantage of using
  // function-like macros compared to regular C functions.  Another advantage
  // is a lack of overhead, i.e. extra work, from having to call a function as
  // the computation is done with an inline expression, in this case
  // '0.5 * base2 * height2', as opposed to having to call a function which
  // involves creating and assigning values to local parameter variables
  // and other work.
  double base2 = 10.5;
  double height2 = 5.2;
  double area2 = area(base2, height2);
  printf("area2: %f\n", area2);

  // Use the result of a macro as an argument of another macro.  You can see
  // the expansion of macros by the preprocessor using the -E option in gcc,
  // so for example: 'gcc -E -o function_like_macros function_like_macros.c'.
  // The below statement will expand into a lengthy expression involving
  // multiple ternary operators.  It is essential that we wrapped the macro
  // expression in ( ) when defining min to ensure that the resulting
  // expressions are evaluated in the desired order, if we did not do this
  // we may get bugs as a result.
  int min_num = min(min(8,1),min(6,3));
  printf("min_num: %d\n", min_num);

  // The # operator in the output() function-like macro will turn "test1" (or
  // any other parameter) into a string literal.
  char string[] = "test2";
  output(test1,string);

  // Test the find_max() function-like macro with an int array.
  int int_array[] = {3,5,2,1,8,3,2};
  int max_int = find_max(int_array,7);
  printf("max_int: %d\n", max_int);

  // Test the find_max() function-like macro with a double array.
  double dbl_array[] = {9.3, 2.4, 10.2, 10.9};
  double max_dbl = find_max(dbl_array, 4);
  printf("max_dbl: %f\n", max_dbl);

  // With a regular function in C, if we were to pass in num = increment() as an
  // argument, the expression would first evaluate.  The increment() function
  // call would increment the global number variable to 1 and then return it,
  // this value would be assigned to num, and the entire assignment = operator
  // would evaluate to '1' which would then be passed to the function.  But
  // remember that function-like macros are essentially text replacement
  // operations, and we will not get this behaviour!
  //
  // Instead the expression num = increment() will be used in function-like
  // macro, potentially evaluating the expression multiple times as it is used
  // in the macro (e.g. evaluating 3x in each call to printf() in the
  // print_number_3x macro).  If this were to occur, we could print out the
  // numbers 1,2,3 instead of the number 1 three times!
  //
  // Because we evalute the parameter 'num = increment()' in the print_number_3x
  // function-like macro *before* we use it in the macro, we will get the number
  // 1 output three times as desired.
  int num;
  print_number_3x( num = increment() );

  return 0;
}

























// Files in C

int file_examples(void)
{
  // We use a file handle created with FILE to access a file
  FILE *fh_output;

  // Open the file with fopen in writing mode with "w", which will create
  // the file if it does not already exist and set it to "blank" if it does
  // exist.  We could use append mode with "a" instead and it would append
  // whatever we write to the file after the existing content of the file.
  fh_output = fopen("io.txt", "w");

  // fputs is one way of writing a string to a file...
  // fputs("abc", fh_output);
  // fputs("123\n", fh_output);
  // fputs("A string\n", fh_output);

  // fprintf works very much like printf, with placeholders that allow us to
  // output things like int values to the file
  // int data = 5;
  // fprintf(fh_output, "data: %d\n", data);

  // we can write to a file in a loop to output a set of data to a file
  // for (int i = 0; i < 10; i++)
  //  fprintf(fh_output, "%d\n", i);

  // here we use an indefinite loop to output a varying amount of information to
  // the file depending on how many numbers the user wants to input
  int input = 0;
  while (true)
  {
    printf("Enter # (-1 quits): ");
    scanf("%d", &input);
    if (input == -1) break;
    else fprintf(fh_output, "%d\n", input);
  }

  // when we're done with the file we use fclose to close the file handle
  fclose(fh_output);

  // open the same file we just wrote to, but this time open it for reading with
  // "r" mode
  FILE *fh_input;
  fh_input = fopen("io.txt", "r");

  int finput = 0;
  int lines = 0;
  int numbers[100];

  // read each int in the file with fscanf that works much like scanf, it will
  // return EOF when the end of file is reached
  // fscanf will keep reading until it encounters EOF (end of file) and it will read
  // the numbers in the file one by one(when it encounters a newline) and store them
  // in the variable finput
  while (fscanf(fh_input, "%d", &finput) != EOF)
  {
    // store the number we've input into the numbers array, keep track of how
    // many numbers we've read with lines... numbers act as a 'buffer' that is
    // able to store some amount of numbers (up to 100)
    numbers[lines] = finput;
    printf("file line %d: %d\n", lines+1, finput);
    lines++;
  }

  // compute the average of the numbers in the file to perform some work with
  // the data we've just read
  int total = 0;
  for (int i = 0; i < lines; i++)
    total += numbers[i];
  printf("Average: %d\n", total / lines);

  // close the file handle
  fclose(fh_input);

  // open another file called in.txt for reading... we can re-use the same
  // file handle fh_input since we have closed it just previously to this
  fh_input = fopen("in.txt", "r");

  // we create a char array called 'buffer', we can call this an inptu buffer
  // as it will store some amount of input
  char buffer[256];

  // fgets will read up to 256 chars and store them into buffer, until it
  // encounters EOF (the end of file) or the first newline
  fgets(buffer, 256, fh_input);
  printf("Buffer: %s\n", buffer);

  // close the file handle
  fclose(fh_input);

  return 0;
}
























//  The difference between a constant pointer and a pointer to a constant in C

int constant_pointer()
{
  // Declare two char variables so we have something to point our pointers to
  char a = 'a';
  char b = 'b';

  // Declares a constant pointer called 'constant_pointer' and sets it to
  // point to the variable a, i.e. we assign it a's memory adddress.  We
  // CAN change the value of what a constant pointer is pointing to, we CAN'T
  // change what the constant pointer is pointing to (i.e. the memory address
  // that it stores).
  char *const constant_pointer = &a;

  // De-reference the constant pointer to access what it is pointing to with
  // the de-reference operator '*'.  In this case, we CAN assign the value 'x'
  // to a, changing the value of what the pointer is pointing to.
  *constant_pointer = 'x';

  // We CAN'T change what a constant pointer is pointing to, this will cause
  // a compilation error.
  // constant_pointer = &b;

  // If we output the variable a we'll find it has been changed to the value 'x'
  printf("a: %c\n", a);

  // Either of the below statements will declare a pointer to a constant
  // called 'pointer_to_const' and set it to point to the variable a.
  //
  // char const *pointer_to_const = &a;
  const char *pointer_to_const = &a;

  // We'll get 'x' if we output the de-referenced pointer as it points to
  // the variable 'a' which now stores the char 'x'
  printf("*pointer_to_const: %c\n", *pointer_to_const);

  // We CAN change what a pointer to a const is pointing to, in this case
  // we have the pointer point to the variable 'b'.
  pointer_to_const = &b;

  // If we output the de-referenced pointer we'll now find we output 'b'
  // as the pointer now points to the variable 'b' which stores the char 'b'.
  printf("*pointer_to_const: %c\n", *pointer_to_const);

  // We CAN'T change the value of what a pointer to a constant is pointing to,
  // in this case if we try to de-reference the pointer to set the variable
  // 'b' to the value 'z' we will get a compilation error.
  // *pointer_to_const = 'z';

  // We can also create a constant pointer to a constant, using either of
  // the below syntaxes.  In this case we cannot change what the pointer is
  // pointing to OR the value of what the pointer is pointing to!
  //
  // const char *const constant_pointer_to_constant = &a;
  char const *const constant_pointer_to_constant = &a;

  // Attempting to change the value of what the pointer is pointing to will
  // cause a compilation error.
  // *constant_pointer_to_constant = 'z';

  // Attempting to change what the pointer is pointing to will casue a
  // compilation error.
  // constant_pointer_to_constant = &b;

  return 0;
}

//  A summary of the different types of pointers we can create and what
// is mutable/immutable (i.e. changable vs. unchangable).
//
//
//                         Pointer      Value
//
//  Non-Const Pointer      Mutable      Mutable
//  To Non-Const
//
//  Non-Const Pointer      Mutable      Immutable
//  To Const
//
//  Const Pointer          Immutable    Mutable
//  To Non-Const
//
//  Const Pointer          Immutable    Immutable
//  To Const





























// * Program: Dynamically Allocate An Array Of Structs In C


// A struct for representing a point, the description member is expected to
// point to dynamically allocated memory where a string will be stored.
typedef struct {
  int x;
  int y;
  char *description;
} Point;

int array_of_structs(int argc, char *argv[])
{
  // Declare a Point struct on the stack
  Point p1;

  // Initialize the members using the dot operator
  p1.x = 5;
  p1.y = 2;

  // Output the members
  printf("(%d, %d)\n", p1.x, p1.y);


  // Declare a pointer to a Point struct that will store the memory address
  // of a Point struct on the heap.  malloc() will allocate enough space in
  // bytes on the heap for the Point struct, by using sizeof(Point) we get
  // the size in bytes to store a Point struct and pass it to malloc to
  // ensure it allocates this amount of space.  malloc() will return the memory
  // address of this block of memory, and we store it into the pointer
  // variable p2.
  Point *p2 = malloc(sizeof(Point));

  // We can set the struct members of the struct on the heap either by
  // dereferencing the pointer and then using the dot operator, or by using
  // the arrow operator.
  (*p2).x = 6;
  p2->y = 10;

  // Output the members
  printf("(%d, %d)\n", (*p2).x, p2->y);

  // Free the dynamically allocated memory to make it available again and
  // to prevent a memory leak.
  free(p2);


  /****************************************************************************
      Example of dynamically allocating space for an array of structs and then
      using realloc() to increase the length of the array.
  ****************************************************************************/

  // Declare and initialize a length variable to keep track of the length of
  // the array
  int length = 3;

  // array is a pointer to a Point struct.  This time when we call malloc() and
  // provide the number of bytes to allocate as an argument, we multiply
  // sizeof(Point) (which is the size in bytes to store a Point struct) by the
  // length of our array (i.e. the number of Point structs to be stored).  This
  // will give us enough space to store the array of structs, and malloc() will
  // return the memory address of the first Point struct which we store
  // into array.
  Point *array = malloc(sizeof(Point) * length);

  // We can access and set the members of each struct in the array with this
  // syntax, here we set the members of the first struct in the array.
  array[0].x = 1;
  array[0].y = 1;

  // Setting the members of the second struct in the array
  array[1].x = 2;
  array[1].y = 2;

  // Setting the members of the third struct in the array
  array[2].x = 3;
  (*(array + 2)).y = 3;

  // We might be surprised that we are using the . dot operator to access the
  // struct members, given in the above example where we had a pointer to a
  // struct (p2) we either had to dereference the pointer THEN use the . dot
  // operator OR we had to use the arrow operator.  But remember that the
  // syntax...
  //
  // array[2].y = 3;
  //
  // ...is acutally the same as this, where we use pointer arimthetic to get
  // the memory address of the struct "two over" from where array is pointing
  // (i.e. the 3rd struct in the array), and then dereference the pointer to
  // access the struct:
  //
  // (*(array + 2)).y = 3;
  //
  // Using the array index notation array[2] is effectively like using a
  // dereferenced pointer to the struct.  We use pointer arimthetic to access
  // the struct in the above example for the y member of the 3rd struct in
  // the array.

  // Output the values of the members of the elements of the array...
  printf("\n");
  printf("Array...\n");
  for (int i = 0; i < length; i++)
  {
    printf("(%d, %d)\n", array[i].x, array[i].y);
  }
  printf("\n");

  // One of the key benefits of using dynamically allocated memory is that we
  // can reallocate blocks of memory to modify their size, for example by
  // increasing the size of the array by 1.  Let's start by setting length to
  // 4 to keep track of the new array length.
  length = 4;

  // Then we call realloc() to allocate space for this larger block of memory
  // to store the larger array.  The 2nd argument to realloc() is the size of
  // the new block of memory.  We multiply the new length of the array by the
  // size in bytes that it takes to store a Point struct as given by
  // sizeof(Point) to get the total number of bytes required.  We pass as a
  // first argument to realloc() the pointer (memory address) of the block
  // of memory that we wish to reallocate.  Sometimes the block of memory will
  // be increased in size at its existing location in memory, in which case
  // realloc() will return the same pointer (memory address) which we then store
  // into array.  It's possible that the block of memory that's allocated cannot
  // be expanding at its existing location in memory (perhaps it would require
  // overwriting nearby memory that is not available).  In this case, realloc()
  // may move the block of memory to a new location in memory, in which case it
  // will return this new pointer (memory address) and we will store this into
  // array to ensure that array continues to point to our dynamically allocated
  // array on the heap.
  array = realloc(array, sizeof(Point) * length);


  // This will allocate a completely new block of memory, copy the contents of
  // the old block of memory to the new block of memory, and then free the old
  // block of memory.
  // array = realloc(NULL, sizeof(Point) * length);

  // After increasing the size of the array, it will have a 4th element, and
  // we can set the members of that element
  array[3].x = 4;
  array[3].y = 4;

  // Output the values of the members of the elements of the array that is
  // now one element larger so we can see the difference
  printf("\n");
  printf("Array...\n");
  for (int i = 0; i < length; i++)
    printf("(%d, %d)\n", array[i].x, array[i].y);
  printf("\n");

  // Free the dynamically allocated array to make the memory avaiable again
  // and to prevent a memory leak.
  free(array);


  /****************************************************************************
      Example of dynamically allocating space for an array of structs, where
      the struct members themselves contain pointers to dynamically allocated
      memory.  This example is intended to illustrate that we need to free
      the dynamically allocated memory for each of these struct member pointers,
      otherwise we will have a memory leak.  To test out this example, wrap the
      above example in multiline comments and then uncomment the code below.
  ****************************************************************************/

  /*

  // Declare and initialize a variable to keep track of the length of the
  // dynamically allocated array.  Allocate space for an array with this
  // length on the heap, using the same technique as previously outlined in
  // the above example.
  int length = 3;
  Point *array = malloc(sizeof(Point) * length);

  // Initialize the x and y members of the first struct in the array
  array[0].x = 1;
  array[0].y = 1;

  // Dynamically allocate space for a char array of 10 characters using
  // malloc() and have description store the pointer (memory address) for this
  // dynamically allocate char array.  Note that a char = 1 byte on virtually
  // all systems, so we can just pass 10 directly instead of using sizeof(char).
  // Technically malloc doesn't allocate space for a number of bytes, it
  // actually allocates space for a "number of char sized units", so even if
  // a char wasn't 1 byte this would work!
  array[0].description = malloc(10);

  // Use strcpy() to store a string into the dynamically allocated char array
  // that description points to
  strcpy(array[0].description, "point 1");

  // Do the same as the above for the 2nd struct in the array
  array[1].x = 2;
  array[1].y = 2;
  array[1].description = malloc(10);
  strcpy(array[1].description, "point 2");

  // And again do the same as the above for the 3rd struct in the array
  array[2].x = 3;
  (*(array + 2)).y = 3;
  array[2].description = malloc(10);
  strcpy(array[2].description, "point 3");

  // Output the values of the members of the structs in the array, including
  // the description string.
  printf("\n");
  printf("Array...\n");
  for (int i = 0; i < length; i++)
  {
    printf("(%d, %d)", array[i].x, array[i].y);
    printf(" - %s\n", array[i].description);
  }
  printf("\n");

  // We cannot JUST use free(array) to free the dynamically allocated memory
  // in this case, because the structs THEMSELVES contain pointers to
  // dynamically allocated memory.  We need to free THIS dynamically allocated
  // memory as well, and we do so by looping through the array elements and
  // freeing the block of memory allocated for each description.
  for (int i = 0; i < length; i++)
    free(array[i].description);

  // We can then free the block of memory for our array of structs
  free(array);

  */

  return 0;
}
























// NULL Pointers in C

struct node
{
  int value;
  struct node *next;
};

int null_pointers(void)
{

  // BEST PRACTICE: Initialize a pointer to NULL

  // If we declare a pointer variable like this, it will store whatever value
  // happens to be in memory at the time it is declared.  This is a problem
  // because the pointer could be pointing to memory that our program doesn't
  // actually "own".
  //
  // int *ptr;

  // If we're not going to initialize the pointer variable right away to a valid
  // memory address, we should initialize it to NULL as NULL means that the
  // pointer "points to nothing".  It's an explicit way of setting the pointer
  // to point to nothing initially.  We can check for NULL before working
  // with a pointer value to understand whether it points to nothing or a valid
  // memory address instead (i.e. is the pointer ready to be worked with, or
  // do we perhaps need to allocate memory or something).
  int *ptr = NULL;

  // If we output the pointer value we'll most likely see it is set to 0x0
  // once we have set it to NULL.  NULL is virtually always "0" but as
  // noted above it's possible to represent it with a non-zero bit pattern.
  printf("ptr: %p\n", ptr);

  // One common use case of the value NULL is for it to be returned when a
  // function cannot allocate memory.  For example malloc() will attempt to
  // allocate a block of memory and return the memory address of that block
  // of memory, but if it fails it will return NULL.
  ptr = malloc(sizeof(int));

  // If instead of the above attempt to allocate space for one int we try
  // to allocate space for many, many ints it is possible malloc() will
  // fail and return NULL instead of a valid memory address.
  //
  // ptr = malloc(sizeof(int) * 1000000000000000);

  // We can CHECK the pointer variable to see if it doesn't equal NULL before
  // working with it as if it is a valid memory address!
  if (ptr != NULL)
  {
    // Dereference the pointer to set a value and output the value
    *ptr = 5;
    printf("*ptr: %d\n", *ptr);
  }
  // if the pointer DOES equal NULL we can then handle it however we like...
  else
   printf("Error: ptr is NULL!\n");



  // BEST PRACTICE: set a free'd pointer variable to NULL

  // Notably calling free() does not set a pointer to NULL, let's output
  // ptr before calling free()
  printf("\nbefore free:\n%p\n", ptr);

  // Calling free() will only free the block of memory that was allocated,
  // it will not modify the memory address that is stored by ptr.  This is a
  // problem because then ptr would be pointing to memory that our program
  // does not "own" anymore.  We call this situation a dangling pointer:
  // https://en.wikipedia.org/wiki/Dangling_pointer
  free(ptr);

  // As a best practice if we intend to re-use a pointer variable after calling
  // free we should set the pointer variable to NULL to make it clear that the
  // pointer no longer points to valid memory.
  ptr = NULL;

  // If we were to comment out the line above, we would notice that ptr still
  // stores the same memory address as it did BEFORE calling free(), i.e. we
  // we would have a dangling pointer.
  printf("\nafter free:\n%p\n", ptr);



  // COMMON USE CASE: Data Struture "End Points"

  // Let's use our struct node above to create a basic linked list data
  // structure, with node n1 linked to node n2 which is linked to the last
  // node in the chain n3.
  //
  // n1 -> n2 -> n3

  // Declare the 3 struct node variables
  struct node n1, n2, n3;

  // Set n1's value to 5 and link it to n1 by having its next member store
  // the memory address of n2.
  n1.value = 5;
  n1.next = &n2;

  // Set n2's value to 6 and link it to n3 by having its next member store
  // the memory address of n3.
  n2.value = 6;
  n2.next = &n3;

  // Set n3's value to 7 and set it to be the LAST node in the chain by
  // settings its next member to NULL.
  n3.value = 7;
  n3.next = NULL;

  // We can traverse the linked list by using a pointer variable, we'll set
  // the struct node pointer variable traverse to initially point to the
  // first node in the above chain.
  struct node *traverse = &n1;

  // Now we can output the linked list...
  printf("\n\nLinked List: ");

  // So long as traverse is not yet equal to NULL we'll know we haven't yet
  // reached the end of the chain of nodes.
  while (traverse != NULL)
  {
    // Output the value stored in node that travserse is currently pointing to
    printf("%d ", traverse->value);

    // Set traverse to point to the NEXT node in the chain using the next
    // member of the node that traverse is currently pointing to.  When
    // traverse reaches the LAST node of the chain, its next member will be
    // set to NULL and the loop will stop!
    traverse = traverse->next;
  }

  // Output a newline
  printf("\n");



  // COMMON USE CASE: Special Function Behaviour When Passed As Argument

  // Some functions that expect a pointer as an argument when NULL is
  // passed to the function instead of a valid memory address.  For example
  // realloc() will dynamically allocate space for an all new block of
  // memory rather than try to reallocate space for an existing block of
  // memory when it is passed NULL as the first argument rather than a valid
  // memory address.

  // Dynamically allocate space for a block of memory to store 10 ints using
  // malloc()
  int *array = malloc(sizeof(int) * 10);

  // Output the value of array
  printf("\nbefore array:\n%p\n", array);

  // If we were to pass the memory address stored in array as the first
  // argument to realloc() it will attempt to enlarge the block of memory by
  // 1 int to store 11 ints.  It will attempt to enlarge the block of memory
  // at its existing location, meaning that the memory address returned by
  // realloc() will almost certainly be the same in the case of this
  // program (as a general rule, realloc may move the block of memory being
  // reallocated somewhere else entirely if for example it can't find more
  // space at the existing location in memory).
  //
  // array = realloc(array, sizeof(int) * 11);

  // If instad we pass realloc() NULL as its first argument it will allocate
  // space for a new block of memory entirely, the behaviour of the function
  // changes almost entirely as a result.
  array = realloc(NULL, sizeof(int) * 11);

  // Output the pointer after calling realloc() and we'll find it's set to
  // a new location in memory entirely.
  printf("\nafter array:\n%p\n", array);



  // The size in bytes that NULL takes up in memory will be equal to the size
  // of a pointer variable.  This can vary from one system and compiler to
  // the next, but something like 8 bytes would be typical.
  printf("sizeof(NULL): %zu\n", sizeof(NULL));
  printf("sizeof(int*): %zu\n", sizeof(int*));



  // NULL also has the property that if it is stored in pointer variables of
  // different types a check for equality between these variales will
  // return true.

  // Here we set char and int pointer variables to NULL
  char *char_pointer = NULL;
  int *int_pointer = NULL;

  // When we compare them for equality we will find out they ARE equal
  if (char_pointer == int_pointer)
    printf("NULL pointers equal\n");

  return 0;
}




























// Allocating Memory for 2D Arrays in C


int twoD_arrays()
{
  // We can declare a 2D array on the stack like this... in this case we have
  // a 2D array of ints with 3 rows and 3 columns.
  // int array[3][3] =
  // { {1,2,3},
  //   {4,5,6},
  //   {7,8,9}
  // };

  // declare and initialize variables for the number of rows and column, we
  // could set these values based on user input or in some other way such
  // that the rows and columns of the 2D array would be determined at runtime
  int rows = 3;
  int cols = 3;

  // array is a pointer to a pointer to an int... it will point to an array of
  // 3 pointers to ints that we allocate space for with malloc.  Each of these
  // pointers to an int can be accessed with array[i], and each pointer to
  // will point to a row (1D array) of ints in our 2D array.
  // This (2D arrays stored on heap) is different than how 2D arrays are stored
  // on the stack, where they are stored as a single block of memory.
  int **array = malloc(sizeof(int *) * rows);

  // int **array = { int* array0, int* array1, int* array2 };


  // int* array0 = malloc(sizeof(int) * 3);
  // int* array1 = malloc(sizeof(int) * 3);
  // int* array2 = malloc(sizeof(int) * 3);

  // Initialize each pointer to an int to point to a dynamically allocated 1D
  // array of 3 ints (i.e. a row in our 2D array)
  for (int i = 0; i < rows; i++)
    array[i] = malloc(sizeof(int) * cols);

  // We can now access the array elements like this:
  //
  // array[1][2]
  //
  // Where array[1] accesses the 2nd row in our 2D array, and array[1][2]
  // accesses the 3rd element in that row!


   // Set each element in the array to a unique value.
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      array[i][j] = (i * 10) + j;

  // Print out the values in the 2D array as a grid.
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      printf("%02d ", array[i][j]);
    printf("\n");
  }
  printf("\n");



  // EXAMPLE: Increase The Number Of COLUMNS

  // Increase the number of columns by 3.
  cols += 3;

  // Each row needs to increase in size by 3, so we use realloc() to reallocate
  // space for each row (array[i]) using the new cols value.
  for (int i = 0; i < rows; i++)
    array[i] = realloc(array[i], sizeof(int) * cols);


  // Set each element in the array to a unique value.
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      array[i][j] = (i * 10) + j;

  // Print out the values in the 2D array as a grid.
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      printf("%02d ", array[i][j]);
    printf("\n");
  }
  printf("\n");



  // EXAMPLE: Increase The Number Of ROWS

  // Increase the number of rows by 3
  int new_rows = 3;
  rows += new_rows;

  // We need to use realloc() to allocate enough space to store the new number
  // of rows.  This will give us the ability to store pointers to the new
  // rows we'll need to allocate.
  array = realloc(array, sizeof(int *) * rows);

  // Allocate space for the new rows... these are NEW rows so we'll need to use
  // malloc instead of realloc, and we only need to allocate space for the
  // "new rows" at the end of our array of pointers to ints, i.e. from
  // rows - new_rows ... rows.
  for (int i = rows - new_rows; i < rows; i++)
    array[i] = malloc(sizeof(int) * cols);


  // Set each element in the array to a unique value.
  for (int i = 0; i < rows; i++)
    for (int j = 0; j < cols; j++)
      array[i][j] = (i * 10) + j;

  // Print out the values in the 2D array as a grid.
  for (int i = 0; i < rows; i++)
  {
    for (int j = 0; j < cols; j++)
      printf("%02d ", array[i][j]);
    printf("\n");
  }
  printf("\n");






  // To free the dynamically allocated 2D array we first loop through the
  // array of pointers to ints and free each individual row in our 2D array.
  for (int i = 0; i < rows; i++)
    free(array[i]);

  // We can then free the array of pointers to ints
  free(array);

  return 0;
}


// This is how the dynamically allocated 2D array can be visualized and how
// it is laid out in memory...
//
//
//          array of
//          pointers
//             |
//             |
//             |
// array -> array[0] -> 1,2,3   <--- arrays of ints
//          array[1] -> 4,5,6
//          array[2] -> 7,8,9
//
//
//
//           ----- array -----
//          |        |        |
//          |        |        |
//     array[0]  array[1]   array[2]
//      / | \      / | \     / | \
//     1  2  3    4  5  6   7  8  9
//
//
//
//                 Address     Value
//
//        array    12          24
//          ...    ...         ...
//     array[0]    24          64    <- malloc for int* x 3
//     array[1]    32          80
//     array[2]    40          96    array[1][2]
//          ...    ...         ...
//  array[0][0]    64          1     <- malloc for int x 3
//  array[0][1]    68          2
//  array[1][2]    72          3
//          ...    ...         ...
//  array[1][0]    80          4     <- malloc for int x 3
//  array[1][1]    84          5
//  array[1][2]    88          6
//          ...    ...         ...
//  array[2][0]    96          7     <- malloc for int x 3
//  array[2][1]    100         8
//  array[2][2]    104         9
//          ...    ...         ...
//
//  In total we have 4 pointers to ints and 9 ints
//
// The 2D array on the stack is laid out differently in memory...
//  (here we have only 9 ints in memory and no pointers to ints)
//  Each row in the 2D array is stored contiguously in memory one after the other.
//
//  int array[3][3] = {{1, 2, 3},
//                     {4, 5, 6},
//                     {7, 8, 9}};
//
//
//  Gives us a single "block of 9 ints in memory".
//
//
//
//  Memory
//  ------
//
//                  Address     Value
//                  ...
//  array[0][0]     04          1       <- array[0]
//  array[0][1]     08          2
//  array[0][2]     12          3
//  array[1][0]     16          4       <- array[1]
//  array[1][1]     20          5
//  array[1][2]     24          6
//  array[2][0]     28          7       <- array[2]
//  array[2][1]     32          8
//  array[2][2]     36          9
//
//
//                          array[0][2]
//                         /
//   array[0]  ->  {1, 2, 3}
//   array[1]  ->  {4, 5, 6}
//   array[2]  ->  {7, 8, 9}
//                      \
//                       array[2][1]
//
//
// This has a notable difference in terms of the number of bytes required to
// store the array...
//
//
//  2D array on the stack:
//
//     9 ints stored in memory = 9 x 4 bytes = 36 bytes
//
//
//  2D array with dynamic memory allocation:
//
//     Total 4 pointers:
//         1 pointer to pointer to int = 8 bytes
//         3 pointers to ints = 3 x 8 bytes = 24 bytes
//
//     Total 9 ints:
//         3 x 3 int blocks = 3 x 3 x 4 bytes = 36 bytes
//
//      Total memory required = 8 + 24 + 36 = 68 bytes





























int time_examples() {
  // gets the current time as the number of seconds since Jan. 1, 1970 midnight
  time_t now = time(NULL);

  // time_t is really a long value, so we can print it out with %ld
  printf("%ld\n", now);

  // we can use difftime to get the difference between two times, here we sleep
  // for 2 seconds so that there will be a difference in time
  //
  // sleep(2);
  // time_t two_secs = time(NULL);
  // printf("%ld\n", two_secs);
  // double diff = difftime(two_secs, now);
  // printf("diff: %f\n", diff);

  // ctime will return a nicely formatted string representation of the time
  char *string_now = ctime(&now);
  printf("%s\n", string_now);


  // gmtime returns a pointer to a struct tm which has members for each
  // individual piece of time data that we might want to work with.  Returns
  // it in Greenwich Mean Time.
  struct tm *gm_time = gmtime(&now);

  // See the definition of the struct members here:
  //   https://www.tutorialspoint.com/c_standard_library/time_h.htm
  printf("tm_sec: %d\n", gm_time->tm_sec);
  printf("tm_min: %d\n", gm_time->tm_min);
  printf("tm_hour: %d\n", gm_time->tm_hour);
  printf("tm_mday: %d\n", gm_time->tm_mday);
  printf("tm_mon: %d\n", gm_time->tm_mon);
  printf("tm_year: %d\n", gm_time->tm_year); // number of years since 1900
  printf("tm_wday: %d\n", gm_time->tm_wday);
  printf("tm_yday: %d\n", gm_time->tm_yday);
  printf("tm_isdst: %d\n", gm_time->tm_isdst);
  printf("\n");


  // local time also returns a struct tm, but for our own local time zone
  struct tm *cur_time = localtime(&now);
  printf("tm_sec: %d\n", cur_time->tm_sec);
  printf("tm_min: %d\n", cur_time->tm_min);
  printf("tm_hour: %d\n", cur_time->tm_hour);
  printf("tm_mday: %d\n", cur_time->tm_mday);
  printf("tm_mon: %d\n", cur_time->tm_mon);
  printf("tm_year: %d\n", cur_time->tm_year);
  printf("tm_wday: %d\n", cur_time->tm_wday);
  printf("tm_yday: %d\n", cur_time->tm_yday);
  printf("tm_isdst: %d\n", cur_time->tm_isdst);

  // asctime uses a struct tm to created a nicely formatted string
  // representation of the time
  char *other_string = asctime(cur_time);
  printf("%s\n", other_string);

  // mktime will convert from a struct tm time to a time_t representation
  // here we increment the # of minutes of cur_time by 1 and then convert
  // it, and so when we compare it to now it should be 60 seconds larger!
  cur_time->tm_min = cur_time->tm_min + 1;
  time_t new_time = mktime(cur_time);
  printf("now: %ld\n", now);
  printf("new_time: %ld\n", new_time);

  // strftime will format a string representation of a given time
  // according to specifiers, see a full list of them here:
  // https://www.tutorialspoint.com/c_standard_library/c_function_strftime.htm
  //
  // The function accepts a buffer to store the string, a max number of chars,
  // a string with specifiers specifying the format to create, and the time
  // in the form a struct tm.  It returns the length of the created string (or
  // 0 if the string is over the max length specified).
  char s[100];
  int strlen = strftime(s,100,"%l %p", cur_time);
  printf("%s\n", s);
  printf("%d\n", strlen);

  // clock() returns the number of CPU clock ticks since the program began, so
  // we can get the clock tics before and after some process (stored as clock_t
  // type data), and divide by the constant value CLOCKS_PER_SEC to get the
  // number of seconds (perhaps a fraction of seconds) for some process to
  // occur!
  clock_t start, end;
  start = clock();
  for (int i = 0; i < 100000000; i++) { }
  end = clock();

  double total = (double) (end - start) / CLOCKS_PER_SEC;
  printf("Time: %fs\n", total);

  return 0;
}































/*******************************************************************************
*
* Program: Comma Operator Demonstration
*
* Description: Examples of using the comma operator in C.
*
*******************************************************************************/


// here commas are used as a separator, not an operator, to separate the
// parameters of the function
int function(int a, int b, int c) { return a + b + c; }

// a function that returns an error status AND sets the global errno to a
// specific value for the type of error that took place
int check_value(int value)
{
  // do some work

  // use comma operator to set errno to an invalid argument error code AND
  // return a more general "-1" error status to the calling environments
  if (value > 1000) return (errno = EINVAL, -1); // -1 will be returned

  return 0;
}

int comma_operator(void)
{
  // declaring variables, initializing arrays, and calling functions with
  // multiple arguments all involve using the comma as a *separator* not
  // an operator
  /*
  int a = 0, b = 0, c = 0;
  int array[] = {1,2,3,4,5};

  function(3,4,5);
  */

  int a = 1;
  int b = 2;
  int c;

  // Here we use the comma as an operator, it separates expressions somewhat
  // like ; separates statements... the expression 'a' will be evaluated (to 1)
  // but than discarded, and then the expression 'b' will be evaluated (to 2)
  // and then returned, so c will be set to 2.
  c = (a,b); // c = b;

  // the comma has the lowest precedence of all operators in C, so the below
  // code will actually assign 1 to c as the c = a assignment operator will
  // evaluate first, leaving the expression b to evaluate to 2 afterwards
  // c = a, b;

  // print out c and it will be 2
  printf("c: %d\n", c);


  int m = 4;
  int n;

  // the expressions will evaluate sequentially, so before the 2nd expression
  // m * 2 executes, the first m += 1 will have executed and m will be 5, thus
  // m * 2 will evaluate to 10 which will be assigned to n
  n = (m += 1, m * 2); // m will be 5, n will be 10

  // m will be 5, n will be 10
  printf("m: %d\nn: %d\n", m, n);

  int number = 5;
  int max = 10;
  int r1, r2;

  // we can use the comma operator to shorten our code, here we do in 2 lines
  // what could take 10
  if (number < max) r1 = 1, r2 = 2;
  else r1 = 3, r2 = 4;

  // alternative longer way of conducting the same work as above
  /*
  if (number < max)
  {
    r1 = 1;
    r2 = 2;
  }
  else
  {
    r1 = 3;
    r2 = 4;
  }
  */

  // r1 is 1 and r2 is 2 as expected
  printf("r1: %d\nr2: %d\n", r1, r2);


  // the comma operator can be useful when initializing variables as part of
  // a for loop, when the initialization of one variable depends on the
  // initialization of the other
  char *s1 = "a string to print from the middle";
  int length, i;

  // both length and the counter variable i need to be initialized, but i's
  // initial value depends on length... so first we initialize length, then
  // we initialize i using length
  for (length = strlen(s1), i = length / 2; i < length; i++)
    printf("%c", s1[i]);
  printf("\n");


  // we can do something similar in an if-statement condition, first assigning
  // to a variable the result of a function call (or some other expression) and
  // then using that variable as part of a condition... remember that the 2nd
  // expression's result is what is returned, so the condition of the if
  // statement is really going to be length < 20
  char *s2 = "some string";
  if (length = strlen(s2), length < 20)
  {
    for (int i = 0; i < length; i++) printf("%c", s2[i]);
    printf("\n");
  }

  // test our function that both returns an 'error status value' AND sets the
  // errno global variable in a single return statement
  int return_value = check_value(1001);
  printf("return value: %d\n", return_value);
  if (errno == EINVAL) printf("errno set by check_value\n");

  return 0;
}


































/*******************************************************************************
*
* Program: Char Array String Vs. Poointer To String Literal
*
* Description: The difference between a string stored in a char array compared
* to a pointer to a string literal in C.  i.e. the difference between
* char s[] = "string"; vs char *s = "string";
*
*******************************************************************************/

#include <stdio.h>

int charArray_vs_charPointer()
{
  // String literals like "abcdef" exist in some place in memory, where
  // exactly is not defined by the C standard.  We can use string literals to
  // initialize a char array to store a string, as we do here.  The char array
  // s1 will be 7 chars long (due to the extra null terminator), and it will
  // be in a place in memory called the stack where we know it is possible
  // to modify the values that are stored there.

  char s1[] = "abcdef"; // constant pointer

  // Change the first char of the char array s1
  s1[0] = 'X'; // we can do this because s1 is a char array

  // Output the modified s1 string which should now be Xbcdef
  printf("s1: %s\n", s1);

  // The string literal "abcdef" will exist *somewhere* in-memory during our
  // program's execution (again we don't exactly know where as it is not
  // defined by the C standard and is up to the compiler).  And s2 is a
  // pointer to the first char of this string literal (which is terminated
  // with a null terminator character).
  char *s2 = "abcdef";
  // const char *s2 = "abcdef"; // pointer to const (pointer to a string literal)

  // Because we've made s2 a pointer to a const char above, the compiler will
  // flag an attempt (like below) to modify what s2 is pointing to as an error.
  // If we did not user the const keyword when declaring s2, the compiler
  // would allow the below statement to be compiled without error.  But we
  // will almost certainly get a runtime error as a result because the
  // modification of a string literal is "undefined" by the C standard.  This
  // does not strictly speaking mean it is not allowed, but as a practical
  // matter the compiler will not support this and we will get a runtime
  // error related to memory acces.  As a result, if we're going to have a
  // pointer to a string literal, we should make sure it's a const char
  // pointer so that we don't accidentally attempt to modify the string.

  // s2[0] = 'X'; // we can't do this because s2 is a pointer to a char

  // The char array s1 is like a constant pointer, so we can't increment
  // s1 to have it point to the next element in the array.

  // s1++; // we can't do this because s1 is a constant pointer

  // s2 is a non-const pointer, so we actually CAN increment s2 to have it
  // point to the next char in the string literal.  Note that s2 is a pointer
  // to a const char, NOT a 'const pointer', there is a difference.
  s2++; // we can do this because

  // Output s2 and we'll get bcdef as s2 has been set to point to the next
  // char in the string literal (i.e. the char 'b' onwards)
  printf("s2: %s\n", s2);

  // We cannot assign a string to s1, we would need to use something like
  // strcpy() in the string.h library to change the string stored in s1
  // s1 = "new string";

  // We actually CAN assign a string literal to s2 because what will happen
  // is that "new string" will be a new string literal in memory, and s2
  // will be changed to point to THIS string literal now.
  s2 = "new string";

  // If we output s2 now we'll get "new string"
  printf("s2: %s\n", s2);

  // If we use the sizeof operator with s1 and s2, we'll get 7 bytes for s1
  // because it stores 7 chars of 1 byte each.  These 7 chars are stored on
  // the stack.  But with s2 we'll get 8 bytes because s2 is REALLY just a
  // pointer on the stack and sizeof will give us the size of that pointer
  // NOT the size of the string literal that it points to.
  printf("sizeof(s1): %d\n", sizeof(s1));
  printf("sizeof(s2): %d\n", sizeof(s2));

  return 0;
}





























// Bitwise Operators in C


int bitwise_operators()
{
  // 9 represented in binary as an unsigned integer
  // 9 = 0000 1001
  unsigned int x = 9;

  // 24 represented in binary as an unsigned integer
  // 24 = 0001 1000
  unsigned int y = 24;

  unsigned int r;

  // The shift left operator shifts the bits over some number of digits to the
  // left, in this case by 2 digits, inserting 0s on the right-hand side.
  // shift left -> add 0s to the right and remove n digits from the left
  // shift left operator (<< n) is equivalent to multiplying by 2^n
  // 36 =  0010 0100
  r = x << 2;
  printf("x << 1: %d\n", r); // 36

  // The shift right operator shifts the bits over some number of digits to the
  // right, in this case by 2 digits, inserting 0s on the left-hand side.
  // shift right -> add 0s to the left and remove n digits from the right
  // 2 =  0000 0010
  r = x >> 2;
  printf("x >> 2: %d\n", r); // 2

  // The one's complement operator flips 1s to 0s and 0s to 1s, when we output
  // the result as an int with %d the result will actually be negative number
  // due to the way C represents negative numbers with what is called 2s
  // complement, see: https://en.wikipedia.org/wiki/Two%27s_complement
  // https://www.youtube.com/watch?v=4qH4unVtJkE&t=581s
  // To get the negative of a number using one's complement we can add 1 to
  // the decimal equivalent of the result of the one's complement operation.
  // To get the one's complement of a number we can use the ~ operator.
  // To get the negative of a number using two's complement we first
  // get the one's complement of the number and then add 1 (0000 1001)
  // to the result of the one's complement operation.
  // -10 = 1111 0110
  r = ~x;
  printf("~x: %d\n", r); // -10

  // Bitwise 'and' operator sets a bit to 1 if both digits in the operands are
  // 1 and 0 otherwise.
  // 8 =  0000 1000
  r = x & y;
  printf("x & y: %d\n", r);

  // Bitwise 'or' operator sets a bit to 1 if either digit in the operands are
  // 1, and 0 otherwise.
  // 25 = 0001 1001
  r = x | y;
  printf("x | y: %d\n", r);

  // Bitwise 'xor' (exclusive or) sets a bit to 1 if either (but not both)
  // digit in the operand is 1, and 0 otherwise.
  // 0 ^ 1 = 1
  // 1 ^ 0 = 1
  // 0 ^ 0 = 0
  // 1 ^ 1 = 0
  // 17 = 0001 0001
  r = x ^ y;
  printf("x ^ y: %d\n", r);

}


























/*******************************************************************************
*
* Program: #if #elif #else  #ifdef #ifndef Conditional Compilation Directive
*
* Description: Examples of using the #if #elif  #else #ifdef and #ifndef preprocessor
* directives for conditional compilation in C.
*
* gcc -E -o out main.c -> to see the preprocessor output
*
*******************************************************************************/

// The preprocessor is the first phase in the compilation of a C program, it
// will operator on preprocessor directives and mostly performs text insertion
// and replacement type operations using these directives.  It will transform
// our source code with these operations into a version of the code that is
// then compiled into an executable program.

// We can define 'constants' (object-like macros) with the preprocessor,  where
// any occurrence of BUFFER_SIZE will then be replaced with the text 100 and
// any occurrence of TOTAL_BUFFERS will then be replaced with the text 10
#define BUFFER_SIZE 100
#define TOTAL_BUFFERS 10

// If we don't define a macro called PLATFORM_CODE, then in the below code where
// we attempt to use the macro PLATFORM_CODE in an #if directive the
// preprocessor will replace it with 0.  Try uncomment the below line and
// changing the PLATFORM_CODE to values like 1,2,5, etc. to see what happens
// with the #if #elif #else directives in the main function.
//
// #define PLATFORM_CODE 4

// The macro VERSION_CODE is set to a char, and we can use this in the condition
// expression of an #if directive
#define VERSION_CODE 'a'

// Define a macro OS to test the define() keyword
#define OS

// Create a function-like macro called func with a parameter x, it will replace
// x with the expression x - 100
#define FUNC(x) x - 100

// The #if directive will include the code beneath it (but before #else) into
// the compiled source code IF the condition is true (i.e. if the condition is
// any value but 0 which is considered false).  Here we use our function-like
// macro defined above... with an argument of 200, the macro will be replaced
// with the text 200 - 100 which evaluates to 100 which is greater than 0!  So
// the VALUE macro will be set to 500.  Try another argument to the
// function-like macro such as 50 to see what happens.
//
#if FUNC(200) > 0
  #define VALUE 500
#else
  #define VALUE 1000
#endif



// Define a macro with #define
#define DEBUG_MODE

// Undefine a macro using #undef, add and remove this below line of code to
// see the effect it has on the program
#undef DEBUG_MODE

// Define a macro WINDOWS
#define WINDOWS

// Set BUFFER_SIZE to 100 based on whether or not LINUX macro is defined
#ifdef LINUX
  #define BUFFER_SIZE 100
#endif

// Set BUFFER_SIZE to 200 based on whether or not MAC macro is defined
#ifdef MAC
  #define BUFFER_SIZE 200
#endif

// Set BUFFER_SIZE to 300 based on whether or not WINDOWS macro is defined
#ifdef WINDOWS
  #define BUFFER_SIZE 300
#endif




int preprocessor_directives()
{


  // If the DEBUG_MODE macro is defined the preprocessor will insert the code
  // printf("Debug mode!\n"); into the source code before it is compiled,
  // otherwise the preprocessor will insert the code
  // printf("Not debug mode 2!\n"); into the source code before it is compiled.
  #ifdef DEBUG_MODE
    printf("Debug mode!\n");
  #else
    printf("Not debug mode 2!\n");
  #endif

  // Insert the printf() statement if the DEBUG_MODE macros is NOT defined
  #ifndef DEBUG_MODE
    printf("Not debug mode!\n");
  #endif

  // Output the size of the buffer char array which will be determined based on
  // which MAC, LINUX or WINDOWS macro has been defined.
  char buffer[BUFFER_SIZE];
  printf("sizeof(buffer): %lu\n", sizeof(buffer));




  // The #if #elif and #else directives work together to insert code into the
  // compiled source code.  First the #if directive expression will be
  // evaluated, if it is true the idented printf() beneath it will be inserted
  // into the compile source code, otherwise the first #elif condition will
  // be checked next.  Notably, if the PLATFORM_CODE macro is NOT defined the
  // preprocessor will replace the macro with the value of 0, which will make
  // the first condition true.  We would need to uncomment the PLATFORM_CODE
  // macro #define directive above to define the PLATFORM_CODE macro to
  // something.  If after evaluating the #if and all #elif directives no
  // condition expressions are true, then the indented printf() following #else
  // will be inserted into the compiled source code.
  #if PLATFORM_CODE == 0
    printf("Platform 0\n");
  #elif PLATFORM_CODE == 2
    printf("Platform 2\n");
  #elif PLATFORM_CODE == 3
    printf("Platform 3\n");
  #else
    printf("Unknown Platform\n");
  #endif

  // Any expression that evaluates to 0 will be considered false, so in the
  // below example '0 is false' will always be output.
  #if 0
    printf("will not print\n");
  #else
    printf("0 is false\n");
  #endif

  // Any expression that does NOT evaluate to 0 will be considered true, so in
  // the below example 'non-zero is true' will always be output.
  #if -1
    printf("non-zero is true!\n");
  #endif

  // We can use char constants in our condition expressions as well, here the
  // VERSION_CODE macro is set to 'a' so we expect this printf() to execute
  #if VERSION_CODE == 'a'
    printf("VERSION_CODE == a\n");
  #endif

  // We can use operators like multiplication, division, addition and
  // subtraction (and some others) in our condition expressions, here we
  // multiply the BUFFER_SIZE by the TOTAL_BUFFERS (100 * 10 = 1000).  We can
  // also use and, or and not operators in our condition expressions as well,
  // so here even though the left operand evaluates to false, we have "or 1"
  // (where 1 being a non-zero value is true), and so "false or true" evaluates
  // to try and we will print out the BUFFER_SIZE value.
  #if BUFFER_SIZE * TOTAL_BUFFERS > 5000 || 1
    printf("%d\n", BUFFER_SIZE);
  #endif

  // We can use defined() to check whether a macro has been defined, this is
  // the same as using the #ifdef directive
  #if defined(OS)
    printf("OS is defined\n");
  #endif

  // OR
  #ifdef OS
    printf("OS is defined\n");
  #endif

  // Print out VALUE, it will be set based on the #if #else directives above
  // the main function.
  printf("Value: %d\n", VALUE);

  return 0;
}




// Predefined Preprocessor Macros Demonstration

// Description: Examples of using the predefined preprocessor macros in C,
//  including to implement program logging.

// When creating large programs made up of hundreds of source code files it
// may be useful to implement logging.  A log function can be used by the
// program to write log messages to the log file at important points in its
// execution, to help trace the execution of the program after it runs if
// there is a bug for example.  If these log messages contain the filename
// and line number at which they were written, this can help us to determine
// the part of our source code we need to examine to understand the bug.
// We'll implement a log function called 'logger' to do this!
void logger(char *msg, char *src, int line);

int predefined_macros()
{
  // The __FILE__ predefined macro will be set to a string of the source code
  // filename in which it is contained
  printf("File: %s\n", __FILE__ ); // basics.c

  // The __DATE__ predefined macro will be set to a string of the date at which
  // the program was compiled
  printf("Date: %s\n", __DATE__ ); // Aug  9 2024

  // The __TIME__ predefined macro will be set to a string of the TIME at which
  // the program was compiled
  printf("Time: %s\n", __TIME__ ); // 12:00:00

  // The __LINE__ predefined macro will be set to the line number of the source
  // code file at which it appears
  printf("Line: %d\n", __LINE__ );

  // The __STDC__ predefined macro will be set to 1 if the compiler conforms to
  // the ANSI C standard
  printf("ANSI: %d\n", __STDC__ );


  // Create a log message 'task 1 done' that is stamped with this source code
  // filename and the line number at which it occurs
  logger("task 1 done", __FILE__, __LINE__);


  // Create a log message 'task 2 done' that is also stamped with this source
  // code filename and the DIFFERENT line number at which it occurs
  logger("task 2 done", __FILE__, __LINE__);

  return 0;
}





// Program: Token-Pasting Operator Demonstration
//  Description: Examples of using the token-pasting operator in C.


// Function-like macro uses ## token-pasting operator to join together tokens,
// prefixing x with 'Big'.
#define print(x) printf("%s\n", Big##x);

// Function-like macro uses ## token-pasting operator to join together command
// name with _instruction to complete the name of the function to call for that
// instruction.  Also uses # stringizing operator to create string literal for
// the name of the instruction (i.e to wrap the name in double quotes).
// The macro is used to initialize an array of Instruction structs.
#define INSTRUCTION(NAME) \
  { #NAME, NAME ## _instruction }

// struct for representing an "Instruction" with a name and function pointer
// to an associated functions
typedef struct
{
  char name[100];
  void (*function) (void);
} Instruction;

// add instruction function
void add_instruction(void)
{
  printf("add instruction");
}

// subtract instruction function
void subtract_instruction(void)
{
  printf("subtract instruction");
}

int main()
{
  // Initialize char arrays BigApple and BigPear
  char BigApple[] = "Big Apple";
  char BigPear[] = "Big Pear";

  // The function-like macro will prefix Apple and Pear with Big to produce
  // the new tokens BigApple and BigPear which can be used to print out the
  // above char arrays using the printf of the function-like macro.
  print(Apple)
  print(Pear)

  // Create an array of Instructions using our function-like macro to
  // help initialize each instruction.  The function like macro prevents us
  // from having to repeat the text 'add' and 'subtract' when initializing
  // each struct as in the below example.
  Instruction instructions[] =
  {
    INSTRUCTION(add),
    INSTRUCTION(subtract)
  };

  // Without the function-like macro and the token-pasting operator we would
  // initialize the Instructions array like this, repeating the 'add' and
  // 'subtract' text.
  /*
  Instruction instructions[] =
  {
    {"add", add_instruction},
    {"subtract", subtract_instruction}
  };
  */

  // Output each Instruction's name nad call it's function to see the result
  for (int i = 0; i < 2; i++)
  {
    printf("\n");
    printf("Name: %s\n", instructions[i].name);
    instructions[i].function();
  }

  return 0;
}






//  Program: #line Preprocessor Directive
// Description: Example of using the line preprocessor directive in C to alter
// the __FILE__ and __LINE__ predefined macros.

int line_directive(void)
{
  // __FILE__ and __LINE__ are predefined macros that will be set to the
  // current source code filename and linenumber be default.

  // This will modify __FILE__ to be "origin.c" and __LINE__ to be 51
  // #line 51 // -> this will only change the line number
  #line 51 "origin.c"
  printf("error @ %s:%d\n", __FILE__, __LINE__); // error @ origin.c:51

  printf("again @ %s:%d\n", __FILE__, __LINE__); // again @ origin.c:53

  // __LINE__ will continue to "increment" from the line where it is
  // altered, so the second call to printf will output a line 2 more than
  // the first call to printf() above it!

  return 0;
}

// We typically use __LINE__ and __FILE__ to output error messages, they
// allow us to associate an error with where it takes place in the source
// code.  But sometimes we write C programs that actually generate other
// C programs, in which case we may want to associate the error message
// with the original C program and not the generated program.
//
// Maybe origin.c generates file.c, and the error on line 9 of file.c
// should *really* be associated with line 51 of origin.c  We can use the
// #line directive to make this association.
//
//  origin.c   ->   file.c
//
//  line 51    ->   line 9 code