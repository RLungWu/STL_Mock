#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_STL_LIST_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_STL_LIST_H_

#include <cstddef>
#include <memory>

#include "stl_list_node.h"


namespace mystl {
    template<class T, class Alloc = std::allocator<T>>
    class list{
        private:
            using list_node = _list_node<T>;
        public:
            using link_type = list_node*;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using value_type = T;
            using reference = value_type&;
            using const_reference = const value_type&;
            using pointer = value_type*;
            using const_pointer = const value_type*;
            using iterator = _list_iterator<T, T&, T*>;
        
        private:
            using node = link_type;
        

        public:
            iterator begin() {return (link_type)((*node).next);}
            iterator end() {return node;}
            bool empty() const {return node->next == node;}
            size_type size() const{
                size_type result = 0;
                distance(begin(), end(), result);
                return result;
            }

            reference front() {return *begin();}
            reference back() {return *(--end());}

            
    };
}



#endif