
#include <iostream>
#include <fstream> // for file input/output
#include <iomanip> // setw, setprecision, setbase etc
// Includes the round function to round a double value
#include <cmath>

#include <array> // for std::array

using namespace std;
// using std::count;

// clang++ basics.cpp -o out

void swap(int *a, int *b)
{
  int temp = *a;
  *a = *b;
  *b = temp;
}
// swap(&a, &b);

// OR

void swap2(int &a, int &b)
{
  int temp = a;
  a = b;
  b = temp;
}
// swap(a, b);

int main()
{

  int a = 10;
  int b = 20;
  int c = a + b;

  // Standard output stream object
  std::cout << "Sum is: " << c << endl;
  return 0;
}

int printToConsole()
{
  // cout is the 'standard output stream object' and the << operator is the
  // stream insertion operator, this statement will output the text "test"
  // to the standard output stream (by default, the console)
  cout << "test";

  // outputs a newline to the console, so that subsequent output will print
  // to the next line
  cout << endl;

  // we MUST output an endl (or \n) to get a newline, as abc123xyz will output
  // all on one line if we execute the below code
  cout << "abc";
  cout << "123";
  cout << "xyz";
  cout << endl;

  // we can also use the special newline character \n to output a newline
  cout << "another line of text\n";

  // we can chain multiple stream insertion operators together in one statement
  cout << "let's learn how to " << "use cout!\n";

  // we can insert multiple endl stream manipulators in a single statement
  cout << "line 1" << endl
       << "line 2" << endl;

  // we can output the value of a variable using the insertion operator
  int x = 20;
  cout << "x: " << x << endl;

  // this is useful for outputing dynamic information that may change as our
  // program is running as part of output, for example an age value that
  // might be calculator somehow or set by the user
  int age = 45;
  cout << "His father is " << age << " years old.\n";

  return 0;
}

int inputFromTerminal()
{

  // prompt the user for an int value
  int x;
  cout << "Enter an integer: ";

  // use cin to store the value the user enters into x... if for some reason
  // it cannot be done, the expression will evaluate to false
  if (cin >> x)
  {
    // if an integer was successfully received, output the value
    cout << "You entered the integer " << x << endl;
  }
  else
  {
    // if there was an error, clear the error, and inform the user
    // they provided invalid input
    cin.clear();
    cout << "Invalid input" << endl;
  }

  // will clear the input stream of any characters, up to either 1000
  // characters or the first newline whichever comes first
  cin.ignore(1000, '\n');

  // variables for rectangle dimensions, area
  double height = 0;
  double width = 0;
  double area = 0;

  // prompt the user for a height and width value
  cout << "Enter height and width (separated by a space):";

  // we can use cin to accept multiple values at once, so inputting a height
  // value and a width value separated by whitespace characters will work,
  // e.g. entering 5     20.5
  cin >> height >> width;
  area = height * width;
  cout << height << " x " << width << " = " << area << endl;

  // clear the input stream of any characters (e.g. a newline)
  cin.ignore(1000, '\n');

  // prompt the user for their name
  string name;
  cout << "Enter your name: ";

  // will only store the string up until the first space character, so a
  // name like 'kevin browne' will only be stored as 'kevin' into name
  // cin >> name;

  // will store the string up until the first newline character into name,
  // so a name like 'kevin browne' will be stored entirely
  getline(cin, name);

  // say hello to the user by using their name  :-)
  cout << "Hello " << name << "!" << endl;

  // Try to read in an int and store it into number.  When we use cin the
  // characters the user enters will end up in an 'input buffer', a temporary
  // place before the values are stored into variables like number.
  int number = 0;
  cout << "number: ";
  cin >> number;

  // After the user enters in a number like '5' and hits enter the input buffer
  // will contain the characters:
  //
  // 5\n
  //
  // And the cin >> number statement will then rip off the character 5 to store
  // the int 5 into the number variable.  This will leave the input buffer with
  // the character:
  //
  // \n
  //
  // The problem is the below attempt to read in a line of input from the user
  // will then fail because getline() stops reading input at the first newline
  // character encountered, and in this case we *already* have a newline
  // character in the input buffer, so getline() stop immediately rather than
  // pausing for user input.
  //
  // We can use cin.ignore(#, xchar) to ignore up to # amount of characters in
  // the input buffer ("ripping them off") or up until the first xchar (where
  // xchar is some character), whichever comes first.  So if we were to run:
  //
  // cin.ignore(1, '\n');
  //
  // This would rip off the newline character from the input buffer, and the
  // getline() would allow the user to enter a string as the buffer would then
  // be empty. But if the user enters something like this:
  //
  // 5----\n
  //
  // where each - represents a space character, then we would need to ignore
  // up to 5 characters in order for the input stream to be blank again!  So
  // we use:
  //
  // cin.ignore(numeric_limits<streamsize>::max(), '\n');
  //
  // where numeric_limits<streamsize>::max() is the maximum number of characters
  // that can possibly occur in an input buffer, ensuring that the buffer MUST
  // be completely cleared after running cin.ignore().
  //
  // If we comment out the below line, notice how the call to getline() fails!
  //
  cin.ignore(numeric_limits<streamsize>::max(), '\n');

  // read in a line of string input from the user, getline will stop reading
  // in characters at the first newline (up to 256 characters)
  char line[256];
  cout << "line: ";
  cin.getline(line, 256);

  // output the number and line values
  cout << endl;
  cout << "number: " << number << endl;
  cout << "line: " << line << endl;

  // We'll try to store the full name into the name string variable
  string name;

  // Prompt the user to enter their full name
  cout << "Enter Full Name: ";

  // If we try to store the string entered into name like this, we'll only store
  // what was entered up until the first space in the string.  So if a user
  // enters a name like "Virat Kohli" only "Virat" will be stored into name!
  //
  // cin >> name;

  // Instead we can use the getline() function by passing cin as the first
  // argument and the name of our string as the second argument (in this case
  // "name").  The getline() function will store the string from user input into
  // the name variable up until the first newline character, i.e. when the user
  // hits the enter key.  This will allow us to store strings with spaces such
  // as "Virat Kholi".
  //
  getline(cin, name);

  // Output the name string so we can see the string was stored successfully
  cout << "Hello, " << name << "!" << endl;

  // There are variations on using getline(), for example...
  //
  // If we are using a char array to store a "C-style string" as below with
  // char name[100], we can use the getline() member function of the cin
  // object to store the string entered into the char array.  We need to pass
  // the char array as the first argument and the size of the char array as
  // the second argument (we could either hardcode a size like 100 or use
  // the sizeof() operator to get the size of the char array).
  //
  /*
  char name[100];

  cout << "Enter Full Name: ";

  cin.getline(name, sizeof(name));

  cout << "Hello, " << name << "!" << endl;
  */

  return 0;
}

