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
 
 I will outline the general algorithm here sometime within the next couple commits.
