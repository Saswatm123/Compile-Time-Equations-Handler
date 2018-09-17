#ifndef PACK_FILE
#define PACK_FILE

//dummy struct to typedef variadic type parameters

namespace MY_PACK
{
    template<typename...args>
    struct pack {};
}

#endif // PACK_FILE
