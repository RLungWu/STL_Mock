#pragma once

#include "alloc.h"
#include "construct.h"

namespace mystl{
    template <class T, class Alloc = _default_alloc>
    class simple_alloc{
        public:
            using value_type = T;
            using pointer = T *;
            using const_pointer = const T *;
            using reference = T&;
            using const_reference = const T&;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
        
        public:
            static pointer allocate();
            static pointer allocate(size_t n);
            
            static void deallocate(pointer ptr);
            static void deallocate(pointer ptr, size_t n);

            static void construct(pointer ptr);
            static void construct(pointer ptr, const T& value);

            static void destroy(pointer ptr);
            static void destroy(pointer first, pointer last);
    };

    template<class T, class Alloc>
    T* simple_alloc<T, Alloc>::allocate(){
        return reinterpret_cast<T*>(Alloc::allocate(sizeof(T)));
    }

    template<class T, class Alloc>
    T *simple_alloc<T, Alloc>::allocate(size_t n){
        if(n == 0){
            return nullptr;
        }
        return reinterpret_cast<T*>(Alloc::allocate(sizeof(T) * n));
    }

    template<class T, class Alloc>
    void simple_alloc<T, Alloc>::deallocate(T* ptr){
        Alloc::deallocate(reinterpret_cast<char*>(ptr), sizeof(T));
    }

    template<class T, class Alloc>
    void simple_alloc<T, Alloc>::deallocate(T* ptr, size_t n){
        if(n == 0){
            return;
        }
        Alloc::deallocate(reinterpret_cast<char*>(ptr), sizeof(T) * n);
    }

    template<class T, class Alloc>
    void simple_alloc<T, Alloc>::construct(T* ptr){
        new (ptr) T();
    }

    template<class T, class Alloc>
    void simple_alloc<T, Alloc>::construct(T* ptr, const T& value){
        new (ptr) T(value);
    }

    template<class T, class Alloc>
    void simple_alloc<T, Alloc>::destroy(T* ptr){
        ptr->~T();
    }

    template<class T, class Alloc>
    void simple_alloc<T, Alloc>::destroy(T *first, T*last){
        for(; first != last; ++first){
            first->~T();
        }
    }
}