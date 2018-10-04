# Compile-Time-Equations-Handler

This is a work in progress. Anyone may feel free to contribute, but I'm mainly putting this up to make viewing history easier and get used to Git.

This is a **Compile Time** Equations Handler. It is handed equations at **compile** time, along with the values of known variables and uses Template Metaprogramming
and constexpr-dependency to resolve the equations for the desired variable before runtime. For those not familiar with compile-time programming, its runtime is zero seconds, not counting the time it takes to run the output statement to tell the user what the answer is.

Heavily-templated code is often hard to read, so the organization standards are as
follows:

 - In case of multiline < or > brackets, the > bracket always **VERTICALLY** lines up with its corresponding < bracket. For example, 
 a multiline std::is_same expression may look like this:
 ``` 
     std::is_same<...
                  ...
                  >::value
 ```
 - The above applies to type aliases as well as metafunctions/expressions.
 
# Usage

This is intended to be easy for even non-programmers to use. Simply open up equation_sheet.hpp and put the following information into each namespace:
- **PUT_VARIABLES_HERE** is where the variables belong (both known and unknown). To declare a known named A with value 6, the syntax is:
```
    known A(6);
```
 To declare an unknown named B, the syntax is as follows:
 ```
     unknown B;
 ```
- **PUT_TARGET_VARIABLE_HERE** is where you indicate which variable you wish to solve for. To set a variable called time as the target variable, the following syntax is used:
```
    using TARGVAR = PACK(time);
```
 Two things must be ensured: that the type alias is called TARGVAR, and that it is wrapped in PACK() (will go into detail in the algorithm section later).
 
- **PUT_EQUATIONS_HERE** is where you write down the equations you wish to use. To use the formula F = ma, with name Newton_First_Law, the following syntax is used:
```
    using Newton_First_Law = PACK(force == mass * acceleration);
    
    //The reason I did not use f, m, and a as variable names are to prevent potential name collision,
    //and it is always good to use names where you know nothing will have the same name as something else
    //(ex. "I" can refer to both Impedance and Current, and when writing large tables of equations, it is best
    //to not put yourself in this situation)
```

 The equation names can be anything, but a crucial piece of syntax is wrapping the equation in PACK(). Without this, the program will    not run correctly. The operators are:
 - \+ : Add
 - \- : Subtract
 - \* : Multiply
 - / : Divide
 - ^ : Exponentiate **Crucial: any exponentiation must be inside parentheses due to natural C++ operator precedence putting ^ last.** That means that the formula
```
    G = (1-v^2/c^2)^(1/2) ) 
```
is written as:
```
    G == (1- (v^2)/(c^2) )^(1.0/2.0)
```
**Crucial: every constant must be specified to at least one point beyond the decimal**, so d = 1/2 * at^2 is written as:
```
 d == (1.0/2.0) * a * (t^2);
 //note: the spaces are not neccessary, they are there only for clarity. 
```

The only last step is to go to the call to iter_through_equations in main() and insert the names of every equation followed by "{}" after the passing in of "knownlist". There is sample code already written in main(), so just follow along with that.

# Algorithm
 
I will split this section into two: the first section is the high level logic, which is a general overview of what the program is doing. The second is some important parts of the low level logic, some of the important parts of the internal mechanisms of how everything is implemented.

# High level
> Let us create an arbitrary equation group.
 - a+b/c == d^e
 - a-b+c == 4
> Let us assume beforehand that we know that:
 - a = 1
 - b = 2
 - d = 4
> Given this information, it is clearly visible to us that one can solve for any unknown target variable present (either c or e). The variables are marked either **unknown** or **known**, and when the compiler sees variables arranged with operators in equation form, it creates an operation tree. The operation trees for these two equations would look like this:

![](images_%20for_readme/EQ1.png)

![](images_%20for_readme/EQ2.png)

Each equation has two states. Either information can be immediately derived from it (State 1), or it has greater than one unknown (State 2). The program takes in all equations at once and uses overload resolution to decide what state and data type each equation exists in, and proceeds from there by picking the appropriate function. If an equation has only one unknown, it extracts the new known, checks to see if it's the one we're looking for, and if it's not, adds it to the list of knowns to check other equations with later. It then casts that equation out from the equation stream (we don't need a stray known variable floating around the equation stream), and repeats. If the amount of unknowns is > 1, it plugs in every known that is known at that time, checking to see if the unknown count ever drops to 1. If not, it plugs the most recently updated version of that equation back into the equation stream and continues with the next equation.

The known extraction process uses tree inversion to isolate the desired unknown inside the op_tree. Basically, this:

![](images_%20for_readme/EQ3.png)

turns into this (with desired unknown "a"):

![](images_%20for_readme/EQ4.png)

It is fairly trivial to derive the processes for every combination, so I will leave that out.

# Specifics

First, I will go over what PACK() means and why it exists. Constexpr functions are required to be able to be run at runtime if its arguments are not constexpr. This means that I cannot use the arguments in constexpr-guaranteed contexts, such as template non type parameters. However, as the developer, I know that these funtion will only be used at compile time, so it should feel safe using consexpr function arguments in constexpr-guaranteed contexts. My workaround for this lack of a way to declare arguments constexpr was to create a macro that creates an anonymous union that contains a static constexpr function unzip() that returns a value passed into it, as well as a typedef Ctype that lets me do type selection as if it was just a regular value. This lets me encode a value into a type without it changing the type, and since types are resolved at compile time, I can call unpack() on any wrapped type and recieve it as if it was passed in unwrapped, and it is guaranteed to be constexpr.

The program flow starts with a call to iter_through_equations(). I should mention that when I call something a function, it is very likely a family of functions with the same name. As this is at compile time, type is a variable aspect of the state. Iter_through_equations() takes in the target variable, an empty ntuple (just a regular tuple implementation I created), and a variadic argument pack of equations (more specifically, what type the equations resolve to). There are six iter_through_equation()s total, and to explain what they do, I have to explain the type system that makes up an equation.

There is a class template called var<std::size_t, bool>, that takes in two arguments in its template list: its ID number (for tracing it in an operation tree), and whether it is known or not. The Macro 
```
known
```
is defined as 
```
static constexpr const var<ticket(),1>
```
while the macro
```
unknown
```
is defined as
```
static constexpr const var<ticket(),0>
```
Ignore ticket() for now, it corresponds to a constexpr counting system of retrieving consecutive numbers.

There is a class template called op_tree<LHS,RHS> that holds information about the current operation and what its two subtrees are. These are created from the operators in the equation. Details can be seen in the code. Step 1 in the code allows me to have a user-friendly interface while creating a way to "freeze" equations for later. Op_tree subnodes are only either op_trees, unknowns, or constants. When it comes across a known, it "replaces" it with its value and runs a resolver to see if it "opened up" a tree to being completely evaluated. An equation can resolve to either an op_tree (>1 unknowns), a known(unknown == constant or known), or a bool (constant == constant). Which version of iter_through_equation() to use is decided by SFINAE based on the type of the first equation, with the rest stored away in a variadic argument pack. 

Manually explaining everything that goes on in the program is going to be very verbose and long, so the rest can be figured out from looking at the source files. If anyone has any questions, feel free to post on the forum of this repo.
