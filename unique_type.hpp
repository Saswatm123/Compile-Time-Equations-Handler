#ifndef UNIQUE_TYPE
#define UNIQUE_TYPE

//Small working unit to prevent
//redefinition of unique_type

///For type equality comparisons to fail to
struct unique_type {private: unique_type() = delete;};

#endif // UNIQUE_TYPE
