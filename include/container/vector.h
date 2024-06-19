#ifndef STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_
#define STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_


#include "allocator.h"



namespace mystl {
    template <typename T, class Alloc = Allocator<T>>
    class MyVector{
        public:
            MyVector();
            MyVector(int size);
        private:
    };
}


#endif