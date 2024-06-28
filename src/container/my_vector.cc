#include "./container/my_vector.h"
#include <algorithm>
#include <memory>

template <typename T, class Allocator>
void mystl::MyVector<T, Allocator>::insert_aux(iterator position, const T& x){
    if ( finish_ != end_of_storage_){
        //Use the std::move for moving the elements to the uninitialized memory
        construct(finish_, std::move(*(finish_ - 1)));
        ++finish_;

        T x_copy = x;
        std::move_backward(position, finish_ - 2, finish_ - 1);
        *position = std::move(x_copy);        
    }else{
        const size_type old_size = size();
        const size_type len = old_size != 0? 2*old_size : 1;

        iterator new_start = std::allocator_traits<allocator_type>::allocate(alloc_, len);
        iterator new_finish = new_start;

        try{
            //use std::move to transfer ownership of objects, improving efficiency
            new_finish = std::uninitialized_move(start_, position, new_start);//TODO: Check later
            construct(new_finish, x);

            ++new_finish;
            new_finish = std::uninitialized_move(position, finish_, new_finish);
        }catch(...){
            std::destroy(new_start, new_finish);
            std::allocator_traits<allocator_type>::deallocate(alloc_, new_start, len);
            throw;
        }

        std::destroy(begin(), end()); //TODO: check later
        deallocate();

        start_ = new_start;
        finish_  = new_finish;
        end_of_storage_ = new_start + len;
    }
}