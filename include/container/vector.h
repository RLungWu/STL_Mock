#ifndef MY_VECTOR
#define MY_VECTOR


#include <memory>



namespace MySTL {
    template <typename T, class Alloc = std::allocator<T>>
    class MyVector{
        public:
            MyVector();
            MyVector(int size);
        private:
    };
}


#endif