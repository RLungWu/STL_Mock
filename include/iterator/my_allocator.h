#ifndef STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_
#define STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_

#include <memory>
#include <climits>
#include <new>
#include <limits>

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

            MyAllocator() = default;
            ~MyAllocator() = default;

            pointer allocate(size_type num_objs){
                return static_cast<pointer>(operator new(sizeof(T)*num_objs));
            }

            pointer allocate(size_type num_objs, const_void_pointer hint){
                return allocate(num_objs);
            }

            void deallocate(pointer p, size_type num_objs){
                operator delete(p);
            }

            size_type max_size() const{
                return std::numeric_limits<size_type>::max();
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