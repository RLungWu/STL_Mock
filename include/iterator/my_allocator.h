#ifndef STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_
#define STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_

#include <memory>
#include <climits>
#include <new>
#include <limits>
#include<iostream>

namespace mystl {
    template <class T>
    class MyAllocator{
        public:
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T*;
            using void_pointer = void*;
            using const_void_pointer = const void*;
            using size_type = size_t;
            using difference_type = std::ptrdiff_t;

            MyAllocator() noexcept: alloc_count(0){}
            ~MyAllocator() = default;

            MyAllocator(const MyAllocator&) noexcept = default;
            MyAllocator& operator=(const MyAllocator&) noexcept = default; //Prevent Assignment

            template<class U>
            MyAllocator(const MyAllocator<U>&) noexcept {}

            pointer allocate(size_type num_objs_, const_void_pointer hint_ = nullptr){
                pointer result = static_cast<pointer>(::operator new(sizeof(T) * num_objs_));
                if(!result) throw std::bad_alloc();

                alloc_count += num_objs_;
                return result;
            }

            void deallocate(pointer p_, size_type num_objs_) noexcept {
                ::operator delete(p_);
                alloc_count -= num_objs_;
            }

            size_type max_size() const noexcept {
                return std::numeric_limits<size_type>::max() / sizeof(T);
            }

            template<class U>
            struct rebind{
                using other = MyAllocator<U>;
            };

            size_type get_allocations() const{
                return alloc_count;
            }
        private:
            size_type alloc_count;


    };
}


#endif