#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_LIST_ITERATOR_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_LIST_ITERATOR_H_

#include <cstddef>
#include <iterator>

#include "stl_list_node.h"


namespace mystl{
    template<class T, class Ref, class Ptr>
    struct _list_iterator{
        using iterator = _list_iterator<T, T&, T*>; //iterator is a pointer to T
        using self = _list_iterator<T, Ref, Ptr>;

        using iterator_category = std::bidirectional_iterator_tag;
        using value_type = T;
        using pointer = Ptr;
        using reference = Ref;
        using link_type = _list_node<T>*;
        using size_type = size_t;
        using difference_type = ptrdiff_t;

        link_type node; //pointer to the node

        //constructor
        _list_iterator(link_type x): node(x){}
        _list_iterator(){}
        _list_iterator(const iterator& x): node(x.node){}

        bool operator==(const self& x) const {return node == x.node;}
        bool operator!=(const self& x) const {return node != x.node;}

        //對iterator取值，娶的是節點的data
        reference operator*() const {return node->data;}

        pointer operator->() const {return &(operator*());}

        self& operator++(){
            node = (link_type)(node->next);
            return *this;
        }

        self operator++(int){
            self tmp = *this;
            ++*this;
            return tmp;
        }

        self& operator--(){
            node = (link_type)(node->prev);
            return *this;
        }
        self operator--(int){
            self tmp = *this;
            --*this;
            return tmp;
        }


    };
};



#endif