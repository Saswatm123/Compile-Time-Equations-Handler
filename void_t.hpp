#ifndef VOID_T
#define VOID_T

//Small working unit to prevent
//redefinition of void_t

template<typename... arglist>
using void_t = void;

#endif // VOID_T
