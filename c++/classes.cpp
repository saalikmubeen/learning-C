

#include <iostream>
#include <fstream> // for file input/output

using namespace std;

// **Classes and Objects **

// define a class Employee
class Employee
{

  // public access specifier will make the member variables and functions defined
  // below available for access OUTSIDE the class (for example, in the main
  // function below)
public:
  // a public member variable for the employee name
  string name;

  // it's typical to define public "getter and setter" functions to manage
  // access to private member variables such as salary (see below)
  void set_salary(double potential_salary)
  {
    // setter and getter functions may "guard" and "manage" access to the
    // members, for example in this case applying some error handling
    // logic before setting the salary.... where if the provided salary
    // is negative, we default to setting salary to 0
    if (potential_salary < 0)
      salary = 0;
    else
      salary = potential_salary;
  }

  // a "getter" member function to return the salary
  double get_salary()
  {
    return salary;
  }

  // print the employee name and their bonus
  void print_bonus()
  {
    // we can access private member functions such as calculate_bonus within
    // the class
    cout << name << " bonus: " << calculate_bonus() << endl;
  }

  // the private access specifiers restricts these member variables and functions
  // such that they can only be accessed inside the class, and not by the "outside
  // world" (such as the main function)
private:
  // a private member variable for the employee's salary
  double salary;

  // a private member function to calculate the employee's bonus
  double calculate_bonus()
  {
    return salary * 0.10;
  }
};

// define a class Rectangle
class Rectangle
{

  // private members can only be accessed within the class, not outside of it
private:
  // private member variables length, width for rectangle length, width
  double length;
  double width;

  // Member functions can ALSO be private, we cannot access area outside
  // the Rectangle class.  Member functions can return a value as well,
  // as the area function will return a double value.
  double area()
  {
    return length * width;
  }

  // the public access specifier will make these member functions available
  // outside the class (e.g. in the main function below!)
public:
  // member functions can accept arguments, and include default values for
  // parameters too (here w is set to a default of 5 if it is not provided)
  void set_dimensions(double l, double w = 5)
  {
    // member functions can access and use member variables, including
    // private member variables
    length = l;
    width = w;
  }

  // member functions can be defined outside of the class definition so
  // long as we provide a declaration (aka prototype) within the class
  double perimeter();

  // member functions can access other member functions, including private
  // member functions such as area
  void print_area()
  {
    cout << "Area: " << area() << endl;
  }
};

// We use ClassName:: to connect this function definition to our class
double Rectangle::perimeter()
{
  return 2 * (length + width); // we have access to the length and width member variables here
}

// object type for representing cats
class Cat
{

  // cats will have a name, color and favorite toy
private:
  string name;
  string color;
  string favorite_toy;

public:
  // member function for printing out a cat's information
  void print_cat()
  {
    cout << "Name: " << name << endl;
    cout << "Color: " << color << endl;
    cout << "Favourite Toy: " << favorite_toy << endl;
  }

  // constructor with no parameters, we initialize member variables to defaults
  // Default Constructor
  Cat()
  {
    name = "Unknown";
    color = "Unknown";
    favorite_toy = "Unknown";
  }

  // constructor with a single parameter, we use it to intitialize the cat name
  Cat(string n)
  {
    name = n;
    color = "Unknown";
    favorite_toy = "Uknown";
  }

  // constructor parameters can have default values such as the default favorite
  // toy value "Laser Pointer", we can also define constructors outside the
  // Class such as this example, as long as we leave function declaration
  // (i.e. prototype) in the class
  Cat(string n, string c, string ft = "Laser Pointer");
};

// we use the syntax ClassName::ClassName( ... ) to define the constructor
// outside the class...
Cat::Cat(string n, string c, string ft)
{
  name = n;
  color = c;
  favorite_toy = ft;
}

// a simple "Number" class
class Number
{

  // the class has one private member variable, a pointer to a double
private:
  double *number;

public:
  // the constructor
  Number(double num)
  {
    // allocate space for a double value, have the private member variable
    // number pointo that space
    number = (double *)malloc(sizeof(double));

    // dereference the pointer, store the value num at the allocated space
    *number = num;

    // output statements to trace execution of when the constructor is called
    cout << "Constructor executing!" << endl;
    cout << "Number: " << *number << endl
         << endl;
  }

  // Destructor function runs when the object is destroyed, and is generally
  // used to do clean up work such as making free dynamically allocated memory.
  // We put a ~ symbol in front of the class name to define the destructor, and
  // destructors cannot have parameters.
  ~Number()
  {
    // output statements to trace execution of when the destructor is called
    cout << "Destructor executing!" << endl;
    cout << "Number: " << *number << endl
         << endl;

    // free the dynamically allocated memory to make it available again and
    // prevent a memory leak
    free(number);
  }
};

// very basic function that creates a Number object locally
void test()
{
  // Number object is created here, and destroyed when the function completes
  // execution... at this point the destructor is called but it is implicit
  // in the sense that we don't write any statement to explicitly invoke it
  Number six(6);
}

class Animal
{
public:
  // a "regular" member variable, also known as an "instance variable"
  string species;

  // a *class* member variable, also known as a "static member variable", this
  // variable is "attached" to the class and there is one common total variable
  // for the *class*
  static int total;

  // we can initialize a const class variable in the class definition
  const string planet = "Earth";

  // we an increment the class variable as if it were a member variable like
  // this in the constructor, to recognize another Animal object has been
  // created
  Animal(string animal_species)
  {
    species = animal_species;
    total++;
  }

  // we decrement total when an Animal object is deleted with the destructor
  ~Animal()
  {
    total--;
  }
};

// we initialize a non-const class variable outside the class definition using
// the ClassName::variable syntax below
int Animal::total = 0;

// A simple square shape type
class Square
{

private:
  int side_length;

  // static member variables, also know as class variables, are
  // variables of the class rather than specific to an individual
  // object instance
  static int total_squares;

  // static member functions can be private, and only accessible within
  // the class
  static void incrementTotal()
  {
    // static member functions can alter static member variables
    total_squares++;
  }

public:
  Square(int length)
  {
    side_length = length;

    // we can call private static member functions in non-static member
    // functions
    incrementTotal();
  }

  int area()
  {

    cout << "Side Length: " << this->side_length << endl; // this keyword is a pointer to the object instance
    return side_length * side_length;
  }

  // public static member functions are available outside the class definition,
  // such as in the main function
  static int getTotal()
  {

    // cout << "Side Length: " << this->side_length << endl; // this keyword is not available in static member functions
    // side_length = 10; // side_length is not available in static member functions
    return total_squares;
  }
};

// initialize the private member variable
int Square::total_squares = 0;

class Rectangle2
{
public:
  int length;
  int width;
  int area;
  string color;

  // This constructor initializes the length and width member variables, and
  // computes and initializes the area member variable, it does not initialize
  // color which will then be left as an empty string (which may be OK,
  // perhaps it is optional).
  Rectangle2(int l, int w)
  {
    length = l;
    width = w;
    area = length * width;
    cout << "Constructor 1" << endl;
  }

