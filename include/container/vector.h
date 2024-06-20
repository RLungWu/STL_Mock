#ifndef STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_
#define STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_

#include <cstddef>

#include "allocator.h"



namespace mystl {
    template <typename T, class Alloc = Allocator<T>>
    class MyVector{
        public:
            //vector type
            typedef T value_type;
            typedef value_type* pointer;
            typedef value_type& reference;
            typedef value_type* iterator;
            typedef std::size_t size_type;
            typedef std::ptrdiff_t difference_type;
        
        protected:
            typedef simple_alloc<value_type, Alloc> data_allocator;
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