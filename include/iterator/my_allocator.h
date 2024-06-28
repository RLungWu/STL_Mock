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
            /*
             * Member Types of Allocator
             */
            using value_type = T;
            using pointer = T*;
            using const_pointer = const T*;
            using reference = T&;
            using const_reference = const T&;
            using void_pointer = void*;
            using const_void_pointer = const void*;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;

            template<class U>
            struct rebind{
                using other = MyAllocator<U>;
            };

            /*
             * Constructor and Destructor
             */


            MyAllocator() noexcept: alloc_count(0){}
            MyAllocator(const MyAllocator&) noexcept = default;
            MyAllocator& operator=(const MyAllocator&) noexcept = default; //using when use "=" operator

            template<class U>
            MyAllocator(const MyAllocator<U>& other) noexcept {}

            ~MyAllocator() = default;

            /*
             * Member Functions
             */


            //address 
            pointer address (reference x) const noexcept;
            const_pointer address(const_reference x) const noexcept;



            //allocate
            pointer allocate(size_type num_objs_, const_void_pointer hint_ = nullptr){
                pointer result = static_cast<pointer>(::operator new(sizeof(T) * num_objs_));
                if(!result) throw std::bad_alloc();

                alloc_count += num_objs_;
                return result;
            }

            pointer allocate(size_type num_objs_){
                return allocate(num_objs_, nullptr);
            }


            //deallocate
            void deallocate(pointer p_, size_type num_objs_) noexcept {
                ::operator delete(p_);
                alloc_count -= num_objs_;
            }

            //max_size
            size_type max_size() const noexcept {
                return std::numeric_limits<size_type>::max() / sizeof(T);
            }

            //construct
            template<class U, class... Args>
            void construct(U* p_, Args&&... args){
                ::new (static_cast<void*>(p_)) U(std::forward<Args>(args)...);
            }

            //destroy
            template<class U>
            void destropy(U* p_){
                p_->~U();
            }


            size_type get_allocations() const{
                return alloc_count;
            }
        private:
            size_type alloc_count;


    };
}


#endif