#ifndef TRASHCAN
#define TRASHCAN

//Small working unit to prevent
//redefinition of trashcan

struct trashcan //for use with std::conditional to prevent failure in non-SFINAE contexts
{
    template<typename T>
    trashcan(const T& obj){}
};

#endif // TRASHCAN
