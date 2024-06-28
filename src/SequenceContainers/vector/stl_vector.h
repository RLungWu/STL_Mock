#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_VECTOR_STL_VECTOR_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_VECTOR_STL_VECTOR_H_

#include <cstddef>
#include <memory>


namespace mystl{
    template<class T, class Alloc = std::allocator<T>>
    class my_vector{
        public: //alizs declaration
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator = value_type *; //iterator is raw pointer
            using const_iterator = const value_type *;
            using reference = value_type&;
            using const_reference = const value_type&;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
        
        private:
            //iterator to indicate the vector's start
            iterator start_;
            iterator end_;
            iterator end_of_storage_;
        
        private:
            //Allocator and construct aux functions
            using data_allocator = Alloc;

            //constructor
            void fill_initialize(size_type n, const value_type &value){
                start_ = allocate_and_fill(n, value);
                end_ = start_ + n;
                end_of_storage_ = end_;
            }

            iterator allocate_and_fill(size_type n, const value_type &value){
                iterator result = data_allocator::allocate(n);
                std::uninitialized_fill_n(result, n, value);
                return result;
            }


    };
};
#endif