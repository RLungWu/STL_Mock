#ifndef STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_
#define STL_MOCK_INCLUDE_ITERATOR_ALLOCATOR_H_


namespace mystl {
    template <typename T>
    class Allocator{
        public:
            T* allocate(size_t n);
            void deallocate(T* p, size_t n);
            void construct(T* p, const T& val);
            void destroy(T* p);
    };
}


#endif