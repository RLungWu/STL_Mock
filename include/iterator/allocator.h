#ifndef STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_
#define STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_

#include <memory>
#include <climits>

namespace mystl {
    template <class T>
    class MyAllocator{
        public:
            typedef T value_type;
            typedef T* pointer;
            typedef const T* const_pointer;
            typedef T& reference;
            typedef const T& const_reference;
            typedef size_t size_type;
            typedef ptrdiff_t difference_type;

            //rebind allocator of type U
            template <class U>
            struct rebind{
                typedef MyAllocator<U> other;
            };

            //hint used for locality
            pointer allocate(size_type n, const void* hint = 0){
                return _allocate((difference_type) n, (pointer) 0);
            }

            void deallocate(pointer p, size_type n){
                deallocate(p);
            }

            void construct(pointer p, const T& value){
                _construct(p, value);
            }

            void destroy(pointer p){
                _destroy(p);
            }

            pointer address(reference x){
                return (pointer)&x;
            }

            const_pointer const_address(const_reference x){
                return (const_pointer)&x;
            }

            size_type max_size() const{
                return size_type(UINT_MAX/sizeof(T));
            }
    };
}


#endif