  // This constructor uses constructor delegation to call the above constructor
  // with the l and w values it was provided, and it counts on that constructor
  // to initialize the length, width and area member variables.  After this
  // constructor function call is complete, this constructor's code will
  // execute, and it initializes the color member variable.  This reduces code
  // duplication, as otherwise this constructor would need to initialize these
  // member variables as well, and it would repeat the exact same code to do so!
  Rectangle2(int l, int w, string c) : Rectangle2(l, w)
  {
    // length = l;
    // width = w;
    // area = length * width;
    color = c;
    cout << "Constructor 2" << endl;
  }

  // out the Rectangle object's relevant information
  void print()
  {
    cout << "length: " << length << endl;
    cout << "width: " << width << endl;
    cout << "area: " << area << endl;
    cout << "color: " << color << endl;
  }
};

// A simple Square class
class Square2
{
public:
  // squares will have a length and color
  int side_length;
  string color;

  // member function to print square data
  void print()
  {
    cout << "side length: " << side_length;
    cout << ", color: " << color << endl;
  }

  // the default constructor to initialize a Square object, with no arguments
  Square2()
  {
    side_length = 0;
    color = "black";
  }

  // constructor that uses two arguments to initialize a Square object
  Square2(int set_length, string set_color)
  {
    side_length = set_length;
    color = set_color;
  }

  // constructor that uses a single argument to initialize a Square object
  Square2(int set_length)
  {
    side_length = set_length;
    color = "black";
  }
};

int classes()
{

  // ** Example 1 **
  // create an employee object
  Employee employee1;

  // we can access public member variable name
  employee1.name = "Kevin";
  cout << employee1.name << endl;

  // we can access public member functions as well
  employee1.set_salary(50000);
  cout << "salary: " << employee1.get_salary() << endl;
  employee1.print_bonus();

  // if we try to access private members outside the class, we will get a
  // compiler error as a result
  // employee1.salary = 20000;
  // employee1.calculate_bonus();

  // ** Example 2 **
  // create a Rectangle object instance
  Rectangle rectangle1;

  // use set dimensions to set its length and width members
  rectangle1.set_dimensions(10, 20);

  // use the other public member functions to output the perimeter, area
  cout << "Perimeter: " << rectangle1.perimeter() << endl;
  rectangle1.print_area();

  // create another Rectangle object instance
  Rectangle rectangle2;

  // if we only provide one argument to set_dimensions the default value for
  // parameter w (width) is used
  rectangle2.set_dimensions(5);

  // output the perimeter and area of the 2nd rectangle
  cout << "Perimeter: " << rectangle2.perimeter() << endl;
  rectangle2.print_area();

  // ** Example 3 **
  // the constructor with no parameters will be called
  Cat cat1;

  cout << "Cat 1..." << endl;
  cat1.print_cat();
  cout << endl;

  // the constructor that accepts a single argument will be called
  Cat cat2("Spot");

  cout << "Cat 2..." << endl;
  cat2.print_cat();
  cout << endl;

  // the constructor that accepts 3 arguments will be called, but the default
  // value will be used for the 3rd argument
  Cat cat3("Garfield", "Orange");

  cout << "Cat 3..." << endl;
  cat3.print_cat();
  cout << endl;

  // ** Example 4 **

  // when we create an object on the heap with dynamically allocated memory
  // using 'new', the constructor will be called
  Number *five = new Number(5);

  // when we use delete to destroy the object and free the memory, the
  // destructor will then be called, and in this sense we are more explicitly
  // "calling" the destructor
  delete five;

  // call the test function, which will cause an object to be created and
  // destroyed when the function completes execution
  test();

  // we create an object that will be destroyed when the program terminates,
  // calling the destructor at that time
  Number seven(7);

  // ** Example 5 **

  // when we create Animal objects, the constructor will run, incrementing the
  // class variable total
  Animal *lion = new Animal("lion");
  Animal *tiger = new Animal("tiger");

  // the instance member variables are tied to each object instance and may be
  // difference for each object
  cout << "Lion: " << lion->species << endl;
  cout << "Tiger: " << (*tiger).species << endl;

  // the class member variable is tied to the class... we can access it with
  // ClassName::variable syntax
  cout << "Total: " << Animal::total << endl;

  // each time we make a new Animal object the total will increase
  Animal *bear = new Animal("bear");

  // note that we CAN access the class variable by using an object instance
  // as below, but the "best practice" is to access it using the class name
  // as we do above
  cout << "Total: " << tiger->total << endl;

  // when we delete an object, the destructor is called, decreasing the total!
  delete lion;

  // now the total is only 2
  cout << "Total: " << Animal::total << endl;

  // delete the remaining objects
  delete tiger;
  delete bear;

  // now the total is zero (and notice we can access the class variable even
  // when no objects of the type exist)
  cout << "Total: " << Animal::total << endl;

  // ** Example 6 **

  Square square1(5);

  cout << "square1 area: " << square1.area() << endl;

  // We can access the getTotal() static member function with the syntax
  // ClassName::FunctionName()
  cout << "total squares: " << Square::getTotal() << endl;

  Square square2(10);

  cout << "square2 area: " << square2.area() << endl;

  // We can also access the getTotal() static member function with any specific
  // object instance, but using the ClassName::FunctionName() syntax is a
  // best practice
  cout << "total squares: " << square2.getTotal() << endl;

  // ** Example 7 **

  // When we use the constructor with 3 arguments to create a Rectangle object,
  // notice how "Constructor 1" first outputs followed by "Constructor 2", this
  // is because of constructor delegation!  The constructor that accepts 3
  // arguments FIRST calles the constructor that only accepts 2 arguments, and
  // THEN it executes its own code.
  Rectangle2 rectangle3(5, 10, "red");
  rectangle3.print();

  Rectangle2 rectangle4(5, 10);
  rectangle4.print();

  // ** Example 8 ** (Array of objects)

  // This will initialize the Square2 elements of the array using the default
  // constructor... if we have not defined a default constructor (the
  // constructor with no arguments), then C++ will use an "implicit" default
  // constructor that initializes the member variables of the object in the
  // default way. Implicit default constructor(when we haven't provided a default
  // constructor with no arguments) will initialize the member
  // variables to the default values(it will fill the primitive member variables like
  // int with junk values that's present in the memory location and fill the
  // object member variables like string will be constructed with it's default
  // constructor which is empty string).
  // If we have defined a constructor that accepts 1 or more
  // arguments and we have NOT defined a default constructor then this will
  // actually result in an error!
  // Square2 squares[3];

  // This will use the constructor with one argument to initialize 3 Square
  // objects for each array element.  As if we initialized the array like this:
  // Square2 squares[3] = {Square2(2), Square2(4), Square2(8)};
  // Square2 squares[3] = {2,4,8};

  // We can initialize the array elements using the constructor with two
  // arguments like this:
  // Square2 squares[3] = {Square2(2,"orange"), Square2(4, "red"), Square2(8,"green")};

  // If we provide less than the required amount of values or constructor calls
  // when initializing the array, the default constructor will be called to
  // initialize the remaining array elements.  For example the below code would
  // use the constructor with one argument to initialize the first array element
  // and the default constructor to initialize the remaining two elements:
  // Square squares[3] = {4};

  // will initialize the first element using the single argument constructor,
  // the second element using the two argument constructor, and the third
  // element using the default constructor
  Square2 squares[3] = {2, Square2(5, "red")};

  // print out each square
  for (int i = 0; i < 3; i++)
    squares[i].print();

  return 0;
}

