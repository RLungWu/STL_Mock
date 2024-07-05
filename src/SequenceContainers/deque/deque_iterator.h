#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_DEQUE_ITERATOR_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_DEQUE_ITERATOR_H_

#include <cstddef>
#include <memory>

namespace mystl{
    template <class T, class Ref, class Ptr, size_t BufSiz>
    struct _deque_iterator{
        using iterator =  _deque_iterator<T, T&,T*, BufSiz>;
        using const_iterator = _deque_iterator<T, const T&, const T*, BufSiz>;

        static size_t buffer_size() { return _deque_buffer_size(BufSiz, sizeof(T)); }
        
        using iterator_category = std::random_access_iterator_tag;
        using value_type = T;
        using pointer = Ptr;
        using reference = Ref;
        using size_type = size_t;
        using difference_type = ptrdiff_t;
        using map_pointer = T**;

        using self = _deque_iterator<T, Ref, Ptr, BufSiz>;


        T* cur;
        T* first;
        T* last;          
        map_pointer node; //point to the current buffer


        static size_t _deque_buffer_size(size_t n, size_t sz){
            return n != 0 ? n : (sz < 512 ? size_t(512 / sz) : size_t(1));
        }

        void set_node(map_pointer new_node){
            node = new_node;
            first = *new_node;
            last = first + difference_type(buffer_size());
        }

        reference operator*() const { return *cur; }
        pointer operator->() const { return &(operator*()); }
        difference_type operator-(const self& x) const {
            return difference_type(buffer_size()) * (node - x.node - 1) + (cur - first) + (x.last - x.cur);
        }

        self& operator++(){
            ++cur;
            if(cur == last){
                set_node(node + 1);
                cur = first;
            }
            return *this;
        }

        self operator++(int){
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self& operator--(){
            --cur;
            if(cur == first){
                set_node(node -1);
                cur = last;
            }
            return *this;
        }
        self operator--(int){
            self tmp = *this;
            --*this;
            return tmp;
        }

        self& operator+=(difference_type n){
            difference_type offset = n + (cur - first);
            if(offset >= 0 && offset < difference_type(buffer_size()))
                cur += n;
            else{
                difference_type node_offset = offset > 0 ? offset / difference_type(buffer_size())
                                                        : -difference_type((-offset - 1) / buffer_size()) - 1;
                set_node(node + node_offset);
                cur = first + (offset - node_offset * difference_type(buffer_size()));
            }
            return *this;
        }

        self operator+(difference_type n) const{
            self tmp = *this;
            return tmp += n;
        }

        self& operator-=(difference_type n) { return *this += -n; }
        self operator-(difference_type n) const{
            self tmp = *this;
            return tmp -= n;
        }

        reference operator[](difference_type n) const { return *(*this + n); }

        bool operator==(const self& x) const { return cur == x.cur; }
        bool operator!=(const self& x) const { return !(*this == x); }
        bool operator<(const self& x) const {
            return (node == x.node) ? (cur < x.cur) : (node < x.node);
        }

        



    };
}


#endif