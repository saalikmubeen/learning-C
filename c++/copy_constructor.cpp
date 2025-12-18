/*******************************************************************************
 *
 * Program: Copy Constructor
 *
 * Description: Example of creating and using copy constructors in C++.
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// A simple object with no dynamically allocated memory
class BasicNumber
{
public:
  // BasicNumber objects will store a single integer
  int n;

  // Our constructor will initialize the object's integer member variable
  BasicNumber(int set_n)
  {
    n = set_n;
  }

  // The Copy Constructor is a special constructor that initializes the
  // object using another object of the same type.  We set member variable
  // n by *doubling* the other object's n member variable.  If we do not create
  // our own custom copy constructor, C++ will automatically create a default
  // copy constructor that will set the member variables of the one object to
  // the value's of the other object's member variables (a member-wise
  // assignment).  We call this a "shallow copy" of the object, and it may
  // become a problem is our objects use dynamically allocated data (BasicNumber
  // does not use dynamically allocated data, this copy constructor is meant
  // to illustrate the concept).
  BasicNumber(const BasicNumber &basic_number)
  {
    n = 2 * basic_number.n;

    cout << "Copy constructor called: " << n << endl;
  }
};

// The Number object has a member variable that points to dynamically
// allocate data, and we provide a copy constructor to perform a deep copy
// instead of a shallow copy so that we don't run into problems!
class Number
{
public:
  // the member variable is a *pointer* to an int value, it will store the
  // memory address of an int value
  int *n;

  // Our own constructor
  Number(int set_n)
  {
    // dynamically allocate space for an int on the heap, have n store the
    // memory address of this int
    n = (int *)malloc(sizeof(int));

    // dereference the pointer n to set the value at the memory address that
    // n stores to set_n
    *n = set_n;
  }

  // The copy constructor makes a DEEP COPY of the object by using another
  // Number object.
  // If we relied on the default copy constructor, it would perform a SHALLOW
  // COPY of the object, which would be a problem because the object uses
  // dynamically allocated memory.  The default copy constructor would copy
  // the memory address that n is pointing to, but not the value that n is
  // pointing to.  This would cause both objects to point to the same memory
  // address, and when one object is deleted it would free the memory that the
  // other object is still pointing to, causing a crash.
  Number(const Number &otherNumber)
  {
    // Similar to the constructor above, dynamically allocate space for an int.
    // This is key to making a "deep copy" of the object... we're going to
    // give this object it's own space on the heap, that it's member variable
    // n will then point to.
    n = (int *)malloc(sizeof(int));

    // We assign to the int on the heap that n is pointing to, the value that
    // the other object's pointer member variable n is pointing to, completing
    // the deep copy.  We've allocated space for the value, now we store into
    // that space the same value that the other object has.
    *n = *(otherNumber.n);
  }

  // When a Number object is deleted, we'll free the dynamically allocated
  // memory.
  ~Number()
  {
    free(n);
  }

  // returns the int value that n points to
  int get()
  {
    return *n;
  }
};

int main()
{
  // Example 1 - Copy Constructor

  // Create BasicNumber object using our constructor (not the copy constructor)
  BasicNumber num1(7);

  // Output the int that num1 stores
  cout << "num1: " << num1.n << endl;

  // Creates object num2 by using the copy constructor we've created.
  BasicNumber num2 = num1;

  // When we output the int that num2 stores, we'll find it's double the int
  // that num1 stores due to our copy constructor doubling the value before
  // assigning it!
  cout << "num2: " << num2.n << endl;

  // Create two more BasicNumber objects using our constructor (not the copy
  // constructor).
  BasicNumber num3(5);
  BasicNumber num4(10);

  // This statement below should not be confused with object initialization,
  // which is one of the places the copy constructor will run.  This statement
  // below is the assignment operator, which is a different type of operation
  // that does not run our copy constructor.  The copy constructor we create
  // may also run in some other instances, such as when a function returns an
  // object as a value, or when a function receives the object as an argument
  // when the parameter is passed by value.
  num3 = num4;

  // Example 2 - Copy Constructor To Perform Deep Copy

  // Create a number object using the constructor (not the copy constructor)
  Number numA(8);

  // Output the value that numA's pointer member variable points to
  cout << "numA: " << numA.get() << endl;

  // Create numB using the copy constructor
  Number numB = numA;

  // Output the value that numB's pointer member variable points to
  cout << "numB: " << numB.get() << endl;

  // If we modify the value that numA's pointer member variable points to,
  // we'll find it has no effect on numB *because* we've implemented a
  // deep copy
  *(numA.n) = 20;

  // Output the values that numA and numB's pointer member variables are
  // pointing to, and we'll find they're different!
  cout << "numA: " << numA.get() << endl;
  cout << "numB: " << numB.get() << endl;

  // JUST FOR FUN...  :-)
  //
  // Try deleting the copy constructor we defined in the Number class! If you
  // do, the default copy constructor C++ gives objects will be used instead
  // in the above code.  The default copy constructor does a shallow copy of
  // the object... assigning numA.n's pointer value (a memory address) to
  // numB.n.  The problem is that both numA.n and numB.b will store the SAME
  // memory address... i.e. they are referring to the SAME int value in
  // memory.  This will have multiple consequences... in the above code when
  // we change the value that numA.n is pointing to to 20, it will cause
  // numB.n to point to 20 as well because numA.n and numB.n are storing the
  // same memory address!  The program will also crash when the destructors
  // for numA and numB BOTH run.  When numA's destructor runs it will free
  // the dynamically allocated memory... but then when numB's destructor runs
  // and it attempts to free the SAME memory that has already been freed it
  // will cause our program to crash!

  return 0;
}

// **********************************************************************************

/******************************************************************************
 *
 * Program: Object Assignment
 *
 * Description: Demonstration of how object assignment works in C++, including
 * an example of why we need to be careful with object assignment if our objects
 * contain member variables that are pointers to data on the heap.
 *
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// A simple object with a single public int member variable
class Simple
{
public:
  int x;
};

// An object with a member variable that is itself an object
class Stack
{
public:
  Simple simple;
};

// An object with a member variable that is a pointer to data on the heap
class Heap
{
public:
  Simple *simple;

  Heap(int set_x)
  {
    simple = new Simple;
    simple->x = set_x;
  }
};

int main()
{
  // See the visualizations of memory in the comment below the main function
  // for these three examples...

  // Example 1
  //
  // Assign an object to another object.
  //

  // create two Simple objects, assign a value to member variable x
  Simple simpleA;
  simpleA.x = 4;
  Simple simpleB;
  simpleB.x = 0;

  // when we assign simpleA to simpleB, the value of member variable x for
  // simpleA will be assigned to the value of the member variable x for
  // simpleB, we call this a "shallow copy"
  simpleB = simpleA;

  // we'll find that both x values are the same (4)
  cout << "simpleA.x: " << simpleA.x << endl;
  cout << "simpleB.x: " << simpleB.x << endl;

  // if we alter the member variable x of simpleB, this will not effect the
  // member variable x of simpleA, as they are two distinct objects in
  // memory
  simpleB.x = 20;
  cout << "simpleA.x: " << simpleA.x << endl;
  cout << "simpleB.x: " << simpleB.x << endl;

  // Example 2
  //
  // Assign an object to another object when the object's have member variables
  // that are themselves objects.
  //

  // Create two Stack objects, both of which have Simple object's as member
  // variables... assign to the x member variable of these Simple objects
  Stack stackA;
  stackA.simple.x = 4;
  Stack stackB;
  stackB.simple.x = 0;

  // When we assign stackA to stackB, an assignment operation occurs with the
  // Simple object member variables, and then as in Example 1 the member
  // variable x value is assigned
  stackB = stackA;
  cout << "stackA.simple.x: " << stackA.simple.x << endl;
  cout << "stackB.simple.x: " << stackB.simple.x << endl;

  // Again if we modify the x member variable of the Simple object of stackB,
  // we're going to find this does not modify the x member variable of the
  // Simple object of stackB... because these are all distinct objects in
  // memory... altering one does not effect the other.
  stackB.simple.x = 20;
  cout << "stackA.simple.x: " << stackA.simple.x << endl;
  cout << "stackB.simple.x: " << stackB.simple.x << endl;

  // Example 3
  //
  // Assign an object to another object when the object member variables
  // include pointers to data on the heap.
  //

  // Create two heap objects, the constructor for heap objects will create
  // a Simple object and assign to the Simple object pointer member variable
  // "simple" the memory address for this object on the heap.
  Heap heapA(4);
  Heap heapB(0);

  // When we assign heapA to heapB, just as in the above two examples,
  // the values of the member variables of heapA will be assigned to the
  // member variables of heapB.  BUT the member variable simple in this
  // case is a POINTER that stores a memory address for Simple objects on
  // the HEAP.  So heapB's simple pointer member variable will store the
  // SAME memory address as heapA's simple pointer member variable after
  // the assignment... in other words, they will be pointing to the same
  // object on the heap after this assignment!
  heapB = heapA;

  // Both x values will output as '4' which may give us the impression that
  // we're getting the same behaviour as Examples 1 and 2, but what's really
  // going on this time is that heapA and heapB's simple pointer member
  // variable are both pointing to the SAME Simple object.
  cout << "heapA.simple->x: " << heapA.simple->x << endl;
  cout << "heapB.simple->x: " << heapB.simple->x << endl;

  // When we alter the x value of the object that heapB's simple pointer
  // member variable is pointing to, we modify the object that BOTH heapA
  // and heapB are now pointing to!  So we'll get the same result (20) as
  // output for "both" x values (really they are the same object).
  heapB.simple->x = 20;
  cout << "heapA.simple->x: " << heapA.simple->x << endl;
  cout << "heapB.simple->x: " << heapB.simple->x << endl;

  return 0;
}

//
//
//   Example 1 Memory View:
//
//                                               STACK
//                                               =====
//
//    ------------       ------------
//   | simpleA    |     | simpleB    |
//   |            |     |            |
//   | int x = 4  |     | int x = 20 |
//    ------------       ------------
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//                                               HEAP
//                                               ====
//
//                Nothing
//
//
//
//
//
//   Example 2 Memory View:
//
//                                               STACK
//                                               =====
//
//    ---------------       ---------------
//   | stackA        |     | stackB        |
//   |               |     |               |
//   | Simple simple |     | Simple simple |
//    --------- | ---       --------- | ---
//              |                     |
//              |                     |
//          ------------       ------------
//         | simple     |     | simple     |
//         |            |     |            |
//         | int x = 4  |     | int x = 20 |
//          ------------       ------------
//
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//
//                                               HEAP
//                                               =====
//
//                     Nothing
//
//
//
//
//
//  Example 3 Memory View:
//
//                                               STACK
//                                               =====
//
//    ----------------       ----------------
//   | heapA          |     | heapB          |
//   |                |     |                |
//   | Simple *simple |     | Simple *simple |
//    --------- | ----       --------- | ----
//              |                     /
//              |           ----------
//              |          /
// ~~~~~~~~~~~~ | ~~~~~~~ / ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//              |        /
//              |       /                        HEAP
//              |      /                         =====
//         ------------        ------------
//        | simple     |      | simple     |
//        |            |      |            |
//        | int x = 20 |      | int x = 0  |
//        ------------         ------------
//