/*******************************************************************************
 *
 * Program: Member Initializer List Examples
 *
 * Description: Examples of how, why, and when to use member initializer lists
 * in C++.
 *
 *******************************************************************************/

// A class with a default constructor AND parametrized constructor
class Major
{
public:
  string name;

  // we initialize member variable name to the value "Undeclared"
  Major() : name("Undeclared")
  {
    // we can still put code in the instructor that will run after the members
    // are initialized, to do other work a constructor may need to do
    cout << "Major Constructor (Undeclared)" << endl;
  }

  // init member variable name to the value of the parameter name
  Major(string name) : name(name)
  {
    cout << "Major Constructor (" << name << ")" << endl;
  }
};

// A class with NO default constructor
class Minor
{
public:
  string name;

  // init member variable name to the value of the parameter name
  Minor(string name) : name(name) {}
};

class Student
{
public:
  string name;
  int start_year;
  int grad_year;

  // We can initialize a const member variable like this, but it will take
  // on the same vaue for every object instance
  // const string id = "abc";

  // If we wish to provide potentially different values for the const member
  // variables of objects, we need to use member initializer lists as below!
  const string id;

  // If a class has a default constructor AND a parameterized constructor, then
  // BOTH the default constructor AND the parameterized constructor will be
  // called to create objects *if* in addition to the below declaration of
  // the member variable major we also initialized it in the constructor
  // body with something like:
  //
  // Major major("something");
  //
  // ONLY if we initialize the object in a member initializer list can we
  // prevent this from happening and only create the object once.  This is
  // important for performance reasons, as otherwise we are calling two
  // constructors when only one is needed!
  // Here the default constructor of Major class will be called in addition to
  // the parameterized constructor if we don't use member initializer list to
  // initialize the major member variable and used the constructor body to
  // initialize the major member variable: this->major = Major(major);
  Major major;

  // We must use member initializer lists to initialize reference member
  // variables like &minor1
  Minor &minor1;

  // If an object member variable only has a parametrized constructor, then
  // we must also initialize the member variable using a member initializer
  // list.
  Minor minor2;

  Student(string name, long int start_year, string major,
          Minor &minor) : // We can use { } in addition to ( ) in member initializer lists, the
                          // difference is that { } will detect type narrowing errors... e.g. if we
                          // uncomment the { } version of start_year's initialization and comment out
                          // the ( ) version of start_year's initialization we will get an error as
                          // int (start_year member variable's type) is a narrower type able to
                          // represent fewer numbers than long int (start_year parameter's type).
                          name{name},
                          //    start_year{start_year},
                          start_year(start_year),
                          grad_year(start_year + 4),
                          id(name.append(to_string(start_year))),
                          major(Major(major)),
                          minor1(minor),
                          minor2(Minor("Physics"))
  {
    cout << "Student Object Constructed!" << endl;

    // Instead of initializing the Major object in the constructor body like below, we
    // can use a member initializer list to initialize it, this will prevent
    // the default constructor from being called in addition to the
    // parameterized constructor, which is more efficient!

    // If we initialize the Major object in the constructor body like this, then
    // the default constructor of the Major object  will be called in addition
    // to the parameterized constructor, which is less efficient!

    // this->major = Major(major); // here the parameterized constructor gets called of Major class
    // this->name = name;
    // this->start_year = start_year;
    // this->grad_year = start_year + 4;
    // this->id = name.append(to_string(start_year)); // this will not work as id is a const member variable and we have to initialize it in the member initializer list
    // this->minor2 = Minor("Physics"); // this will not work as minor2 object doesn't have a default constructor but only a parameterized constructor so we have to initialize it in the member initializer list
    // this->minor1 = minor; // this also won't work as minor1 is a reference member variable and we have to initialize it in the member initializer list
  }
};

// When a derived class has a base class with no default constructor, we need
// to use a member initializer list to use the base class constructor
class MatureStudent : Student
{
public:
  int age;

  // We use the member initializer list to use Student's constructor...
  MatureStudent(int age, string name,
                long int start_year, string major,
                Minor &minor) : Student(name, start_year, major, minor),
                                age(age) {}
};

// So we have to use member initializer list in the following cases:
// 1. When we have a const member variable
// 2. When we have a reference member variable
// 3. When we have an object member variable that doesn't have a default constructor
// 4. When we have a base class that doesn't have a default constructor

