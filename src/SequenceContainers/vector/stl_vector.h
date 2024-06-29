#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_VECTOR_STL_VECTOR_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_VECTOR_STL_VECTOR_H_

#include <cstddef>
#include <memory>
#include <type_traits>


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
            iterator finish_;
            iterator end_of_storage_;
        
        
        public: //constuctor
            my_vector(): start_(nullptr), finish_(nullptr), end_of_storage_(nullptr){}
            explicit my_vector (size_type n){fill_initialize(n, value_type());}
            my_vector(size_type n, const value_type &value){fill_initialize(n, value); }
            



    };
};
#endif