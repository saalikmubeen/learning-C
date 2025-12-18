/*******************************************************************************
 *
 * Program: this Keyword Use Cases
 *
 * Description: Demonstration of several use cases of the this keyword in C++.
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// declare the Student class which we will define later
class Student;

// declare the create_report function, which will use a pointer to a Student
// object as an argument
void create_report(Student *student);

// Define a simple Student class that will allow us to demonstrate use
// cases of this
class Student
{
public:
  // Student objects will have two member variables for the student name and age
  string name;
  int age;

  // The constructor uses parameters with the same name as the member variables!
  // If we tried to assign: age = age; this would not work and we would have a
  // bug, but we can use the this keyword to access the member variable age
  // instead.  This will allow us to set the member variable, but it also allows
  // use to use a parameter with the same name without issue... which is one
  // potential use case of the this keyword.
  Student(string name, int age)
  {
    // "this" is an implicit argument provided to every non-static member
    // function, including the constructor, and it is a pointer to the object
    // instance for which this member function was called... notice how when
    // we output this (a pointer, i.e. member address) it's the SAME value as
    // when we output the address of student1 in the main function... that's
    // because they're the same thing!
    cout << "    this memory address: " << this << endl;

    // we can use the pointer this in conjunction with arrow -> to access the
    // member variables of the object instance for which the member function
    // is running
    this->name = name;
    this->age = age;
  }

  // We can use this in ANY non-static member function, not just the
  // constructor, as we do here to access the age member variable and increment
  // it by 1
  void increase_age()
  {
    this->age = this->age + 1;
  }

  // We can also access the member functions of the object instance using the
  // pointer this.... here we use this to call the increase_age() member
  // function!
  void increase_and_output_age()
  {
    this->increase_age();
    cout << "age: " << this->age << endl;
  }

  // We call the create_report student function and pass a pointer to the object
  // instance for which the graduate member function was called... i.e. we are
  // essentially having the object "pass a pointer to itself" to a function!
  // This is another use case of the this pointer.
  void graduate()
  {
    cout << "Congratulations!" << endl;
    create_report(this);
  }

  // We can also use the this pointer to implement method chaining... by having
  // member functions return a reference to the object for which they were
  // called, we can chain together a series of member function (i.e. method)
  // calls together as we do in main with set_name and set_age.
  Student &set_name(string name)
  {
    // set the name member variable to the argument provided
    this->name = name;

    // de-reference the pointer to the object for which this member function
    // was called, when used in conjunction with the return type Student& we
    // will be returning a reference to the object for which this member
    // function was called... allowing another member function to be called
    // with the return value
    return *this;
  }

  // Same idea as the above set_name function, except with the age member
  // variable
  Student &set_age(int age)
  {
    this->age = age;
    return *this;
  }
};

// Create's a "report" on a student... we may not actually want to include
// this logic in a member function of the Student class itself depending
// on how we've split up the functionality of our program.  It may be that
// creating reports on a Student is best handled externally from the class
// from an architectural perspective, in which case this approach can be used.
void create_report(Student *student)
{
  cout << student->name << " " << student->age << endl;
}

int main()
{
  // create the student object, the constructor will output the "this" pointer
  // memory address when we do so
  Student student1("Lucas", 20);

  // notice how the memory address of student1 is the same as "this" pointer
  // memory address... that's because that's exactly what the "this" pointer
  // is supposed to be... a pointer to the object instance for which the
  // member function is being called
  cout << "student1 memory address: " << &student1 << endl;

  // increase the student's age and output the age
  student1.increase_and_output_age();

  // Use method chaining to set the student's name and age using seter methods.
  // What makes this work is that this member function call here:
  //
  // student1.set_name("John")
  //
  // Will return a *reference* to the student1 object, which means .set_age(23)
  // can be called with this "return value" and keep working on the same object.
  //
  // Notably becuase both setter methods return a reference to the object for
  // which they were called, we could switch their order and it will still work.
  student1.set_name("John").set_age(23);

  // call the graduate member function that will pass the object instance's
  // pointer to the create_report function to create the report
  student1.graduate();

  return 0;
}

/*******************************************************************************
 *
 * Program: Method Chaining Example
 *
 * Description: Example of implementing method chaining in C++.
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// Basic Rectangle class, but with setter methods that allow for method chaining
class Rectangle
{
public:
  // rectangles will have length, width and color member variables
  int length;
  int width;
  string color;

  // a standard constructor for initializing the member variables
  Rectangle(int set_length, int set_width, string set_color)
  {
    length = set_length;
    width = set_width;
    color = set_color;
  }

  // a standard function for printing out member variable values
  void print()
  {
    cout << "Length: " << length << endl;
    cout << "Width: " << width << endl;
    cout << "Color: " << color << endl;
  }

  // A setter method that sets the color member variable to the value provided
  // as an argument, BUT the method ALSO returns a reference to the object
  // instance for which it was called, allowing for method chaining.
  Rectangle &setColor(string set_color)
  {
    // set the member variable
    color = set_color;

    // this is a pointer to the object instance for which this method is
    // executing, and when we dereference the pointer with * we return a
    // reference to the object instance itself
    return *this;
  }

  // same as above, except for length member variable
  Rectangle &setLength(int set_length)
  {
    length = set_length;
    return *this;
  }

  // same as above, except for width member variable
  Rectangle &setWidth(int set_width)
  {
    width = set_width;
    return *this;
  }
};

int main()
{
  // Create a rectangle object
  Rectangle rectangle1(4, 5, "red");

  // Use method chaining with the setter methods
  //
  // When we call:
  //    rectangle1.setColor("orange")
  //
  // What is returned is a reference to rectangle1, and so then when we run
  // .setLength(10) on this, it's effectively like running:
  //
  // rectangle1.setLength(10)
  //
  // (after having run rectangle1.setColor("orange"))
  //
  // And so on with setWidth(2).
  //
  rectangle1.setColor("orange").setLength(10).setWidth(2);

  // print the rectnagle so we can see the affect of the setter methods
  rectangle1.print();

  // ** Difference between Pointers and References **

  Rectangle rectangle2(4, 5, "red");
  Rectangle *ptr = &rectangle2; // Creates a pointer to the address of rectangle2

  // Key Points :
  //     ptr stores the memory address of rectangle1.You need to dereference the pointer to access or
  //     modify the original object.Usage :

  (*ptr).length = 10;  // Access using dereference operator (*)
  ptr->color = "blue"; // Shortcut: Use `->` for member access

  Rectangle *ptr = nullptr; // Valid
  if (ptr)
  {
    ptr->setLength(10); // Safe check before using a pointer.
  }

  Rectangle rectangle3(4, 5, "red");
  Rectangle &ref = rectangle3; // Creates a reference to rectangle3

  //   Key Points:
  // ref directly refers to rectangle1 (an alias for the object).
  // No need to use * to access the value—it behaves just like the original object.
  // Usage:

  ref.length = 10;    // Modifies rectangle3 directly
  ref.color = "blue"; // Also affects rectangle3
  // Rectangle &ref = nullptr; // INVALID (references cannot be null).

  return 0;
}

// Let's say we have an object...
//
// ObjectType object1;
//
//
// We can call methods like this...
//
// object1.method1();
// object1.method2();
// object1.method3();
//
// - But we repeat object1 three times
//
//
// Method Chaining:
//
// object1.method1().method2().method3();
//
// - object1 is no longer repeated
// - We can read from left to right
//
//
// It's a debate whether method chaining is worth it or not,
// I will post a link to an interesting paper on the subject.
//
// Paper: "An Empirical Study of Method Chaining in Java"
//
// Link: https://static.csg.ci.i.u-tokyo.ac.jp/papers/20/nakamaru-msr2020.pdf
//
// It seems to be useful in situations where we have chains
// of methods that act on an object, e.g. setter methods.
//
// It is used extensively in jQuery, a very popular
// JavaScript library.
//