// ** Strings **

int strings()
{
  // create a string variable called test1, assign a string literal to it
  string test1 = "abcdefghi";

  // we can output strings using cout and <<
  cout << test1 << endl;

  // output the size of the string (i.e. number of characters)
  cout << "size: " << test1.size() << endl;

  // we can access individual characters at each index in the string with []
  // and .at()
  cout << "test1[1]: " << test1[1] << endl;
  cout << "test1.at(3): " << test1.at(3) << endl;

  // we can modify individual characters in the string with the [] syntax too
  test1[2] = 'C';
  cout << test1 << endl;

  // there are several ways to concatenate and append strings
  test1 = test1 + "jklmnop";
  test1 += "qrstu";
  test1.append("vwxyz");
  cout << test1 << endl;

  // the length() function is a synonym for size(), it returns the same value
  cout << "length: " << test1.length() << endl;

  // we can create more string type variables
  string test2 = "123";
  string test3;

  // we can concatenate string variables, not just string literals
  test3 = test1 + test2;
  cout << "test3: " << test3 << endl;

  // empty() will check if a string is empty (test3 should not be)
  if (test3.empty())
    cout << "test3 is empty" << endl;
  else
    cout << "test3 is not empty" << endl;

  // clear() will make a string empty (test3 should now be empty, i.e "")
  test3.clear();
  cout << "test3: " << test3 << endl;

  // test3 should now be empty!
  if (test3.empty())
    cout << "test3 is empty" << endl;
  else
    cout << "test3 is not empty" << endl;

  // we can convert values like ints and doubles to strings using to_string()
  string test4 = to_string(-10.5);
  cout << test4 << endl;

  // we can convert double values to strings using stod(), other functions
  // exist to convert other types as well
  double number = stod(test4);
  cout << "number: " << number << endl;

  float f = stof("10.5");
  cout << "f: " << f << endl;

  int i = stoi("10");
  cout << "i: " << i << endl;

  // many functions exist for working with strings, for example we can
  // extract a substring from a string using substr, which in this case
  // extracts 4 characters starting from the character at index 2 (i.e. "test")
  string test5 = "A test string";
  string sub = test5.substr(2, 4);
  cout << "sub: " << sub << endl; // "test"

  // we can store use input into a string type variable, for example a name
  string name;
  cout << "Enter name: ";

  // will only store the string up until the first space character, so a
  // name like 'kevin browne' will only be stored as 'kevin' into name because of the cin will stop
  // at the first space character
  // cin >> name;

  // will store the string up until the first newline character into name,
  // so a name like 'kevin browne' will be stored entirely
  getline(cin, name); // stops at the first newline character

  // output the name
  cout << "Name: " << name << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: set Stream Manipulator
 *
 * Description: Demonstration of how to use the setw stream manipulator in C++
 * to output data into fields with a specified character width.
 *
 *
 *******************************************************************************/

int main()
{
  // A basic example of using setw.  setw(5) will cause the next value in the
  // stream to output into a field of width 5 characters.  We can use left/right
  // to modify the alignment of that field, and setfill to set a different
  // fill character than the default space character.
  //
  // Uncomment the below code to try it out (though setting the fill character
  // to * will make the table below output with stars instead of spaces).
  //
  /*
  int area = 123;

  cout << left << setw(5) << setfill('*');
  cout << area << endl;
  cout << endl << 12345 << endl;
  */

  // One practical use case for setw is to output a table of data such that
  // each column has a set, consistent width.  Let's go over an example!

  // Output a table heading
  cout << endl;
  cout << "Rectangle Area Table";
  cout << endl
       << endl;

  // Create a table with 3 columns: length, width and area.  The table will
  // present the rectangle areas for rectangles with the associated lengths
  // and widths.  We'll make the length and width columns 10 characters in
  // width and left-aligned, and the area column 4 characters in width and
  // right-aligned.  We'll create this format using setw and left/right as
  // below, and then we'll use these stream manipulators in exactly the same
  // way to output the DATA for the fields of the table.
  //
  cout << left;
  cout << setw(10) << "Length";
  cout << setw(10) << "Width";
  cout << right;
  cout << setw(4) << "Area";
  cout << endl;

  // As our table is 24 characters in width (10+10+4), output 24 * characters
  // to separate the column headings from the data
  for (int i = 0; i < 24; i++)
    cout << "*";
  cout << endl;

  // go over a range of lengths from 5 to 25 by 5
  for (int length = 5; length <= 25; length += 5)
  {
    // go over a range of widths from 2 to 10 by 2
    for (int width = 2; width <= 10; width += 2)
    {
      // output length and width into left-aligned 10 character width fields
      cout << left;
      cout << setw(10) << length;
      cout << setw(10) << width;

      // output the area into a right-aligned 4 character width field
      cout << right;
      cout << setw(4) << length * width;

      // output end line so the next row of output starts on the next line
      cout << endl;
    }
  }

  // end the program with some new/blank lines
  cout << endl
       << endl;

  // Result:
  // Rectangle Area Table
  // Length    Width      Area
  // **************************
  // 5         2           10
  // 5         4           20
  // 5         6           30

  return 0;
}

/*******************************************************************************
 *
 * Program: setprecision Stream Manipulator
 *
 * Description: Examples of using the setprecision stream manipulator in C++ to
 * format the output of floating point values.
 *
 *
 *******************************************************************************/

int main()
{
  double n = 42.123456;

  // setprecision will limit the number of digits output for the floating
  // point value n to the argument provided (5,4,3,...)
  cout << "5: " << setprecision(5) << n << endl; // 42.123
  cout << "4: " << setprecision(4) << n << endl; // 42.12
  cout << "3: " << setprecision(3) << n << endl; // 42.1
  cout << "2: " << setprecision(2) << n << endl; // 42
  cout << "1: " << setprecision(1) << n << endl; // 4e+01

  cout << endl;

  // When used in conjunction with the stream manipulator fixed, we can have
  // setprecision output a certain amount of *decimal* digits (digits after
  // the decimal).  The stream manipulator fixed is for fixed-point notation.
  cout << "5: " << fixed << setprecision(5) << n << endl; // 42.12346
  cout << "4: " << fixed << setprecision(4) << n << endl; // 42.1235
  cout << "3: " << fixed << setprecision(3) << n << endl; // 42.123
  cout << "2: " << fixed << setprecision(2) << n << endl; // 42.12
  cout << "1: " << fixed << setprecision(1) << n << endl; // 42.1

  cout << endl;

  // We can use scientific to get scientific notation output of the  number n,
  // again with a certain of amount of decimal digits.
  cout << "5: " << scientific << setprecision(5) << n << endl; // 4.21235e+01
  cout << "4: " << scientific << setprecision(4) << n << endl; // 4.2123e+01
  cout << "3: " << scientific << setprecision(3) << n << endl; // 4.21e+01
  cout << "2: " << scientific << setprecision(2) << n << endl; // 4.2e+01
  cout << "1: " << scientific << setprecision(1) << n << endl; // 4e+01

  cout << endl;

  double n1 = 34.2;
  double n2 = 23.456;

  // When using fixed and setprecision together, we will get the provided amount
  // of decimal digits of output even if those digits are 0s.  So in the case of
  // n1 for example we will get 34.200 even though the last 2 digits don't
  // really provide any more information, and that's because setprecision(3)
  // will ensure the number is output with 3 decimal digits.
  cout << "n1: " << fixed << setprecision(3) << n1 << endl; // 34.200
  cout << "n2: " << fixed << setprecision(3) << n2 << endl; // 23.456

  cout << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: setbase Example
 *
 * Description: Example of using setbase to output integers as hexadecimal and
 * octal numbers in C++.
 *
 *
 *******************************************************************************/

int main()
{
  int number = 17;

  // normally integers will output in base 10, i.e. deciaml, the way numbers
  // are typically represented
  cout << "number: " << number << endl;

  // setbase is a stream manipulator, if we pass in the value 8 using it as
  // below future ints output in the stream will be output as base 8 (octal)
  // numbers
  cout << "number: " << setbase(8) << number << endl;

  // if we pass in the value 16 subsequent integers will be outpiut using base
  // 16, i.e. hexadecimal numbers
  cout << "number: " << setbase(16) << number << endl;

  // we can switch it back to base 10 / decimal with setbase(10)
  cout << "number: " << setbase(10) << number << endl;

  // output the numbers 0-32 as decimal, octal and hexadecimal numbers
  for (int i = 0; i <= 32; i++)
  {
    cout << setbase(10) << i << " ";
    cout << setbase(8) << i << " ";
    cout << setbase(16) << i << endl;
  }

  return 0;
}

// ** Files **

int files()
{

  // ** Writing to a file **

  // create the ofstream object
  ofstream outFile;

  // open file.txt using the append flag so content will be appended to the
  // end of any existing content in file.txt instead of overwriting the existing
  // file content which is the default behavior
  outFile.open("file.txt", ofstream::app); // ::app is the append flag

  // check if the file failed to open, fail gracefully if this is the case
  if (outFile.fail())
  {
    cout << "Error opening file." << endl;
    return 1;
  }

  // we use the stream insertion operator to write to the file, as we do
  // when we use cout!
  outFile << "content" << endl;

  // declare and initialize some variables of different types
  double x = 4.5;
  int y = 10;
  string z = "abc";

  // we can output the content of variables as well!
  outFile << x << endl;
  outFile << y << endl;
  outFile << z << endl;

  // close the file, as a best practice when we are done with it
  outFile.close();

  // ** Reading from a file **

  // Uncomment/comment the below code blocks and modify the contents of file.txt
  // in order to test out the different examples of reading content from a file.

  // create an ifstream object
  ifstream inFile;

  // attempt to open file.txt
  inFile.open("file.txt");

  // if file.txt fails to open, fail() will return true, and we'll exit
  // gracefully by telling the user what went wrong and exiting with status 1
  // which is a signal to the shell that something wen't wrong in the execution
  // of our program
  if (inFile.fail())
  {
    cout << "Error opening file." << endl;
    return 1;
  }

  // Assume file.txt contains the text:
  // 4
  //

  // variable to store number in the file
  int number = 0;

  // attempt to read in the number
  inFile >> number;

  // if we attempt to read in another number it will fail and we can also
  // detect this using the fail() function
  /*
  inFile >> number;
  if (inFile.fail())
  {
    cout << "Error file format incorrect." << endl;
    return 1;
  }
  */

  cout << number << endl;

  // Attempt to read in a file with the format:
  // 1.1 1.2 1.3
  // 2.1 2.2 2.3
  // 3.1 3.2 3.3
  //
  // Notice howe can read in multiple values, also notice how we use the
  // eof() function to detect the end of a file... allowing us to process
  // a file with an arbitrary number of lines, where each line has 3
  // double values per line.
  // The stream extraction operator(>>) stops at the first whitespace character
  // it encounters, so it will read in the first double value, then the second,
  // and then the third, and then stop at the newline character.  We can use
  // this to read in multiple values from a file, and process them as needed.
  //

  double x1 = 0, x2 = 0, x3 = 0;

  while (true)
  {
    inFile >> x1 >> x2 >> x3;
    if (inFile.eof())
      break;
    else
      cout << x1 << " " << x2 << " " << x3 << endl;
  }

  // We could also store the values into a 2D array for usage later.  Notice
  // how we use the stream extraction operator IN the while loop condition...
  // it will return true when extraction is successful, and false when it is
  // not (e.g. we reach the end of the file).  This is an alternative way of
  // reading in the values from the file.  Also notice how we increment i
  // in each iteration of the loop to store the values in the next row of the
  // 2D array.
  //

  double a[3][3];
  int i = 0;
  while (inFile >> a[i][0] >> a[i][1] >> a[i][2])
    i++;

  cout << endl;
  for (int j = 0; j < 3; j++)
  {
    cout << a[j][0] << " ";
    cout << a[j][1] << " ";
    cout << a[j][2] << endl;
  }

  // If the file contents are:
  // This is a string.
  //
  // Then if we use the stream extraction operator, only the keyword "This"
  // will be stored into line... because the operator will stop at the first
  // whitespace character.  We can use getline() to read in the entire line
  // including spaces, because it will stop at the first newline in the file.
  //

  string line;
  // inFile >> line; // only "This" will be stored in line
  getline(inFile, line); // "This is a string." will be stored in line
  cout << line << endl;

  // we close the file when we are done with it as a best practice, it also has
  // the effect of free-ing up some memory too
  inFile.close();

  return 0;
}

/*******************************************************************************
 *
 * Program: Namespace Examples
 *
 * Description: Examples of using namespaces in C++.  For more on namespaces see:
 * https://en.cppreference.com/w/cpp/language/namespace
 *
 *
 *******************************************************************************/

// We use namespace std to avoid having to put std:: in front of cout... this
// is generally OK in smaller applications, but in larger and commercial
// applications we would generally avoid this as it includes many names which
// may lead to name conflicts.  This will have the scope of everything defined
// below it in this file.
using namespace std;

// Define a namespace named "raw"... namespaces are named scopes that allow
// us to avoid name conflicts (e.g. two functions with the same name) in the
// same project by defining functions, classes and other things within a
// namespace scope (other namespace scopes are free to have functions, classes,
// etc with the same name).
namespace raw
{
  // Define a print function within the raw namespace that prints a double
  // value it is provided as an argument without any rounding (i.e. 'raw')
  void print(double number)
  {
    cout << "print raw: " << number << endl;
  }
}

// Define a namespace named rounded
namespace rounded
{
  // We have a print function with the same return value and parameters as in
  // the raw namespace, which would cause a name conflict if both functions were
  // defined outside of namespaces.  But as they are defined within separate
  // namespaces, there is no conflict.  This print function uses the round()
  // function to print a rounded version of the double value it is provided as
  // an argument.
  void print(double number)
  {
    cout << "print rounded: " << round(number) << endl;
  }
}

// Define a namespace called inception *within* the rounded namespace.  We could
// also define this namespace by putting:
//    namespace inception { .... }
// inside of the namespace rounded { ... } definition above, but this way is
// nice from the standpoint of code readability.
namespace rounded::inception
{
  // Another version of the print function within this inception namespace
  // within the rounded namespace, this version prints out "print inception"
  void print(double number)
  {
    cout << "print inception: " << round(number) << endl;
  }
}

int main()
{
  // To access a function from within a namespace we can use the scope
  // resolution operator :: as below.  In this case we access the print
  // function defined inside the raw namespace.
  raw::print(25.6789);

  // The using keyword allows us to use a namespace such that we don't need
  // to use the scope resolution operator to access functions.  The statement
  // below has a scope in that we can't just call print() as we do below in
  // another function besides main().
  using namespace rounded::inception;

  // Because of the above statement, we can call the print function inside
  // the inception namespace (which is itself inside rounded) by simply using
  // the function name.
  print(25.6789);

  // We can give namespaces an alias, and then use that alias to access things
  // defined inside the namespace
  namespace rnd = rounded;

  // Use the alias rnd with the scope resolution operator to access the print
  // function inside the rounded namespace.
  rnd::print(25.678);

  return 0;
}

/*******************************************************************************
 *
 * Program: array Class Examples
 *
 * Description: Examples of using the array class in C++.
 *
 *
 *******************************************************************************/

// We can have array class function parameters & pass array objects to functions
void print_array(array<int, 5> arr)
{
  cout << "Array: ";
  for (int element : arr)
    cout << element << " ";
  cout << endl;
}

int main()
{

  int c_style_array[5] = {1, 2, 3, 4, 5};

  // We can use the array class to store a fixed-size array of elements.  The
  // array class is a template class, so we provide the type of the elements
  // in the array (int) and the size of the array (5) as template arguments.

  // Declare and initialize an array class object instance of size 5 to store
  // the int values provided.  The array class is really a template class, so
  // int and 5 are template arguments to use the template class to create an
  // array class specific to this type (int) and size (5).
  array<int, 5> array1 = {9, 8, 7, 6, 5};

  // Use the array index operator (scripting operator) to ouput the array
  // element at index 1
  cout << "array1[1] = " << array1[1] << endl;

  // Use the at member function to output the array element at index 2
  cout << "array1.at(2) = " << array1.at(2) << endl;

  // The front member function will output the first element in the array
  cout << "array1.front() = " << array1.front() << endl;

  // The back member function will output the last element in the array
  cout << "array1.back() = " << array1.back() << endl;

  // The size member function will output the size of the array (in this case
  // 5), notably the compiler will replace the call to .size() with the
  // size of the array as a constant value, so there is no "function call
  // performance overhead" for using the .size() member function.
  cout << "array1.size() = " << array1.size() << endl;

  // The max_size() member function will return the same size as the size
  // member function.  The array class is one of several sequence container
  // classes, and the max_size() member function is more relevant to other
  // sequence container classes.
  cout << "array1.max_size() = " << array1.max_size() << endl;

  // Use the size member function to loop through and output the array elements
  for (int i = 0; i < array1.size(); i++)
    cout << "array1[" << i << "] = " << array1[i] << endl;

  // Declare another array object to store 5 ints
  array<int, 5> array2;

  // Use the fill member function to set all array2 elements to 10
  array2.fill(10);

  // Loop through the array2 elements and output them to confirm all elements
  // have been set to 10
  for (int i = 0; i < array2.size(); i++)
    cout << "array2[" << i << "] = " << array2[i] << endl;

  // Use the swap member function to swap the elements of array1 and array2
  array1.swap(array2);

  // All of the array1 elements will now be set to 10, here we use a range-based
  // for loop to out the array elements to verify this
  cout << "Array1: ";
  for (int element : array1)
    cout << element << " ";
  cout << endl;

  // Output all of the array2 elements to verify they match the old array1
  // elements before the swap
  cout << "Array2: ";
  for (int element : array2)
    cout << element << " ";
  cout << endl;

  // The empty member function will return true if the array size is 0 and false
  // otherwise, so array1 will not be empty
  if (array1.empty())
    cout << "array1 is empty" << endl;
  else
    cout << "array1 is not empty" << endl;

  // Create an empty array
  array<int, 0> array3;

  // Verify that the empty member function can detect that array3 is empty
  if (array3.empty())
    cout << "array3 is empty" << endl;
  else
    cout << "array3 is not empty" << endl;

  // The array class uses a C-style array internally to store the array data,
  // and we can use the data member function to return a pointer to this
  // C-style array.  We can then use pointer notation and/or array notation
  // to directly accesing the array elements.  Here we store the pointer to
  // the C-style array (i.e. the memory address) into ptr.
  int *ptr = array1.data();

  // Use pointer notation to set the element at index 2 to 20
  *(ptr + 2) = 20;

  // Use array notation to set the element at index 3 to 30
  ptr[3] = 30;

  // Output array1 again to verify the above assignments worked correctly
  cout << "Array1: ";
  for (int element : array1)
    cout << element << " ";
  cout << endl;

  // We can also pass array objects to a function
  print_array(array1);

  // The array class allows us to use iterator objects to loop through the
  // array.  The begin member function will return an iterator pointing to the
  // first element in the array, and we increment the iterator until we reach
  // the end of the array (the end member function will return an iterator
  // pointing just past the last element in the array).
  cout << "Array (iterator print): ";
  for (array<int, 5>::iterator it = array1.begin(); it != array1.end(); it++)
  {
    // de-reference the iterator and output the current element it's pointing to
    cout << *it << " ";
  }
  cout << endl;

  // The type of the iterator is complicated in the above loop, we can use the
  // auto keyword instead and the compiler will use a type for 'it' variable
  // based on the initialization of the variable.  This means if we later change
  // the size of the array, we don't need to change this code to match it.  We
  // can use the member functions rbegin and rend to iterate through the array
  // in reverse.  The rbegin, rend, begin and end functions are also available
  // as global functions, which we use below.
  cout << "Array (iterator print): ";
  for (auto it = rbegin(array1); it != rend(array1); it++)
  {
    cout << *it << " ";
  }
  cout << endl;

  // If we compile the program in debug mode, we can detect array out of bounds
  // errors as below, where we try to access the 6th element in an array that is
  // only 5 elements in size.
  //
  // array1[5] = 10;

  // The at member function will also throw an exception if we try to access an
  // element that is out of range.  We can use a try catch block to handle this
  // exception, giving us another way of dealing with this issue.  This is not
  // possible with C-style arrays.
  try
  {
    // try to access an array element that is out of bounds
    array1.at(5);
  }
  // catch the out_of_range exception and output an error
  catch (const out_of_range &orr)
  {
    cout << "ERROR OUT OF RANGE: " << orr.what() << endl;
  }

  return 0;
}

/*******************************************************************************
 *
 * Program: new & delete Operator Examples
 *
 * Description: Examples of using the new and delete operators in C++.
 *
 *******************************************************************************/

// A simple class for representing a student
class Student
{
public:
  // Student objects have a public member variable name
  string name;

  // Student objects have a public member function print that prints the
  // student's name
  void print()
  {
    cout << name << endl;
  }

  // Define a constructor that sets the student name member variable based on a
  // string argument that is provided
  Student(string name) : name(name) {}
};

int main()
{
  // Regular variables in C++ are given space 'on the stack', a place in memory
  // that contains fixed size blocks of data and is 'automatically' managed for
  // the programmer.  The stack works when we know 'how much data' our program
  // will work with at compile-time when we are writing the program, and we
  // can simply create the required variables and/or create arrays that are
  // 'large enough'.
  int x = 4;

  // The new operator will allocate space for data in a different portion of
  // memory called the heap.  Programmers need to manage data on the heap,
  // free-ing the memory when they are done working with it, making it more
  // challenging to work with.  But with the heap we can allocate memory for
  // however much data we need at run-time, as our program is executing.  Here
  // we allocate space for an int on the heap and set the int to 5, the new
  // operator will return a memory address to this int on the heap which
  // we store in the ptr_to_int variable.  The ptr_to_int variable is made a
  // pointer by * and the int preceeding it means that it is a pointer to
  // an int.
  //
  int *ptr_to_int = new int(5);

  // We can use the * operator to 'dereference the pointer', and access the
  // memory address that the pointer stores... in this case we assign the
  // value 5 to this space in memory.  We comment it out because we initialized
  // the value to 5 already.
  //
  // *ptr_to_int = 5;

  // If we output ptr_to_int we will see it is a memory address, if we
  // dereference the pointer ptr_to_int we will have the value 5.
  cout << "ptr_to_int: " << ptr_to_int << endl;
  cout << "*ptr_to_int: " << *ptr_to_int << endl;

  // If we were to use the new operator again to allocate new space for an
  // int, and store the memory address into ptr_to_int, we would have a memory
  // leak.  This is because it would no longer be possible to free the memory
  // on the heap that ptr_to_int was originally pointing to... the memory
  // address would be "lost" and we would have no copy of it anywhere.  This
  // means we could no longer make the memory available again to be used
  // with the delete operator... we call this a "memory leak" because the
  // memory is "gone" in that we can no longer use it.
  //
  // ptr_to_int = new int(10);

  // The delete operator will free the memory on the heap that was allocated
  // by the new operator and make it available to be used by our program again.
  // We use the delete operator when we no longer need the allocated memory.
  delete ptr_to_int;

  // We can also allocate space for a block of memory using [], in this case
  // a block of memory able to store 4 double values.  The block of memory
  // allocated will be contiguous.
  double *array = new double[4];

  // We can access this block of memory like an array of data... in this case
  // setting 4 double values...
  array[0] = 5;
  array[1] = 6;
  array[2] = 7;
  array[3] = 8;

  // We can print out the dynamically allocated array of data as well...
  for (int i = 0; i < 4; i++)
    cout << "array[" << i << "] = " << array[i] << endl;

  // When deleting the block of memory we will need to use [] again as below
  delete[] array;

  // We can dynamically allocate space for objects as well, in this case we
  // store a Student object on the heap.  Notably C++ originated from the C
  // language that makes available the malloc() and calloc() functions to
  // dynamically allocate memory, and these functions are available in C++
  // as well.  The big difference between using malloc() and calloc() vs
  // using the new operator is that the new operator will call the constructor
  // for an object when it allocates space for the object on the heap!
  Student *student = new Student("Mary");

  // We can use the . dot operator to access the member variables and functions
  // of the object on the heap so long as we dereference the pointer to the
  // object with the * operator.  Notably we need to wrap the *student in
  // brackets to ensure the dereference operator is applied FIRST before the
  // . dot operator (due to operator precedence).
  (*student).name = "John";
  (*student).print();

  // We can also use the -> arrow operator to access the member variables and
  // functions of an object on the heap.
  student->name = "Mary";
  student->print();

  // We can free the dynamically allocated memory for the student object on
  // the heap using the delete keyword.  The function free() from the C
  // language is available in C++ as well, but similarly to how the new operator
  // will call an object's constructor, the delete operator will call the
  // object's destructor.
  delete student;

  // Dynamic memory allocation can fail, for example if not enough memory is
  // available or a contiguous block of sufficient size is not available.  We
  // can detect and handle this situation using a try-catch block.
  try
  {
    // Try allocating far more space than is available, the new operator will
    // throw a bad_alloc exception in this case...
    double *big_array = new double[99999999999999];
  }
  catch (bad_alloc &exp)
  {
    // catch the exception and output an error message...
    cout << "bad_alloc caught: " << exp.what() << endl;
  }

  // If we don't want to use exceptions, we can use "nothrow" as below with the
  // new operator and instead of throwing an exception new will return NULL if
  // the dynamic memory allocation fails.
  double *big_again = new (nothrow) double[99999999999999];

  // If the memory was not successfully allocated NULL will be assigned to
  // big_again, so if big_again is NULL we can handle the error (in this case
  // just outputting an error message).
  if (big_again == NULL)
    cout << "Failed to allocate again!" << endl;

  // There is something commonly called the placeholder new operator that allows
  // use to re-use previously allocated memory, and its usage looks like the
  // below.  This is provided to make the reader aware of this concept, it will
  // be covered more fully in a future example.  In the below example, the
  // 2nd double value is created in the same space in memory that was allocated
  // in the 1st statement (the memory address that mydouble stores).
  //
  // double *mydouble = new double(12.2);
  // double *specific = new(mydouble) double(20.5);

  return 0;
}

// A visualization of memory...
//
// STACK
// -----
//
//               Memory
// Variable      Address      Value
//
// x             0x0001       4
// ptr_to_int    0x0002       0x9902
//               ...
//               ...
//               ...
// HEAP          ...
// ----          ...
//               0x9901       5
//               0x9902       10
//               0x9903
//

/*******************************************************************************
 *
 * Program: Switch Statements
 *
 * Description: Examples of using switch statements in C++.  See more detailed
 * documentation on switches in C++ here
 *  https://en.cppreference.com/w/cpp/language/switch
 *
 *
 *******************************************************************************/

int main()
{
  // A typical switch example

  // declare and initialize a value variable to 2
  int value = 2;

  // The switch accepts a 'condition' expression which must evaluate to an
  // integral or enumeration type, which means we can use types like int
  // or char, but not floats or doubles.  It's a bit more complicated than
  // this, see the link above for more details.  We could put something like
  // "value + 1" here as well, and that would also work as it evaluates to
  // an int value.
  switch (value)
  {
  // We have cases with specific matching values.  When the condition matches
  // the value, execution of satements below the case will occur.  Execution
  // of statements will continue until the first 'break' is encountered OR
  // until the end of the switch body is reached.  It is very typical to see
  // a break statement as the final statement after each group of statements
  // after a case as the statements are intended to execute for each case.
  case 1:
    cout << "Case 1 Matches!" << endl;
    break;

  // We can put control structures like if-statements in the switch.  In the
  // below case we do not have a break statement at the end of the group of
  // statements associated with the case.  Execution will thus "fall through"
  // to the statements of the default case, in the situation that case 2
  // matches, executing those statements as well!  We call this fallthrough
  // logic and it is a key feature of switches.  We can *intentionally* use
  // fallthrough logic, so that if one case matches, another case's (or
  // multiple case's) statements are ALSO executed.  Fallthrough logic can
  // however make switch statements prone to logic bugs, as if a programmer
  // forgets the break at the end of a group of statements for a case,
  // unintended behaviour will result, and yet the compiler will not provide
  // an error message as the language allows for this.  This makes the bug
  // more likely to occur by accident, and more difficult to find.
  case 2:
    cout << "Case 2 Matches!" << endl;
    if (value >= 1)
      cout << "Value >= 1" << endl;

  // The statements underneath the default case will execute when no other
  // cases have matched.
  default:
    cout << "Default Case!" << endl;
    cout << "Value: " << value << endl;
  }

  // After the switch is done, control flow will drop below the switch
  cout << "Value after: " << value << endl;

  // Examples of other switch features

  // As of C++17 we can use an initialization statement in our switch like this,
  // typically used to declare and initialize a variable used in the switch.
  // After the semicolon comes the expression/condition for which the switch
  // will try to find a matching case.
  switch (int value = 1; value)
  {
  case 1:
    cout << "Case 1 Matches!" << endl;
    break;

  // Some compilers will allow us to output a warning message when fallthrough
  // logic is detected (i.e. when a break is missing).  This might be helpful
  // for preventing logic bugs, as the compiler could warn us when we forget
  // to use a break.  But if we intentionally wanted to use fallthrough logic,
  // we would not want this warning.  The [[fallthrough]] attribute allows us
  // to supress this warning from the compiler.
  case 2:
    cout << "Case 2 Matches!" << endl;
    if (value >= 1)
      cout << "Value >= 1" << endl;
    [[fallthrough]];

  default:
    cout << "Default Case!" << endl;
    cout << "Value: " << value << endl;
  }

  // In the above example 'value' would have the scope of the switch statement,
  // and so while we could access it IN the switch statement we could not
  // access it outside the switch statement such as here...
  cout << "Value after: " << value << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Exception Handling Examples
 *
 * Description: Examples of using exception handling in C++.  Exception handling
 * allows us to handle unwanted or unexpected events by throwing exceptions to
 * interrupt regular execution and jump to a catch block to handle the exception.
 *
 *
 *******************************************************************************/

// We can create our own custom types of exceptions and throw and catch them,
// here were create a derived class of the exception class
class custom_exception : public exception
{
  // define our own what method to provide a custom string message
  // const noexcept is a C++11 feature that indicates that the function
  // will not throw an exception
  // virtual const char * is a virtual function that returns a pointer to a
  // constant character, and the function is virtual so that it can be
  // overridden by derived classes
  virtual const char *what() const noexcept
  {
    return "Custom Exception";
  }
};

// Functions we define can throw exceptions.  If an exception is thrown inside
// a function called by another function that we call (and so on in a series
// of function calls), the exception will "propogate upwards" through the
// chain of calls until the closest catch block is able to catch it.
void myfunction2()
{
  throw 5.6;
}

// Calls a function which throws an exception.
void myfunction1()
{
  myfunction2();
}

int main()
{
  // string with only 4 characters
  string word = "four";

  // A try block will essentially "listen" for exceptions and allow us to
  // catch and handle the exception with a catch block.  Together we call
  // the blocks a try-catch block.
  try
  {
    // if we try to access the 5th character in the sting an out_of_rnage
    // exception will be thrown
    // cout << word.at(4) << endl;

    // if we try to allocate an int array this large a bad_alloc exception
    // will be thrown
    // int *array = new int[9999999999999999];

    // we can throw our own custom exceptions
    // throw custom_exception();

    // we don't need to throw an exception-type object, we can throw any
    // value such as an int or a double
    // throw 20;
    // throw 5.6;

    // If we call a function that either throws an exception or calls a
    // function which throws an exception (and so on through a series of
    // function calls), then we can catch the exception.
    myfunction1();
  }
  // Catch block that will match and handle out_of_range exception.  The
  // object e represents the exception, and the what() method may provie
  // additional information about the exception.
  catch (out_of_range &e)
  {
    cout << "First Catch: " << e.what() << endl;
  }
  // We can have multiple catch blocks, and the first catch block to match from
  // top to bottom is the catch block that will execute.  Here we catch a
  // bad_alloc type exception.
  catch (bad_alloc &e)
  {
    cout << "Second Catch: " << e.what() << endl;
  }
  // The exception type is the most general exception-type object, the base
  // class of an inheritance hierarchy of exception-type objects.
  // See: https://en.cppreference.com/w/cpp/error/exception
  // Because exceptions like bad_alloc are ALSO of type 'exception' if this
  // catch block were first, those exceptions would be caught by this
  // catch block!  So we need to be mindful of this when ordering our
  // catch blocks.
  catch (exception &e)
  {
    cout << "Exception Caught: " << e.what() << endl;
  }
  // We can throw and catch any type of value, for example here we catch
  // an int.
  catch (int code)
  {
    cout << "Error Code: " << code << endl;
  }
  // The ... is a special "catch all" catch block case that allows to catch
  // ANY type of exception.
  catch (...)
  {
    cout << "Default catch case!" << endl;
  }

  return 0;
}

/*******************************************************************************
 *
 * Program: Command-Line Arguments Example
 *
 * Description: Example of how to use command line arguments in C++.
 *
 *
 *******************************************************************************/

// Command arguments are supplied to our program on the command line, e.g.
//
// ./cmdargs a1 a2
//
// Would run the program "cmdargs" and supply it with the arguments a1 and a2.
// These arguments serve as input to the program, e.g
//
// mkdir video
//
// will make a directory called video.

// To use command line arguments we must include the parameters argc and
// argv.  We can have either char **argv or char *argv[] as the argv parameter
// type because the two are equivalent... when we pass an array to a function
// in C++ we are really passing a pointer to the first element in the array.
// argc is the number of command line arguments (including the program itself,
// so ./cmdargs in the above example).  argv is an array of pointers to
// C strings containing the command-line arguments, beginning with the program
// name... i.e. the above example would result in:
//
//  argc = 3
//
//  argv[0] -> {'.', '/', 'c', 'm', 'd', 'a', 'r', 'g', 's', '\0'}
//  argv[1] -> {'a', '1', '\0'}
//  argv[2] -> {'a', '2', '\0'}
//
// Though notably if we want to have command-line arguments with spaces we can
// use single quotes or double quotes, so for example:
//
// ./cmdargs "a1 a2"         OR       ./cmdargs 'a1 a2'
//
// would result in argc = 2 and argv[1] = {'a', '1', ' ', 'a', '2', \0'}
// In C++ when we pass array to a function we are really passing a pointer to
// the first element in the array, so we can use char *argv[] or char **argv
// as the type of the argv parameter.
int main(int argc, char *argv[]) // or char **argv
{
  // We may want to check to make sure the number of supplied arguments matches
  // what is expected, e.g. if 2 arguments are required then argc should equal 3
  // as argc also includes the program name in the 'count'.
  if (argc == 3)
  {
    // Output each of the 3 strings stored in argv, which will be the program
    // name stored in the first index followed by the two arguments.
    cout << "argv[0]: " << argv[0] << endl;
    cout << "argv[1]: " << argv[1] << endl;
    cout << "argv[2]: " << argv[2] << endl;
  }
  // Output the program with an error message and status if an incorrect
  // number of command line arguments are provided.
  else
  {
    cout << "Exactly 2 arguments required" << endl;
    return -1;
  }

  // We could use argc to access an arbitrary/unknown number of command
  // line arguments... the below loop will have the counter variable i go
  // through string that argv[] points to and output each string.
  for (int i = 0; i < argc; i++)
  {
    cout << "argv[" << i << "] = ";
    cout << argv[i] << endl;
  }

  // We could convert the C-style string to a C++ string object as follows...
  // This will convert the C-style string stored in argv[1] to a C++ string object
  // stored in the string object convert1.
  string convert1(argv[1]);

  // We can still output and use the string
  cout << "Convert1: " << convert1 << endl;

  // But we can use C++ string methods like length
  cout << "Length: " << convert1.length() << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Return Multiple Values From A Function
 *
 * Description: Examples of techniques to return multiple values from a function
 * in C++, including returning a tuple, returning a vector, and using pass by
 * reference.
 *
 *
 *******************************************************************************/

#include <iostream>
#include <tuple>
#include <vector>

// Returns multiple values by returning a tuple object that stores multiple
// values.  Tuples can store a fixed number of values of different types.
// Tuples are available in C++11 onwards.
tuple<int, int, string> tupleReturn1(int a, int b)
{
  // Creates and returns a tuple object
  return make_tuple(a + b, a - b, "returned string");
}

// The same as the above function except we use { } syntax to create the tuple,
// available in C++17 onwards.
tuple<int, int, string> tupleReturn2(int a, int b)
{
  return {a + b, a - b, "returned string"};
}

// If we want to return an unknown number of values of the same type we could
// return a vector, here we return a vector with 3 ints but we could have put
// any number of int values into the vector.
vector<int> vectorReturn()
{
  vector<int> numbers;

  numbers.push_back(1);
  numbers.push_back(2);
  numbers.push_back(3);

  return numbers;
}

// Effectively "returns" two values by using reference variable parameters.
// The reference variables sum and diff will be aliases for whatever variables
// we pass to the function in the calling function, and so if we modify the
// value these variables store we will modify the values of the variables in
// the calling function.  In this way we can return multiple values.  Note
// that the void function type means the function does not officially return
// a value in the conventional way.
void passByReference(int a, int b, int &sum, int &diff)
{
  sum = a + b;
  diff = a - b;
}

int main()
{
  // test variables for function arguments
  int x = 10;
  int y = 4;

  // Test tupleReturn1()

  // Store "return values" in the tuple that tupleReturn1() returns
  int total1;
  int difference1;
  string retstr1;

  // tie() is used to "unpack" the tuple and we'll store the 3 values returned
  // into total1, difference1, and retstr1.  This technique to unpack the tuple
  // will work in C++11 onwards.
  tie(total1, difference1, retstr1) = tupleReturn1(x, y);

  // Output the returned values
  cout << "Total: " << total1 << endl;
  cout << "Difference: " << difference1 << endl;
  cout << "Returned String: " << retstr1 << endl;

  // Test tupleReturn2()

  // In C++17 onwards we can use structured bindings to unpack the tuple, this
  // time storing the values returned in the tuple into total2, difference2
  // and retstr2.  When using 'auto' the C++ compiler will determine the right
  // type to use for these variables.
  auto [total2, difference2, retstr2] = tupleReturn2(x, y);

  // Output the returned values
  cout << "Total: " << total2 << endl;
  cout << "Difference: " << difference2 << endl;
  cout << "Returned String: " << retstr2 << endl;

  // Test vectorReturn()

  // Store the vector of ints returned by the function into returnedVector()
  vector<int> returnedVector = vectorReturn();

  // Output each into value returned as part of the vector
  for (int value : returnedVector)
  {
    cout << value << endl;
  }

  // Test passByReference()

  // Store the "return values" of passByReference()
  int total3;
  int difference3;

  // When we call passByReference() the last two parameters are reference
  // variables, so because we pass in total3 and difference3 the function
  // is given "direct access" to total3 and difference3 in main() and can
  // set them to whatever it wants... allowing it to essentially "return"
  // multiple values.
  passByReference(x, y, total3, difference3);

  // Output the resulting total and difference
  cout << "Total: " << total3 << endl;
  cout << "Difference: " << difference3 << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: fill() Function Examples
 *
 * Description: Examples of using the fill() function in C++ to fill a range with
 * a value.
 *
 *
 *******************************************************************************/

#include <iostream>

// The fill() function is defined inside the algorithm library so we include it
// in order to use the function.
#include <algorithm>
#include <vector>

using namespace std;

int main()
{
  // Declare a vector to store int values, initialize 10 elements to 0.
  vector<int> items(10, 0);

  // Value will store the user input value that all elements in the range will
  // be set to.
  int value = 0;

  // Prompt the user to enter the value and store it into the value variable.
  cout << "Enter Value: ";
  cin >> value;

  // Call the fill function to fill all the elements in the range with the
  // provided value.  The first two arguments define a range from a first
  // element to a last element, and all elements from the first element up to
  // but not including the last element will be set to the value, i.e. the
  // range to be filled is: [first, last).  The begin() member function of the
  // vector will return an iterator pointing to the first element of the
  // vector and the end() member function will return an iterator pointing to
  // an element one beyond the last vector element, so all vector elements
  // will be set to the value provided as the third argument.
  //
  fill(items.begin(), items.end(), value);

  // Notably we can use operators + and - to alter the range, here the range
  // will start at the element 5 deep into the vector and stop at the element
  // 2 from the end of the vector.
  //
  // fill(items.begin() + 5, items.end() - 2, value);

  // Output the vector elements after calling the fill() function to see
  // that the elements in the range have been set to the value provided
  // auto is a C++11 feature that allows the compiler to determine the type
  // of the variable based on the initialization of the variable.
  for (auto item : items)
  {
    cout << item << " ";
  }
  cout << endl;

  return 0;

  // We can fill ranges of any type, including a C-style array as in the
  // example below...
  //
  /*

  // Declare a C-style int array of length 10
  int items[10];

  // Stores the value that all elements in the array will be set to
  int value = 0;

  // Prompt the user to enter the value to set all array elements to, store it
  // into the value variable
  cout << "Enter Value: ";
  cin >> value;

  // Pointer arithmetic is used to define the range as the entire array
  fill(items, items + 10, value);

  // Output the elements to check if they are all set to the value
  for (auto item : items)
  {
    cout << item << " ";
  }
  cout << endl;

  return 0;

  */
}