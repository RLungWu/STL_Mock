#ifndef STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_
#define STL_MOCK_INCLUDE_CONTAINER_VECTOR_H_


#include <cstddef>
#include <memory>



namespace mystl {
    template <typename T, class Allocator = std::allocator<T>>
    class MyVector{
        public:
            //vector type
            using value_type                    = T;
            using allocator_type                = Allocator;
            //using pointer                       = value_type*;  //old version 
            using pointer                       = typename std::allocator_traits<allocator_type>::pointer;
            using const_pointer                 = typename std::allocator_traits<allocator_type>::const_pointer;
            using reference                     = value_type&;
            using const_reference               = const value_type&;
            using iterator                      = pointer;
            using size_type                     = std::size_t;
            using difference_type               = std::ptrdiff_t;
        
        private:
            //using data_allocator = std::allocator<value_type> ;
            iterator start_; //The memory address of the first element
            iterator finish_; //The memory address of the last element, which is going to insert
            iterator end_of_storage_; //The memory address of the end of the storage
            allocator_type alloc_;//The allocator

            void insert_aux(iterator position, const T& x);

            void deallocate(){
                if(start_){
                    //data_allocator::deallocate(start_, end_of_storage_ - start_);
                    std::allocator<allocator_type> :: deallocate(alloc_, start_, end_of_storage_ - start_);
                }
            }

            void fill_initialize(size_type n, const T& value){
                start_ = allocate_and_fill(n, value);
                finish_ = start_ + n;
                end_of_storage_ = finish_;
            }//fill_initialize

            iterator allocate_and_fill(size_type n, const T& x){
                iterator result = std::allocator_traits<allocator_type>::allocate(alloc_, n);
                try{
                    std::uninitialized_fill_n(result, n, x);
                }catch(...){
                    std::allocator_traits<allocator_type>::deallocate(alloc_, result, n);//Ensure that the memory is released
                    throw;
                }

                return result;
            }

        public:
            iterator begin() { return start_; }
            iterator end() { return finish_; }
            size_type size() const { return size_type(end() - begin()); }
            size_type capacity() const { return size_type(end_of_storage_ - begin()); }
            bool empty() const { return begin() == end(); }
            reference operator[](size_type n) { return *(begin() + n); }
            

            //constructor
            MyVector(): start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
            MyVector(size_type n, const T& value) { fill_initialize(n, value); }
            MyVector(int n, const T& value) { fill_initialize(n, value); }
            MyVector(long n, const T& value) { fill_initialize(n, value); }
            explicit MyVector(size_type n) { fill_initialize(n, T()); }

            //destructor
            ~MyVector(){
                destroy(start_, finish_);
                deallocate();
            }

            //Search
            reference front() { return *begin();}           //The first element
            reference back() { return *(end() - 1); }       //The last element
            
            //Insert
            void push_back(const T& x){
                if(finish_ != end_of_storage_){
                    construct(finish_, x);
                    ++finish_;
                }else{
                    insert_aux(end(), x);
                }
            }

            //Erase
            void pop_back(){
                assert(finish_ > start_);
                --finish_;
                finish_ -> ~T();
            }

            iterator erase(iterator position){
                assert(position >= start_ && position < finish_);
                
                if(position +1 != end()){
                    std::move(position + 1, finish_, position);
                }

                --finish_;
                finish_ -> ~T();

                return position;
            }

            void resize(size_type new_size, const T& x){
                if(new_size < size()){
                    erase(begin() + new_size, end());
                }else{
                    insert (end(), new_size - size(), x);
                }
            }
            void resize(size_type new_size){
                resize(new_size, T());
            }

            void clear(){
                erase(begin(), end());
            }
    };
}


#endif