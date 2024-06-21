#ifndef STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_
#define STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_

#include <cstddef>

#include "allocator.h"



namespace mystl {
    template <typename T, class Alloc = MyAllocator<T>>
    class MyVector{
        public:
            //vector type
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using iterator = value_type*;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
        
        protected:
            typedef MyAllocator<value_type> data_allocator;
            iterator start; //The memory address of the first element
            iterator finish; //The memory address of the last element
            iterator end_of_storage; //The memory address of the end of the storage

            void insert_aux(iterator position, const T& x);
            void deallocate(){
                if(start){
                    data_allocator::deallocate(start, end_of_storage - start);
                }
            }
        

    };
}


#endif