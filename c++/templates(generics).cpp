/*******************************************************************************
 *
 * Program: Function Templates Demonstration
 *
 * Description: Example of using function templates in C++ to reduce code
 * duplication.
 *
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// This template prefix allows us to create a function template find_max, with
// a single type parameter T which we can also call a template variable.  When
// we call find_max in our program, the compiler will at compile time create
// and use a type specific version of the find_max function called a template
// function (so if we call find_max with ints, T will be replaced with int, if
// we call find_max with double T will be replaced with double).  The find_max
// function will find the maximum of a and b.
template <typename T> // or template<class T>
T find_max(T a, T b)
{
  if (a > b)
    return a;
  else
    return b;
}

// We can create function templates with multiple type parameters by seperating
// them by commas.  Here we return the max size in bytes of two parameters a
// and b using two different type parameters T1 and T2.
template <typename T1, typename T2>
int find_max_size(T1 a, T2 b)
{
  if (sizeof(a) > sizeof(b))
    return sizeof(a);
  else
    return sizeof(b);
}

int main()
{
  // create two doubles to find a max
  double x = 10.6;
  double y = 5.3;

  // Use our function template find_max and explicitly provide 'double' as an
  // argument for the template parameter T.  The compiler will create a version
  // of the find_max function specific to the type double and use it here.
  double max_double = find_max<double>(x, y);

  // verify the correct maximum double value is returned
  cout << "max_double: " << max_double << endl;

  // create two ints to find a max
  int m = 5;
  int n = 3;

  // Use our function template find_max again, this time with int arguments.
  // Note that if we don't explicitly provide a type like find_max<int>(m,n) it
  // will still work because the compiler can figure out the type needed
  // based on our arguments.
  int max_int = find_max(m, n);

  // verify the correct maximum int value is returned
  cout << "max_int: " << max_int << endl;

  // try max_size below using different arguments like x,m,c,b, etc.
  char c;
  byte b;

  // Find the maximum size of the two arguments provided... any combination
  // of types can be used because we have two type parameters in oru function
  // template find_max_size.
  int max_size = find_max_size(c, b);

  // verify the correct maximum size of the two arguments is output
  cout << "max_size: " << max_size << endl;

  return 0;
}

// At compile time, based on how we use the function template in our function
// calls, the compiler will decide to generate different type-specific versions
// of the function to be used at run-time (called template functions).  The
// process is visualized below. Function Template is used to generate template functions.
//
//
// double x = 10.6                           int m = 5;
// double y = 5.3;                           int n = 3;
//
// find_max(x,y)     <- Function Calls ->    find_max(m,n)
//              \                           /
//               \                         /
//                \                       /
//                 \                     /
//
//                  template <typename T>
//                  T find_max(T a, T b)
//  Function        {
//  Template ->       if (a > b) return a;
//                    else return b;
//                  }
//
//                 /               \
//                /                 \
//               /                   \
//              /                     \
//
// double find_max(double a,       int find_max(int a,
//                 double b)                    int b)
// {                               {
//   if (a > b) return a;            if (a > b) return a;
//   else return b;                  else return b;
// }                               }
//
//                \                  /
//                 Template Functions

/*******************************************************************************
 *
 * Program: Class Templates Demonstration
 *
 * Description: Example of using a class template in C++.
 *
 *
 *******************************************************************************/

// We wish to have a class to represent Arrays and supporting operations, but
// we would like to avoid defining separate classes for different types like
// int, string, etc, and different array sizes.  This would lead to a lot of
// code duplication.  So instead we use a class template.
//
// We use a template prefix with two parameters to define a 'class template'.
// T is a template parameter that we will use by providing a template argument
// in order to create a type-specific version of the class Array.  We'll also
// provide a length int argument to define the length of the array.  These
// arguments will be provided whenever we attempt to use the class template
// to create an object instance.
//
// When we define a class template, and then use it by creating object
// instances, the compiler, at compile time, will create classes by replacing
// the template parameters with the template arguments provided.  So a class
// template defines a family of potential classes, and how we use the class
// template determines which specific classes the compiler will create for
// usage at runtime.
//
template <typename T, int length>
class Array
{
public:
  // Internally our Array class uses an array to store the array data.
  // public member variable array's type and length will be determined by
  // the template arguments provided.
  T array[length];

