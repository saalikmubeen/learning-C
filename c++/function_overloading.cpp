/*******************************************************************************
 *
 * Program: Function Overloading Example
 *
 * Description: Examples of using function overloading in C++ to have multiple
 * functions with the same name in the same scope.
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// add function with 2 int parameters
int add(int x, int y)
{
  return x + y;
}

// add function with 3 int parameters... because the number of parameters are
// different than the function above, C++ will allow this
int add(int x, int y, int z)
{
  return x + y + z;
}

// add function with 3 double parameters... because the types of parameters
// are different than the function above, C++ will allow this too!
double add(double x, double y, double z)
{
  return x + y + z;
}

// Even though the NAMES of the parameters are different, we will get an error
// with the below add function because the number and types of parameters are
// identical to the add function above!
/*
double add(double a, double b, double c)
{
  return a + b + c;
}
*/

// Member functions can also be overloaded, as this Add class has two member
// functions called add
class Add
{
public:
  // add function with two int parameters
  int add(int x, int y)
  {
    return x + y;
  }

  // add function with two double parameters, because the type of a parameter
  // is different (in this case, both parameter's types), C++ will allow this
  double add(double x, double y)
  {
    return x + y;
  }
};

int main()
{
  // call the 3 different add functions
  cout << add(10, 5) << endl;
  cout << add(10, 5, 2) << endl;
  cout << add(10.5, 3.2, 4.1) << endl;

  // If we try the below an error will result as it is ambiguous which add
  // function we intend to call.  In the above with the floating-point literal
  // 10.5 it is clear we intend to call the add function with 3 double
  // parameters, but the integer literal 10 makes it unambiguous.
  // cout << add(10,3.2,4.1) << endl;

  // create an add object
  Add add_obj;

  // test out both the 'int' and 'double' add member functions
  cout << add_obj.add(10, 5) << endl;
  cout << add_obj.add(10.5, 4.5) << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Operator Overloading Example
 *
 * Description: Examples of operator overloading in C++.
 *
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// Define a simple Number class
class Number
{

public:
  int n;

  Number(int set_n)
  {
    n = set_n;
  }

  // Overload the + operator, the function header must follow a particular
  // format, and the exact format depends on the operator.  The left operand
  // of the + operator will become the "this" object for which this member
  // function is being called, and the right operand of the + operator will
  // be provided as an argument to the function.  The function is expected
  // to return a Number object.
  Number operator+(const Number &numA)
  {
    // create and return a new Number object with it's n member variable
    // initialized to the sum of the two operand object's n member variables
    return Number(this->n + numA.n);

    // We *could* provide a definition of the overloaded operator that does not
    // make sense, such as always returning a Number object with an n member
    // variable value of 0.  But we should not do this as it will make our
    // program more difficult to understand, we should try to provide
    // definitions that make sense given the operator's standard purpose.
    //
    // return Number(0);
  }

  // Overload the equality operator ==, which is expected to return a bool
  // (given what the equality operator does, this makes sense).  Again the
  // left operand of the equality operator will be the "this" object for which
  // this member function is called, and the right operand of the equality
  // operator will be provided as an argument to this function.
  bool operator==(const Number &numA)
  {
    // return true if the n member variable's of the two objets are equal,
    // and false otherwise
    if (this->n == numA.n)
      return true;
    else
      return false;
  }
};

int main()
{
  // Create two number objects
  Number a(5);
  Number b(10);

  // if we didn't overload the + operator, this would cause an error, but
  // instead our member function runs and c is set to the return value
  // of that function ('a' becomes the 'this object', and 'b' becomes the
  // parameter object).
  Number c = a + b;

  // output the result, we will get c.n: 15
  cout << "c.n: " << c.n << endl;

  // test the overloaded equality operator (case where two objects aren't equal)
  if (a == c)
    cout << "a == c" << endl;
  else
    cout << "a != c" << endl;

  // test the overloaded equality operator (case where two objects are equal)
  Number d(15);
  if (c == d)
    cout << "c == d" << endl;
  else
    cout << "c != d" << endl;

  return 0;
}