#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_VECTOR_STL_VECTOR_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_VECTOR_STL_VECTOR_H_

#include <algorithm>
#include <cstddef>
#include <memory>
#include <type_traits>



namespace mystl{
    template<class T, class Alloc = std::allocator<T>>
    class my_vector{
        public: //alizs declaration
            using value_type = T;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator = value_type *; //iterator is raw pointer
            using const_iterator = const value_type *;
            using reference = value_type&;
            using const_reference = const value_type&;
            using size_type = std::size_t;
            using difference_type = std::ptrdiff_t;
        
        private:
            //iterator to indicate the vector's start
            //using data_allocator = simple_alloc<value_type, Alloc>; 後續補上
            using data_allocator = std::allocator<value_type>; //data_allocate::allocate(n) means append n space for value_type
            iterator start_;
            iterator finish_;
            iterator end_of_storage_;

        private:
            void insert_aux(iterator position, const T& x){
                if(finish_ != end_of_storage_){//if there is enough space
                    //construct an element at the end of the vector, and use the first element to copy the value of the last element
                    construct(finish_, *(finish_ - 1));
                    ++finish_;
                    T x_copy = x;
                    std::copy_backward(position, finish_ -2, finish_ - 1);
                    *position = x_copy;
                }else{
                    const size_type old_size = size();
                    const size_type len = old_size != 0 ? 2 * old_size : 1;

                    iterator new_start_ = data_allocator::allocate(len);
                    iterator new_finish_ = new_start_;
                    try{
                        new_finish_ = std::uninitialized_copy(start_, position, new_start_);
                        construct(new_finish_, x); //set the value of the new element
                        ++new_finish_;
                        new_finish_ = std::uninitialized_copy(position, finish_, new_finish_);
                    }catch(...){
                        //"commit or rollback" semantics.
                        destroy(new_start_, new_finish_);
                        data_allocator::deallocate(new_start_, len);
                        throw;
                    }

                    destory(begin(), end());
                    deallocate();

                    start_ = new_start_;
                    finish_ = new_finish_;
                    end_of_storage_ = new_start_ + len;
                }
            }
            void deallocate(){
                if(start_){
                    data_allocator::deallocate(start_, end_of_storage_ - start_);
                }
            }
            void fill_initialize(size_type n, const T& value){
                start_ = allocate_and_fill(n, value);
                finish_ = start_ + n;
                end_of_storage_ = finish_;
            }
        
        public: //Basic public function
            iterator begin() {return start_;}
            iterator end() {return finish_;}
            size_type size() const {return size_type(end() - begin());}
            size_type capacity() const {return size_type(end_of_storage_ - begin());}
            bool empty() const {return begin() == end();}
            reference operator[](size_type n) {return *(begin() + n);}
        
        public: //constuctor
            my_vector(): start_(nullptr), finish_(nullptr), end_of_storage_(nullptr) {}
            my_vector(size_type n, const T& value) {fill_initialize(n, value);}
            my_vector(int n, const T& value) {fill_initialize(n, value);}
            my_vector(long n, const T& value) {fill_initialize(n, value);}
            explicit my_vector(size_type n) {fill_initialize(n, T());}
        
        public://deallocator
            ~my_vector(){
                destroy(start_, finish_);
                deallocate();
            }
        
        public: //get number
            reference front(){return *begin();}
            reference back() {return *(end() - 1);}
            void push_back(const T& x){
                if(finish_ != end_of_storage_){
                    construct(finish_, x);
                    ++finish_;
                }else{ //if the vector is full
                    insert_aux(end(), x);
                }
            }
            void pop_back(){
                --finish_;
                destroy(finish_);
            }
            iterator erase(iterator position){
                if(position + 1 != end()){
                    std::copy(position + 1, finish_, position);
                }
                --finish_;
                destroy(finish_);
                return position;
            }
            void resize(size_type new_size, const T& x){
                if(new_size < size()){
                    erase(begin() + new_size, end());
                }else{
                    insert(end(), new_size - size(), x);
                }
            }
            void resize(size_type new_size) {resize(new_size, T());}
            void clear() {erase(begin(), end());}

            void insert(iterator position, size_type n, const T& x){
                if(n != 0){
                    if(size_type(end_of_storage_ - finish_) >= n){//if there is enough space
                        T x_copy = x;
                        const size_type elems_after = finish_ - position;
                        iterator old_finish_ = finish_;
                        if(elems_after > n){//if the elements after the position is more than n
                            std::uninitialized_copy(finish_ - n, finish_, finish_);
                            finish_ += n;
                            std::copy_backward(position, old_finish_ - n, old_finish_);
                            std::fill(position, position + n, x_copy);
                        }else{
                            std::uninitialized_fill_n(finish_, n - elems_after, x_copy);
                            finish_ += n - elems_after;
                            std::uninitialized_copy(position, old_finish_, finish_);
                            finish_ += elems_after;
                            std::fill(position, old_finish_, x_copy);
                        }
                    }else{//這種情況下，竟要先配置新的內存，在移動
                        const size_type old_size = size();
                        const size_type len = old_size + std::max(old_size, n);
                        iterator new_start_ = data_allocator::allocate(len);
                        iterator new_finish_ = new_start_;
                        try{
                            new_finish_ = std::uninitialized_copy(start_, position, new_start_);
                            new_finish_ = std::uninitialized_fill_n(new_finish_, n, x);
                            new_finish_ = std::uninitialized_copy(position, finish_, new_finish_);
                        }catch(...){
                            destroy(new_start_, new_finish_);
                            data_allocator::deallocate(new_start_, len);
                            throw;
                        }
                        destroy(start_, finish_);
                        deallocate();
                        start_ = new_start_;
                        finish_ = new_finish_;
                        end_of_storage_ = new_start_ + len;
                    }
                }else{

                }
            }
        
        private:
            iterator allocate_and_fill(size_type n, const T& x){
                iterator result = data_allocator::allocate(n);//allocate n space for value_type
                std::uninitialized_fill_n(result, n, x);
                return result;
            }
            
            
            



    };
};
#endif