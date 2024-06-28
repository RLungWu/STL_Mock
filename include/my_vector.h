#ifndef STL_MOCK_INCLUDE_MY_VECTOR_H_
#define STL_MOCK_INCLUDE_MY_VECTOR_H_


#include <memory>


namespace mystl{
    template <class T, class Allocator = std::allocator<T>>
    class my_vector{
        public:
            using value_type = T;
            using allocator_type = Allocator;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = typename std::allocator_traits<Allocator>::pointer;
            using const_pointer = typename std::allocator_traits<Allocator>::const_pointer;

            using iterator = value_type*;
            using const_iterator = const value_type*;
            using reverse_iterator = std::reverse_iterator<iterator>;
            using const_reverse_iterator = std::reverse_iterator<const_iterator>;

        private:
            iterator begin_;    // begin of the vector
            iterator end_;      // end of the vector
            iterator cap_;      // end of the allocated space
        
        public:
            // constructor
            my_vector() noexcept;                                                               // default constructor
            explicit my_vector(const Allocator& alloc) noexcept;                                // constructor with allocator
            my_vector(size_type count, const T& value, const Allocator& alloc = Allocator());   // constructor with count and value
            explicit my_vector(size_type count, const Allocator& alloc = Allocator());          // constructor with count
            
            template <class InputIt>
            my_vector(InputIt first, InputIt last, const Allocator& alloc = Allocator());       // constructor with iterator

            my_vector(const my_vector& other);                                                  // copy constructor

            // destructor
            ~my_vector();

            

    };
};




#endif