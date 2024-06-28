#include "./iterator/my_allocator.h"



namespace mystl {
    template<class T>
    T* MyAllocator<T>::address(reference x) const noexcept {
        return &x;
    }
    
}