# Compile-Time-Equations-Handler

This is a work in progress. Anyone may feel free to contribute, but I'm mainly putting this up to make viewing history easier and get used to Git.

This is a **Compile Time** Equations Handler. It is handed equations at **compile** time, along with the values of known variables and uses Template Metaprogramming
and constexpr-dependency to resolve the equations for the desired variable before runtime.

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
 
# Algorithm
 
I will split this section into two: the first section is the high level logic, which is a general overview of what the program is doing. The second is the low level logic, the actual internal mechanisms of how everything is implemented.

># High level
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

---Will finish this explanation later---