int main()
{
  // Test out making a student object, as the code is written now this long int
  // value should not cause a compiler error (though the int value for the start
  // and grad year's will be incorrect due to a type narrowing issue).
  //
  // Everything else should work as expected!
  //
  long int year = 999999999999;
  Minor minor("Economics");
  Student s1("John", year, "Computer Science", minor);

  cout << "Name: " << s1.name << endl
       << "Start Year: " << s1.start_year << endl
       << "Grad Year: " << s1.grad_year << endl
       << "ID: " << s1.id << endl
       << "Major: " << s1.major.name << endl
       << "Minor1: " << s1.minor1.name << endl
       << "Minor2: " << s1.minor2.name << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Defaulted Functions Example
 *
 * Description: Example of using defaulted functions in C++.
 *
 *
 *******************************************************************************/

// A simple class for storing some data with a single public member value
class Data
{
public:
  // public member variable value
  int value;

  // If we define a parameterized constructor in our class, the default
  // constructor that the C++ compiler normally includes in our class will NOT
  // be included in our class!  UNLESS we use a defaulted function as below.
  Data(int set_value)
  {
    value = set_value;
  }

  // We use the '= default;' specifier to create a defaulted function, in this
  // case the default constructor.  We are telling the compiler to explicitly
  // still include the default constructor, despite the fact that we have
  // provided a parameterized constructor above (which would normally cause the
  // default constructor to NOT be included in our class).
  Data() = default;
};

int main()
{
  // Initialize a Data object using the default constructor (with no parameter).
  // If we comment out the defaulted function: "Data() = default;" in the Data
  // class the compiler will produce an error, because without explicitly
  // including the default constructor with the default specifier, the default
  // constructor will NOT be included when we have provided our own
  // parameterized constructor!
  Data data1;

  // set the member variable
  data1.value = 8;

  // output the data object's value member variable
  cout << "data1.value = " << data1.value << endl;

  return 0;
}

// We can use defaulted functions to include the default versions of these
// special member functions:
//
// Constructor
// Destructor
// Copy constructor
// Copy assignment
// Move constructor
// Move assignment

/*******************************************************************************
 *
 * Program: Base Class Access Specifier Examples
 *
 * Description: Demonstration of how the base class access specifier works with
 * inheritance in C++.  The base class access specifier works in conjunction
 * with the base class member access specifiers to determine the access
 * specification of the members in the derived class, in this code we go over
 * the different combinations with examples.
 *
 *
 *******************************************************************************/

//  This table shows which access specifier will result in the derived class for
//  which combination of base class access specifier and member access specifier
//  in the base class.
//
//
//                    *              Member Access Level
//  Base Class        *
//  Access Specifier  *   Private          Protected       Public
//  *****************************************************************
//                    *
//  Public            *   Inaccessible     Protected       Public
//                    *
//  Protected         *   Inaccessible     Protected       Protected
//                    *
//  Private           *   Inaccessible     Private         Private
//
//
//  The base class access specifier will affect how members are inherited by the
//  derived class.  For example, when using the proteted acces specifier when
//  inheriting the base class, the public members of the base class will become
//  protected members of the derived class.  Generally, we use the public
//  specifier. Notably, if no access specifier is provided the default is
//  private.
//

// A simple BaseClass with public, protected and private members
class BaseClass
{
public:
  int _public_member;

protected:
  int _protected_member;

private:
  int _private_member;
};

// A derived class using the PUBLIC base class access specifier
class DerivedClass1 : public BaseClass
{
public:
  void member()
  {
    // private members of the base class will be inaccessible in the derived
    // class
    // _private_member = 20;

    // protected members of the base class will become protected members of
    // the derived class and thus accessible in the derived class
    _protected_member = 30;

    // public members of the base class will become public members of the
    // derived class
    _public_member = 10;
  }
};

// A derived class using the PROTECTED base class access specifier
class DerivedClass2 : protected BaseClass
{
public:
  void member()
  {
    // private members of the base class will become inaccessible in the derived
    // class
    // _private_member = 30;

    // protected members of the base class will become protected members in the
    // derived class
    _protected_member = 20;

    // public members of the base class will ALSO become protected members in
    // the derived class
    _public_member = 10;
  }
};

// A derived class using the PRIVATE base class access specifier
class DerivedClass3 : private BaseClass
{
public:
  void member()
  {
    // private members of the base class will become inaccessible in the derived
    // class
    // _private_member = 30;

    // both protected and public members of the base class will become private
    // members in the derived class
    _protected_member = 10;
    _public_member = 20;
  }
};

// If we make a derived class of DerivedClass3, we find that we cannot access
// _protected_member and _public_member because they were both made private
// members of DerivedClass3 when it inherited them from BaseClass using the
// private base class access specifier!
class NextLevel : public DerivedClass3
{
public:
  void member()
  {
    // _protected_member = 20;

    // _public_member = 30;
  }
};

int main()
{
  // create an instance of DerivedClass1 to confirm the usual behaviours of
  // the members variables based on their access specification
  DerivedClass1 derived1;

  // we can't access the protected member outside of the class, as is the usual
  // case with a protected member
  //
  // derived1._protected_member = 30;

  // we can access a public member outside the class, as is the usual case with
  // a public member
  derived1._public_member = 10;

  // create an instance of DerivedClass2 to confirm the usual behaviours of
  // the members variables based on their access specification
  DerivedClass2 derived2;

  // we can't access the protected member outside of the class, as is the usual
  // case with a protected member
  //
  // derived2._protected_member = 30;

  // _public_member has now been turned into a protected member in DerivedClass2
  // because we used the protected base class access specifier, and so we can
  // no longer access it outside DerivedClass2
  //
  // derived2._public_member = 30;

  // create an instance of DerivedClass3 to confirm the usual behaviours of
  // the members variables based on their access specification
  DerivedClass3 derived3;

  // We cannot access _public_member or _protected_member because when we used
  // the private base class access specifier when inheriting BaseClass it gave
  // both these variabales a private access specification.
  //
  // derived3._public_member = 20;
  // derived3._protected_member = 30;

  return 0;
}

/*******************************************************************************
 *
 * Program: Convert User-Defined Class Type To Primitive Type
 *
 * Description: Demonstration of how to convert a user-defined class type to
 * a primitive type (like double, char, etc.) in C++.
 *
 *
 *******************************************************************************/
#include <iostream>

using namespace std;

// create a simple Grade class
class Grade
{

  // Grade objects will have a single member variable grade intended to be set
  // to values like 92.45, 45.35, etc.
private:
  double grade;

public:
  // constructor for initializing the grade member variable
  Grade(double grade) : grade(grade) {}

  // operator type() allows us to create a function that will return a value
  // of the specified type, defining how object instances will be converted to
  // the type.
  //
  // In this case we define how to convert a Grade object to a double value by
  // simply return the grade member variable's value...
  operator double()
  {
    return grade;
  }

  // In the chase of converting a Grade object to a char, we return a "letter
  // grade" A,B,C,D or F depending on the range of the grade value
  operator char()
  {
    if (grade >= 80)
      return 'A';
    else if (grade >= 70)
      return 'B';
    else if (grade >= 60)
      return 'C';
    else if (grade >= 50)
      return 'D';
    else
      return 'F';
  }
};

int main()
{
  // instantiate a Grade object
  Grade grade1(93.22);

  // assigning the Grade object instance to a double variable will cause the
  // type converstion to occur as we specified with operator, resulting in the
  // value 93.22
  double dbl_grade1 = grade1;
  cout << "dbl_grade1: " << dbl_grade1 << endl;

  // assigning the Grade object instance to a char variable will cause the
  // type conversion to occur as we specified with operator, resulting in the
  // value 'A'
  char chr_grade1 = grade1;
  cout << "chr_grade1: " << chr_grade1 << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Dynamic Polymorphism example
 *
 * Description: Demonstration of how to use the virtual keyword in C++ to
 * implement dynamic polymorphism (i.e. dynamic binding).
 *
 *
 *******************************************************************************/

// A basic class for representing students
class Student2
{
public:
  // student objects will have a name member variable
  string name;

  // a simple constructor to set the name member variable
  Student2(string name) : name(name) {}

  // we use the virtual keyword when creating the member function, this will
  // enable dynamic binding of the print member function when it is called,
  // we can call the print member function a virtual member function
  virtual void print() { cout << name << endl; }
};

// A derived class of the Student class for medical students
class MedicalStudent : public Student2
{
public:
  // medical students will also have a speciality member variable
  string speciality;

  // a constructor to set the name and speciality of the medical student
  MedicalStudent(string name, string speciality) : speciality(speciality), Student2(name) {};

  // we overide the print member function for medical students to also output
  // the student's speciality, because we've used the virtual keyword in the
  // Student2 class when creating the print member function of the base class,
  // we can expect dynamic binding (see below)
  void print() { cout << name << ": " << speciality << endl; }
};

int main()
{
  // An array of pointers to Student2 objects dynamically allocated on the heap,
  // notably the array elements also point to MedicalStudent object instances
  Student2 *students[] =
      {
          new Student2("Kevin"),
          new Student2("Mary"),
          new Student2("Kulvinder"),
          new MedicalStudent("Ali", "Family Medicine"),
          new MedicalStudent("Harry", "Pediatrics"),
          new MedicalStudent("Lisa", "Radiology")};

  // We can loop through the array and call the print member function and
  // exactly *which* print member function (the base class, or derived class)
  // will actually be resolved at *runtime* depending on the type of object
  // that students[i] is pointing to.  This is enabled by the virtual keyword
  // used when defining the print member function in the Student2 class, without
  // using this keyword we would get *static* binding determined at *compile
  // time* instead, and the print member function of the base class would be
  // called for all objects regardless of whether the object is a Student2
  // object (base class) or MedicalStudent (derived class).  This ability to
  // call a different member function depending on the object type at runtime
  // is called dynamic polymorphism or dynamic binding, and it comes with a
  // small performance cost as our program must do some work at runtime to
  // determine which function to call.  It comes with the benefit of code with
  // more dynamic behaviour, while reducing code duplication as we can store and
  // work with objects of different types in one data structure (e.g. an array).
  for (int i = 0; i < 6; i++)
    students[i]->print();

  // free the dynamically allocated objects
  for (int i = 0; i < 6; i++)
    delete students[i];

  return 0;
}

/*******************************************************************************
 *
 * Program: Virtual Destructors Example
 *
 * Description: Demonstration of how to use virtual destructors in C++.
 *
 *
 *******************************************************************************/

// A simple base class that dynamically allocates memory
class BaseData
{
public:
  // data is a pointer to the dynamically allocated memory
  int *data;

  // in the constructor will allocate space for an array of ints on the heap
  BaseData(int size)
  {
    data = new int[size];
  }

  // In the destructor we free the dynamically allocated memory... it's
  // *essential* that the destructor run otherwise we would have a memory leak.
  // Notice how we use the virtual keyword to create a virtual destructor, this
  // means that the destructor of any derived classes will be called using
  // dynamic binding at runtime IF we have a BaseData (base class) pointer to
  // that derived class instance on the heap.  Below we'll create a derived
  // class so we can observe this behaviour.
  virtual ~BaseData()
  {
    cout << "BaseData destructor is running" << endl;
    delete data;
  }
};

// A simple derived class that inherits from our base class BaseData, and also
// dynamically allocates memory
class DerivedData : public BaseData
{
public:
  // a pointer to the dynamically allocated memory
  int *extra_data;

  // Our DerivedData class inherits the member variables and functions of its
  // base class BaseData, including the member variable data.  Below we call
  // the base class constructor with BaseData(size), which means the member
  // variable data will be made to point to dynamically allocated memory, and
  // it will be essential that this memory is freed.  We *also* dynamically
  // allocate additional memory and have the extra_data member variable point
  // to this memory.
  DerivedData(int size, int extra_size) : BaseData(size)
  {
    // dynamically allocate an array of ints of size extra_size and have
    // extra_data point to this data
    extra_data = new int[extra_size];
  }

  // The derived class constructor frees the memory pointed to by extra_data.
  // It does not free the memory allocated by the BaseData constructor, because
  // we expect the BaseData destructor to be called *after* the DerivedData
  // destructor.  This is the "default behaviour" to expect with destructors
  // and inheritance.  The issue is when we have a BaseData pointer to a
  // DerivedData object on the heap... in this case, unless we declare the
  // BaseData destructor using virtual as we have above, *only* the BaseData
  // destructor will run, and the memory allocated in DerivedData will never
  // be freed!  Which means we'll have a memory leak bug in our program.
  ~DerivedData()
  {
    cout << "DerivedData destructor is running" << endl;
    delete extra_data;
  }
};

int main()
{
  // Create an array of BaseData pointers to BaseData and DerivedData objects
  // dynamically allocated on the heap
  BaseData *array[] =
      {
          new BaseData(5),
          new DerivedData(4, 4)};

  // We typically have arrays of base class pointers to allow us to do some work
  // using polymorphism... so we could for example have a loop that would loop
  // through the elements of the above array, calling some member function for
  // each object instance, regardless of whether it is a base class object or a
  // derived class object!  This technique relies on polymorphism and can reduce
  // code duplication.
  //
  // for (int i = 0; i < 2; i++) array[i].method();

  // Now when we're done working with the objects we'll want to free the
  // dynamically allocated memory, so again we use a loop to go through the
  // array of pointers and destroy each object.  The problem is if we *don't*
  // use the virtual keyword when defining the BaseData destructor then in
  // the case of the DerivedData object *only* the BaseData destructor will
  // be called and NOT the DerivedData destructor, leaving the dynamically
  // allocated data of that object (what extra_data is pointing to) still
  // allocated and not freed!  This would be a memory leak.  Try removing the
  // virtual keyword from the BaseData destructor to see the effect.  This is
  // why it's important to use a virtual destructor when defining a class if
  // it is possible the class will be inherited by another class.
  for (int i = 0; i < 2; i++)
    delete array[i];

  return 0;
}

/*******************************************************************************
 *
 * Program: Abstract Class With Pure Virtual Function Example
 *
 * Description: Example of how to use a pure virtual function to create an
 * abstract class in C++.
 *
 *
 *******************************************************************************/

// Shape is a base class for Square and Triangle, and we want to be able to
// use polymorphism to work with arrays of pointers to Square, Triangle and
// other object instances so that we can execute member functions like
// area() to calculate the area of shapes.  The problem is that there is NO
// "generic shape area", the area calculation is really only defined for
// specific types of shapes.  So instead we create a pure virtual function
// area() in our base class shape, which makes the Shape class an abstract
// class (having 1 or more pure virtual member function makes any class a
// an abstrac class).  We then leave it to Square, Triangle and any other
// derived class to override the area member function (if they do not, they
// will also become abstract classes).  Notably, we cannot create an instance
// of an abstract class like Shape, but we CAN have pointers and references
// of the type of an abstract class.
//
class Shape
{
public:
  // The virtual keyword will make the area() function a virtual function that
  // allows for dynamic binding (i.e. runtime polymorphism).  The = 0 assignment
  // will make it a pure virtual function, and makes Shape an abstract class.
  virtual double area() = 0;
};

// Square inherits from Shape, and overrides the area pure virtual function.
// Because Square overrides all of the pure virtual functions of its base
// class, we can create Square object instances, and we would call Square
// a concrete class.
class Square3 : public Shape
{
public:
  double side;

  Square3(double side) : side(side) {}

  double area()
  {
    return side * side;
  }
};

// Triangle also inherits from Shape, and also overrides the area pure virtual
// function with a calculation that works for triangles.
class Triangle : public Shape
{
public:
  double base;
  double height;

  Triangle(double base, double height) : base(base), height(height) {}

  double area()
  {
    return 0.5 * base * height;
  }
};

int main()
{
  // We *cannot* make a Shape object instance because it is an abstract class.
  // So the below code will cause a compiler error if we uncomment it.
  //
  // Shape shape;

  // We CAN create pointers and references of the type of an abstract class,
  // allowing us to create an array of pointers to object instances of
  // derived classes of Shape.
  Shape *shapes[] =
      {
          new Square3(5),
          new Triangle(8, 10),
          new Square3(7),
          new Triangle(3, 4)};

  // Because we have an array of pointers to Shapes, which support dynamic
  // binding of the area() member function, we can use runtime polymorphism
  // to loop through our array of pointers to Shapes and call the area
  // member function for each object!  This allows us to utilize polymorphism,
  // BUT without implementing a "generic Shape" area member function that
  // would not make any sense.
  //
  for (int i = 0; i < 4; i++)
    cout << "Shape " << i << ": " << shapes[i]->area() << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Pure Virtual Destructor Example
 *
 * Description: Example of how to use a pure virtual destructor in C++.  We
 * would use a pure virtual destructor when we want a class to be abstract BUT
 * we also don't want any of the other member functions to be pure virtual
 * member functions (perhaps so derived classes can optionally inherit and use
 * these member function implementations).
 *
 *
 *******************************************************************************/

// Base is an abstract class due to the pure virtual destructor, so we can't
// create object instances of type Base
class Base
{
public:
  // All the other member functions are NOT pure virtiual functions, in this
  // case just the function func(), despite the fact that it is an abstract
  // class due to the pure virtual destructor.  This is a virtual member
  // function so when derived classes overide this member function we will
  // have dynamic binding / runtime polymorphism behaviour.
  virtual void func()
  {
    cout << "Base func()" << endl;
  }

  // Make the destructor a pure virtual destructor
  virtual ~Base() = 0;
};

// While it's possible to provide an implementation for all pure virtual member
// functions this is typically not done.  In the case of a virtual destructor
// we *must* supply an implementation of the function (otherwise our code
// below to delete object instances of derived types will fail when an attempt
// is made to call the base class destructor).  We provide a simple definition
// of the destructor below, we might normally do something like free dynamically
// allocated memory in a destructor to prevent a memory leak.
Base::~Base()
{
  cout << "Base Destructor" << endl;
}

// Derived1 is a derived class of Base, it is a concrete class so we can create
// object instaces of type Derived1
class Derived1 : public Base
{
public:
  // We overide func()
  void func()
  {
    cout << "Derived1 overriding func()" << endl;
  }

  // Typically with pure virtual functions if the derived class does not provide
  // an implementation of all pure virtual functions in the base class then the
  // derived class will ALSO be an abstract class.  But in the case of a pure
  // virtual destructor the default destructor C++ provides to all objects will
  // be provided as an implementation of the destructor, and so despite the fact
  // that we do not explicitly provide an implementation of the destructor
  // ourselves Derived1 will be a concrete class and not an abstract class.
};

// Derived2 is a derived class of Base, and is also a concrete class
class Derived2 : public Base
{
public:
  // We don't override func1() in Derived2, so Derived2 will use func1() as it
  // is defined in the Base class (and inherited from Base).

  // We provide an implementation of the destructor... because the Base class
  // destructor is virtual when an object of type Derived2 is deleted we can
  // expect the Derived2 destructor to be called followed by the Base class
  // destructor.
  ~Derived2()
  {
    cout << "Derived2 Destructor" << endl;
  }
};

int main()
{
  // Despite the fact that all member functions aside from the destructor
  // are NOT pure virtual member functions, because the destructor IS a pure
  // virtual destructor, Base is an abstract class!
  ///
  // So we cannot create an instance of the Base class as below:
  //
  // Base base;

  // We can use Base as a pointer type, here we create an array of pointers of
  // type Base to Derived1 and Derived2 object instances on the heap
  Base *array[] =
      {
          new Derived1(),
          new Derived2()};

  // We use dynamic binding / runtime polymorphism to work with the objects,
  // calling the func() member function of each object.  In the case of
  // the Derived1 object the overridden func1() will be called, in the case
  // of the Derived2 object the func1() defined in the Base class will be
  // called instead.
  for (int i = 0; i < 2; i++)
    array[i]->func();

  // When we delete the object instances, in the case of the Derived1 object
  // the "default destructor" is called before the Base class destructor
  // and so we only see output from the Base class destructor (the default
  // destructor does not have default output).  In the case of the Derived2
  // object the Derived2 destructor is called followed by the Base class
  // destructor.
  for (int i = 0; i < 2; i++)
    delete array[i];

  // So we have an abstract class Base, and yet aside from the destructor none
  // of the member functions are pure virtual member functions, and the
  // derived classes can use these implementations by not overriding them.
  //
  // Normally when we have an abstract class it's because we have a pure
  // virtual member function that is not the destructor, and if our derived
  // classes do not override all the pure virtual member functions they will
  // become abstract classes themselves.  Here our derived classes are still
  // concrete classes whether they overide the destructor or not, and they
  // can utilize the implementation of the member function(s) in the base class.

  return 0;
}

/*******************************************************************************
 *
 * Program: Provide An Implementation Of A Pure Virtual Function Demonstration
 *
 * Description: It's a bit of a "hidden feature" in C++ that we can provide an
 * implementation of pure virtual member function in its own abstract class.  We
 * demonstrate how to do so in the example below.
 *
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// The Logger base class is an abstract class due to the pure virtual member
// function log
class Logger
{
public:
  // class variable total will keep track of the total character length of all
  // log messages made from certain derived class objects
  static int total;

  // pure virtual member function, derived classes will implement this method
  // in different ways to log to the console, files, database, etc. (or so is
  // the intention)
  virtual void log(string message) = 0;

  // destructor
  virtual ~Logger() {};
};

// initialize total to 0 as initially there are no log messages
int Logger::total = 0;

// We typically do not provide an implementation of a pure virtual member
// function in the abstract class, typically it is left to be implemented
// by concrete derived classes.  But C++ does allow us to do this, and if we
// implement the function it can be called too (for example, in the derived
// classes).  Here we have the log member function increment the total class
// variable by the length of the log message so we can keep track of the total
// character length of all log messages from certain derived class objects.
void Logger::log(string message)
{
  total += message.length();
}

// ConsoleLogger is a derived class of Logger, the intention is we could have
// several more derived classes of Logger to implement different types of
// logging (e.g. database, file, etc.)
class ConsoleLogger : public Logger
{
public:
  // logs a log message to the console, but also utilizes the base class
  // implementation of log
  void log(string message)
  {
    // Call the base class Logger log function.  All ConsoleLogger objects
    // will contribute to the total character count kept track of by Logger's
    // total class variable.  The intention is that some derived classes of
    // Logger will utilize the log() implementation in the Logger class and
    // some will not.  Note that if it were a *requirement* for the derived
    // class log member function to call the base class log member function
    // then this would be an instance of the Call Super anti-pattern and
    // we would want to refactor our code to avoid this situation:
    // https://en.wikipedia.org/wiki/Call_super.  Because calling the base
    // class log member function is 'optional' it is not an instance of the
    // anti-pattern.
    //
    Logger::log(message);

    // Output the log message to the console
    cout << "Log: " << message << endl;
  }
};

int main()
{
  // We can't create an instance of the Logger class because it is an abstract
  // class... so the below statement would cause an error:
  //
  // Logger logger;

  // We can create pointers of type Logger which is useful for dynamic binding
  // (i.e. runtime polymorphism), and we can create an instance of the
  // ConsoleLogger object.
  Logger *logger = new ConsoleLogger();

  // Call the log member function of the ConsoleLogger object twice
  logger->log("abc");
  logger->log("1234");

  // Because the log member function of ConsoleLogger calls the Logger base
  // class log implementation, a total of the characters written to the log
  // is kept in the class variable total, which we output below (it should be 7)
  cout << "Total: " << Logger::total << endl;

  // Free the dynamically allocated memory
  delete logger;

  return 0;
}

/*******************************************************************************
 *
 * Program: Friend Functions Examples
 *
 * Description: Examples of how to use friend functions in C++.
 *
 *
 *******************************************************************************/

// A simple class with some private members
class MyClass
{
  // We declare a 'friend function' double_x that will be allowed access to any
  // protected and private members of MyClass objects.  The friend function is
  // NOT a member function of MyClass.  We could put this declaration beneath
  // the public or private access specifiers and it would work the same as it
  // does here, because it is NOT a member of MyClass.
  friend void double_x(MyClass &object);

private:
  // a private member variable x
  int x;

  // a private member function add that adds an int argument provided to x
  void add(int n)
  {
    x += n;
  }

public:
  // a constructor that sets the member variable x to the int argument provided
  MyClass(int x) : x(x) {};

  // A public member function that prints out the value of member variable x...
  // MyClass CAN access private members of MyClass, but the "outside world"
  // such as the main function cannot... friend functions will *also* be able
  // to access private and protected members!
  void print()
  {
    cout << "x: " << x << endl;
  }
};

// We define the friend function double_x and because it is declared a friend
// function inside MyClass (notice the friend keyword is absent here) we can
// access the private and protected members of the MyClass object instance.
void double_x(MyClass &object)
{
  // access the private member variable x, store it into current_x_value
  int current_x_value = object.x;

  // use the private member function add to add the current_x_value to itself,
  // having the effect of doubling x
  object.add(current_x_value);

  // Alternatively we could have accessed the private member variable x directly
  // in order to double it like this...
  //
  // object.x *= 2;
}

// A friend function can also be a friend to multiple classes, here we have an
// example of a friend function that is a friend to both a Revenue1 class and a
// Costs1 class.

// forward declaration of Costs1 so we can use it in the Revenue1 class
class Costs1;

// A very simple class for representing revenue
class Revenue1
{
  // declare friend function profit1
  friend bool profit1(Revenue1 rev, Costs1 cos);

private:
  // private member variable revenue for representing revenue
  int revenue;

public:
  // construct sets the revenue member variable to the int argument provided
  Revenue1(int revenue) : revenue(revenue) {}
};

// A very simple class for representing costs
class Costs1
{
  // we ALSO declare the profit1 function as a friend of Costs1
  friend bool profit1(Revenue1 rev, Costs1 cos);

private:
  // private member variable for representing costs
  int costs;

public:
  // Constructor for setting the costs member variable
  Costs1(int costs) : costs(costs) {}
};

// The profit1() function will have access to the protected and private members
// of BOTH Revenue1 AND Costs1 because both classes declare it as a friend
// function!  We return true if revenue is greater than costs (as this would
// represent a profit), otherwise we return false.
bool profit1(Revenue1 rev, Costs1 cos)
{
  if (rev.revenue > cos.costs)
    return true;
  else
    return false;
}

// A friend function can also be a member function of another class!  Here we
// re-work the above example to make the profit2 function a member of class
// Revenue2 and a friend function of Costs2.

// Again we provide a forward declaration of Costs2
class Costs2;

// A simple class for representing revenues
class Revenue2
{
private:
  // a private member variable for representing the revenue
  int revenue;

public:
  // Construct sets revenue member variable to the int argument provided
  Revenue2(int revenue) : revenue(revenue) {}

  // profit2 is a member variable of Revenue2 (we will define it later) and it
  // will be a friend function of the Costs2 class
  bool profit2(Costs2 cos);
};

// A simple class for representing costs
class Costs2
{
  // profit2 is a member function of Revenue2 (notice Revenue2::) BUT is also
  // declared as a friend function of Class2 below
  friend bool Revenue2::profit2(Costs2 cos);

private:
  // private member variable for representing costs
  int costs;

public:
  // Constructor sets costs member variable to the int argument provided
  Costs2(int costs) : costs(costs) {}
};

// Define the Revenue2 member function profit2... because it is a friend of
// Costs2 it will have access to the private and protected members of the
// Costs2 object.  We return return if revenue is greater than costs, and
// false otherwise.
bool Revenue2::profit2(Costs2 cos)
{
  if (revenue > cos.costs)
    return true;
  else
    return false;
}

// Test out the above classes
int main()
{
  // Create an instance of MyClass with x set to 7
  MyClass myobject(7);

  // Call print to print out the current value of x
  myobject.print();

  // Use friend function double_x() to double the value of x... notice how we
  // do NOT call it like a member function (e.g. my.object.doublex(...)) because
  // it is NOT a member function of MyClass objects.
  double_x(myobject);

  // Print out the current value of x again to verify that it has doubled to 14
  myobject.print();

  // Create Revenue1 and Costs1 objects, here revenue is greater than costs
  Revenue1 revenue1(1000);
  Costs1 costs1(500);

  // Use the profit1() function which is a friend of both Revenue1 and Costs1
  // classes, and we should report that a profit has occurred
  if (profit1(revenue1, costs1))
    cout << "Profit!" << endl;
  else
    cout << "No profit!" << endl;

  // Create Revenue2 and Costs2 objects, here revenue is not greater than costs
  Revenue2 revenue2(500);
  Costs2 costs2(1000);

  // Use the profit2() member function of the Revenue2 object revenue2 which is
  // ALSO a friend of the Costs2 class, and we should report no profit this time
  if (revenue2.profit2(costs2))
    cout << "Profit!" << endl;
  else
    cout << "No profit!" << endl;

  return 0;
}

/*******************************************************************************
 *
 * Program: Operator Overloading Using Friend Functions Examples
 *
 * Description: Examples of how to implement operator overloading using friend
 * functions in C++.
 *
 *******************************************************************************/

#include <iostream>

using namespace std;

// A simple class to represent a counter that starts off at an specific int
// count value when it is created, where this counter value can be incremented
// and printed.
class Counter
{
  // declare a friend function that will set a Counter object's count to zero
  friend void set_to_zero(Counter &counter);

  // declare a friend function that will overload the + operator for Counter
  // objects, adding together their counts and creating a new Counter with this
  // combined count
  friend Counter operator+(Counter c1, Counter c2);

  // declare a friend function that will multiply the count of a Counter object
  // by an int value m, where Counter is the right operand and the int value m
  // is the left operand, returning a new Counter object set to the resulting
  // value
  friend Counter operator*(int m, Counter counter);

private:
  // the count of the Counter object
  int count;

public:
  // constructer sets the count of the Counter to the int argument its provided
  Counter(int count) : count(count) {}

  // print out the count of the Counter
  void print()
  {
    cout << count << endl;
  }

  // increment the count of the Counter by 1
  void increment()
  {
    count++;
  }

  // We could overload the + operator using a member function, but we comment
  // it out here so we can overload it using a friend function instead.  If we
  // left both functions uncommented, there would be an ambiguity as to which
  // function to call when using the + operator, so we need to comment it out.
  //
  // The member function is called for the left operand of the + operator when
  // it is used, so when we use the member variable 'count' it is the member
  // variable count of the left operand.  The right operand Counter object is
  // provided as a function argument, and so we use counter.count to access the
  // right operand's count.  We create a new Counter with the sum of these
  // values as its initial count, and we return this object.
  /*
  Counter operator+(Counter const &counter)
  {
    Counter new_counter(count + counter.count);
    return new_counter;
  }
  */

  // We cannot use a member function to implement operator overloading when the
  // left operand is not a class, but we CAN implement operator overloading when
  // the right operand is not a class.  This is because the left operand is the
  // object for which the member function which implements operator overloading
  // is called.  We use a friend function (see below) to implement a situation
  // where the left operand is not a class.  Here we use member function to
  // implement a situation where the right opernad is not a class.
  //
  // This member function will handle the situation that we multiply:
  //
  //    counter_object * int_value
  //
  // Where the Counter object is the left operand (for which this member
  // function is called), and the right operand is an int.  We create and return
  // a new Counter object with the count set to the count of the left operand
  // Counter object by the int_value (m).
  //
  Counter operator*(int m)
  {
    int new_count = count * m;
    return Counter(new_count);
  }
};

// Implement the friend function declared in the Counter class.  This function
// is NOT a member of the Counter class, but as a friend function is has access
// to the protected and private members of Counter class.  The function is
// passed a Counter object by reference, which means the Counter object passed
// to this function as an argument can be modified.  We set the Counter object's
// count to 0... notably we're able to access and modify a private member
// variable of Counter even though is is not a member function of Counter.
void set_to_zero(Counter &counter)
{
  counter.count = 0;
}

// Implement another friend function declared in the Counter class.  Here we
// overload the + operator.  Unlike the member function commented out above to
// overload the + operator, we must have two parameters for each Counter object
// (in this case c1 and c2).  In the case of the member function implementation,
// the left operand is the object for which the member function is called and
// so we only needed one parameter.  In this solution we add together the count
// member variables of each Counter object (and we're able to access these, as
// this function is a friend of Counter), create a new Counter object with this
// sum, and return it.
Counter operator+(Counter c1, Counter c2)
{
  int new_count = c1.count + c2.count;
  return Counter(new_count);
}

// Implement the last friend function declared in the Counter.  We actually
// CANNOT implement operator overloading using member functions if the left
// operand is not a class, because the left operand is the object for which
// the member function that implements operator overloading is called.  If
// the left operand is not a class (or a class we can modify), we can't define
// a member function for it.  Here we implement operator overloading of the
// * operator:
//
//     int_value * counter_object
//
// where the left operand is the int value m, and the right value is a Counter
// object.  We multiply m by the Counter object's count, we create a new
// Counter object with the result of this multiplication, and we return it.
Counter operator*(int m, Counter counter)
{
  int new_count = m * counter.count;
  return Counter(new_count);
}

int main()
{
  // Create a Counter object, increment the count, and print it, to test if
  // the Counter objects are working correctly.
  Counter counter1(7);
  counter1.increment();
  counter1.print();

  // Create a 2nd Counter object and print it
  Counter counter2(3);
  counter2.print();

  // Use the overloaded + operator to add two Counters together
  Counter counter3 = counter1 + counter2;

  // Output the resulting Counter object (the count should be 11)
  counter3.print();

  // Use the friend function set_to_zero to set the Counter object's count
  // to 0... notably the friend function can access the private member variable
  // count of the Counter object, and modify it by setting it to 0 too.
  set_to_zero(counter3);

  // Output the Counter object after setting it to 0 to verify it has worked
  counter3.print();

  // Create another Counter object with a count of 5
  Counter counter4(5);

  // Use the overloaded * operator to multiply the Counter object's count by 20
  // and produce a new Counter object (which should have a count of 100).
  //
  // Here we use the member function which overloads the * operator with an int
  // as the right operand.
  //
  Counter counter5 = counter4 * 20;

  // We could also use the friend function with overloads the * operator with
  // an int as the left operand.
  //
  // Counter counter5 = 20 * counter4;

  // Output to confirm the count of this Counter is set to 100
  counter5.print();

  return 0;
}

// There are some situations in which we MUST use either a friend function
// or member function to implement operator overloading.
//
// We cannot use a member function for operator overloading when the left
// member is not a class OR it is a class we can't modify.
//
// Notably, we cannot overload the << stream insertion operator as a member
// function because doing so requires a function with << as the left operand!
//
// C++ does not allow these operators to be overloaded using a friend function:
//
//   assignment: =
//   function call: ()
//   member selection (arrow): ->
//
// So we must use a member function instead!

/*******************************************************************************
 *
 * Program: Deleted Functions Demonstration
 *
 * Description: Examples of using deleted functions in C++.  Deleted functions
 * are declared using the delete specifier '= delete;' at the end of a function
 * declaration, and the compiler will disable a deleted function.
 *
 *
 *******************************************************************************/

// Create a simple class where each object should have a unique id
class Unique
{

  // the private member variable id will store each object's unique id
private:
  int id;

public:
  // Use a class variable next_id to keep track of the 'next id' to give
  // to an object when it is created, we initialize it to 1 below the class.
  static int next_id;

  // When a Unique object is instantiated we set its id to the current value
  // of next_id and then increment next_id so that the next object created
  // will use the next id.
  Unique()
  {
    id = next_id;
    next_id++;
  }

  // returns the id of the object
  int getID()
  {
    return id;
  }

  // Normally the C++ compiler will automatically provide implicitly-declared
  // default copy constructor and copy assignment member functions. So that
  // we can initialize an object like this using  the copy constructor:
  //
  // Unique u1 = u2;
  //
  // and u1 would be assigned u2's id!  We could also use the copy assignment
  // operator like this:
  //
  // u1 = u2;
  //
  // and again u1 would be assigned u2's id.  The problem is that we want our
  // objects to each have a unique id, so supporting either of these operations
  // doesn't make sense given they would cause a bug for our program's logic
  // (though not a compiler error).
  //
  // Here we disable both member functions for the copy constructor and
  // the copy assignment operator using the "= delete;" specifier.  We call
  // the deleted functions now... attempting to use them will cause a compiler
  // error to occur signalling to the developer/writer that they're doing
  // something they shouldnt' be!

  // Make the copy constructor a deleted function.
  Unique(const Unique &) = delete;

  // Copy constructor:
  // Unique(const Unique &other)
  // {
  //   id = other.id;
  // }

  // Make the copy assignmenet operator a deleted function.
  Unique &operator=(const Unique &) = delete;

  // Copy assignment operator:
  // Unique &operator=(const Unique &other) {
  //   this->id = other.id;
  //   return *this;
  // }
};

// Initialize the next_id class variable to 1
int Unique::next_id = 1;

int main()
{
  // Create 3 unique objects, each will have a unique id
  Unique unique1;
  Unique unique2;

  // Attempt to use the copy constructor, this will cause a compiler error
  // because it is a deleted function!
  // Unique unique3 = unique1;

  // How we should create unique3...
  Unique unique3;

  // Attempt to use the copy assignment operator, this will also cause a
  // compiler error because it is a deleted function too!
  // unique3 = unique1;

  // Output the ids of the Unique objects which we want to be all unique.
  // If we remove the deleted function declarations in our Unique class
  // then our program will compile, but unique3 will not have a unique id,
  // it will have the same id as unique1!  If we also remove the assignment
  // statement above and the initialization of unique3 to unique1, then we'll
  // find all the objects have unique ids as desired.
  cout << "unique1 id: " << unique1.getID() << endl;
  cout << "unique2 id: " << unique2.getID() << endl;
  cout << "unique3 id: " << unique3.getID() << endl;

  return 0;
}