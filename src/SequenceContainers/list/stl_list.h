#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_STL_LIST_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_STL_LIST_H_

#include <cstddef>
#include <memory>

#include "stl_list_node.h"
#include "list_iterator.h"

namespace mystl {
    template<class T, class Alloc = std::allocator<T>>
    class my_list{
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
            link_type node;
            
        

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
        
        private:
            link_type get_node() {return Alloc::allocate();}
            void put_node(link_type p) {Alloc::deallocate(p);}

            link_type create_node(const T& x){
                link_type p = get_node();
                Alloc::construct(&p->data, x);
                return p;
            }

            void destroy_node(link_type p){
                Alloc::destroy(&p->data);
                put_node(p);
            }
        
        private: //constructor
            void empty_initialize(){
                node = get_node();
                node->next = node;
                node->prev = node;
            }

            iterator insert(iterator position, const T& x){
                link_type tmp = create_node(x);
                tmp->next = position.node;
                tmp->prev = position.node->prev;
                (link_type(position.node->prev))->next = tmp;
                position.node->prev = tmp;
                return tmp;
            }
        
        public:
            my_list() {empty_initialize();}

            void push_back(const T& x){
                insert(end(), x);
            }
            void push_front(const T& x){
                insert(begin(), x);
            }

            iterator erase(iterator position){
                link_type next_node = link_type(position.node->next);
                link_type prev_node = link_type(position.node->prev);
                prev_node->next = next_node;
                next_node->prev = prev_node;
                destroy_node(position.node);
                return iterator(next_node);
            }

            void pop_front(){
                erase(begin());
            }
            void pop_back(){
                iterator tmp = end();
                erase(--tmp);
            }

            void  clear(){
                link_type cur = link_type(node->next);
                while(cur != node){
                    link_type tmp = cur;
                    cur = link_type(cur->next);
                    destroy_node(tmp);
                }
                node->next = node;
                node->prev = node;
            }

            void remove(const T& value){
                iterator first = begin();
                iterator last = end();
                while(first != last){
                    iterator next = first;
                    ++next;
                    if(*first == value) erase(first);
                    first = next;
                }
            }

            void unique(){
                iterator first = begin();
                iterator last = end();
                if(first == last) return;
                iterator next = first;
                while(++next != last){
                    if(*first == *next) erase(next);
                    else first = next;
                    next = first;
                }
            }

            
    };
}



#endif