  // fills the array with value of a type determined by T
  void fill(T value)
  {
    for (int i = 0; i < length; i++)
      array[i] = value;
  }

  // returns a reference to the array element of type T at the given index
  T &at(int index)
  {
    return array[index];
  }
};

int main()
{
  // Because we use Array<int, 5> the compiler will create a class for us
  // based on the above class template... replacing T with int and length
  // with 5!  Then at *runtime* the executing program will create an object
  // instance using this class created by the compiler.
  //
  Array<int, 5> intArr;

  // Fill the array with the value 2
  intArr.fill(2);

  // If we output the value of the array element at index 2 we will get 4
  cout << "intArray[4]: " << intArr.at(4) << endl;

  // Again we use the class template and at copmile-time the compiler will
  // create a class by replacing T with string and length with 8 in our class
  // template Array.  Then at runtime the executing program will create an
  // object instance using this class.
  Array<string, 8> strArr;

  // Fill the array with the string "abc"
  strArr.fill("abc");

  // But set the array element at index 6, the 2nd to last, to "123"
  strArr.at(6) = "123";

  // Output the array element values, all but the 2nd last array element
  // should be "abc" except the 2nd last element should be "123"
  for (int i = 0; i < 8; i++)
    cout << "strArr[" << i << "]: " << strArr.at(i) << endl;

  // Note that because the compiler uses our class template to create specific
  // classes at compile-time, we can't use class template with something that
  // isn't a constant expression for the length parameter.  Here we try to use
  // a variable x that is set at runtime with our class template... but the
  // compiler can't know what 'x' is going to be, so it's just can't use the
  // class template to create a class.  If we uncomment below lines we will
  // see the resulting error.
  //
  // int x;
  // cin >> x;
  // Array<double, x> xArr;

  return 0;
}

/*******************************************************************************
 *
 * Program: Overloading Function Templates
 *
 * Description: Example of how to overload function templates in C++, combining
 * the benefits of function overloading with the benefits of function templates.
 *
 *
 *******************************************************************************/

// Create a function template called sum that accepts two parameters and
// returns a value of type parameter T.  The function will return the sum
// of the two parameters, and so the addition operator + must be supported
// by whatever type argument is provided when we use the function template.
template <typename T>
T sum(T a, T b)
{
  cout << "template sum" << endl;
  return a + b;
}

// Create a function template ALSO called sum (i.e. sum is overloaded) that
// works the same as the above function template, except with 3 parameters.
template <typename T>
T sum(T a, T b, T c)
{
  return a + b + c;
}

// We can continue to overload sum with a non-template function that uses
// two double parameters and a double return value specifically, creating
// a potentially (but not actually) ambiguous situation as to which function
// is called when two double arguments are provided... either the function
// template above that accepts two arguments of some type, OR this non-template
// function that uses type double specifically.
double sum(double a, double b)
{
  cout << "non-template sum" << endl;
  return a + b;
}

int main()
{
  // Either our non-template function OR our 1st two-parameter function template
  // could be used here, so which does the compiler choose?  By default in C++
  // the compiler will run the non-template function when there is an ambiguity
  // like this between a function template and a non-template function.  Notice
  // the "non-template sum" in the output.
  cout << sum(2.0, 3.5) << endl;

  // At compile-time the compiler will determine that we are using the sum
  // function template that accepts three arguments, and that we are using
  // type double arguments, and it will generate a 'template function' using
  // this function template that uses the type double in-place of T.  That
  // function will then be called at runtime when this statement is executed.
  cout << sum(1.0, 1.1, 1.4) << endl;

  // Here when we pass int values to sum, the compiler will use the function
  // template that accepts two arguments as the non-template function accepts
  // double arguments.  As above, the compiler will use the function template
  // to generate at compile-time a type-specific 'template function' that uses
  // the type int, and THAT function will be called at runtime when this
  // statement is executed.
  cout << sum(1, 3) << endl;

  // The compiler will use the function template that accepts 3 arguments, but
  // this time it will create the type-specific template function using int as
  // the type.  We've effectively combined the benefits of function templates
  // AND function overloading as we're able to use both a varying number and
  // type of arguments with sum.
  cout << sum(1, 3, 5) << endl;

  return 0;
}