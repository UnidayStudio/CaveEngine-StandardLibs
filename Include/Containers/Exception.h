#ifndef CAVE_EXCEPTION_H
#define CAVE_EXCEPTION_H

#include <cstddef> // size_t

namespace cave{
    class Exception{
    public:
        Exception();
        virtual ~Exception();
    };

    class OutOfRangeException : public Exception {
    public:
        OutOfRangeException(size_t p=-1 /*(should overflow, yes)*/); 
        virtual ~OutOfRangeException();

        size_t pos;
    };
}

#endif // !CAVE_EXCEPTION_H