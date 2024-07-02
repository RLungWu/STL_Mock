#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_DEQUE_STL_DEQUE_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_DEQUE_STL_DEQUE_H_

#include "deque_iterator.h"
#include <cstddef>
#include <memory>



namespace mystl {
    template<class T, class Alloc = std::allocator<T>, size_t BufSiz = 0>
    class my_deque{
        public:
            using value_type = T;
            using pointer = value_type*;
            using reference = value_type&;
            using size_type = size_t;
            using difference_type = ptrdiff_t;
            using const_pointer = const value_type*;
            using const_reference = const value_type&;
            using const_iterator = const value_type*;

            using iterator = _deque_iterator<T, T&, T*, BufSiz>;
        
        private:
            using map_pointer = pointer*;
            using map_type = pointer*;
            using map_alloc = typename Alloc::template rebind<pointer>::other;

            iterator start;
            iterator finish;
            
            map_pointer map;
            size_type map_size;
        
        public:
            iterator begin() { return start; }
            iterator end() { return finish; }
            //const_iterator begin() const { return start; }

            reference operator[](size_type n) { return start[difference_type(n)]; }//Define in deque_iterator.h

            reference front(){return *start;}
            reference back(){
                iterator tmp = finish;
                --tmp;
                return *tmp;
            }

            size_type size() const { return finish - start; }
            size_type max_size() const { return size_type(-1); }
            bool empty() const { return finish == start; }

        private:
            //using data_allocator = simple_alloc<value_type, Alloc>;
            using data_allocator = typename Alloc::template rebind<value_type>::other;
            using map_allocator = typename Alloc::template rebind<pointer>::other;

            void fill_initialize(size_type n, const value_type& value){
                create_map_and_nodes(n);
                map_pointer cur;
                for(cur = start.node; cur < finish.node; ++cur){
                    uninitialized_fill(*cur, *cur + iterator::buffer_size(), value);
                }
                uninitialized_fill(finish.first, finish.cur, value);
            }
            void create_map_and_nodes(size_type num_elements){
                size_type num_nodes = num_elements / iterator::buffer_size() + 1;
                map_size = std::max(static_cast<size_type>(8), num_nodes + 2);
                map = map_allocator::allocate(map_size);
                map_pointer nstart = map + (map_size - num_nodes) / 2;
                map_pointer nfinish = nstart + num_nodes - 1;

                map_pointer cur;
                for(cur = nstart; cur <= nfinish; ++cur){
                    *cur = data_allocator::allocate(iterator::buffer_size());
                }
                start.set_node(nstart);
                finish.set_node(nfinish);
                start.cur = start.first;
                finish.cur = finish.first + num_elements % iterator::buffer_size();
            }
            void push_back_aux(const value_type& t){
                value_type t_copy = t;
                reserve_map_at_back();
                *(finish.node + 1) = data_allocator::allocate(iterator::buffer_size());
                construct(finish.cur, t_copy);
                finish.set_node(finish.node + 1);
                finish.cur = finish.first;
            }
            void push_front_aux(const value_type& t){
                value_type t_copy = t;
                reserve_map_at_front();
                *(start.node - 1) = data_allocator::allocate(iterator::buffer_size());
                start.set_node(start.node - 1);
                start.cur = start.last - 1;
                construct(start.cur, t_copy);
            }
            void reserve_map_at_back(size_type nodes_to_add = 1){
                if(nodes_to_add + 1 > map_size - (finish.node - map)){
                    reallocate_map(nodes_to_add, false);
                }
            }
            void reserve_map_at_front(size_type nodes_to_add = 1){
                if(nodes_to_add > size_type(start.node - map)){
                    reallocate_map(nodes_to_add, true);
                }
            }
            void reallocate_map(size_type nodes_to_add, bool add_at_front){
                size_type old_num_nodes = finish.node - start.node + 1;
                size_type new_num_nodes = old_num_nodes + nodes_to_add;
                map_pointer new_nstart;
                if(map_size > 2 * new_num_nodes){
                    new_nstart = map + (map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
                    if(new_nstart < start.node){
                        std::copy(start.node, finish.node + 1, new_nstart);
                    }else{
                        std::copy_backward(start.node, finish.node + 1, new_nstart + old_num_nodes);
                    }
                }else{
                    size_type new_map_size = map_size + std::max(map_size, nodes_to_add) + 2;
                    map_pointer new_map = map_allocator::allocate(new_map_size);
                    new_nstart = new_map + (new_map_size - new_num_nodes) / 2 + (add_at_front ? nodes_to_add : 0);
                    std::copy(start.node, finish.node + 1, new_nstart);
                    map_allocator::deallocate(map, map_size);
                    map = new_map;
                    map_size = new_map_size;
                }
                start.set_node(new_nstart);
                finish.set_node(new_nstart + old_num_nodes - 1);
            }
            void pop_back_aux(){
                data_allocator::deallocate(finish.first, iterator::buffer_size());
                finish.set_node(finish.node - 1);
                finish.cur = finish.last - 1;
                destroy(finish.cur);
            }
            void pop_front_aux(){
                destroy(start.cur);
                data_allocator::deallocate(start.first, iterator::buffer_size());
                start.set_node(start.node + 1);
                start.cur = start.first;
            }
            void destroy(iterator first, iterator last){
                for(map_pointer node = first.node; node < last.node; ++node){
                    destroy(*node, *node + iterator::buffer_size());
                }
                if(first.node != last.node){
                    destroy(first.cur, first.last);
                    destroy(last.first, last.cur);
                }else{
                    destroy(first.cur, last.cur);
                }
            }
        public:
            my_deque(int n, const value_type& value){
                fill_initialize(n, value);
            }
            
        
        public:
            void pop_front(){
                if(start.cur != start.last - 1){
                    destroy(start.cur);
                    ++start.cur;
                }else{
                    pop_front_aux();
                }
            }
            void push_back(const value_type& t){
                if(finish.cur != finish.last - 1){
                    construct(finish.cur, t);
                    ++finish.cur;
                }else{
                    push_back_aux(t);
                }
            }

            void push_front(const value_type& t){
                if(start.cur != start.first){
                    construct(start.cur - 1, t);
                    --start.cur;
                }else{
                    push_front_aux(t);
                }
            }

            void pop_back(){
                if(finish.cur != finish.first){
                    --finish.cur;
                    destroy(finish.cur);
                }else{
                    pop_back_aux();
                }
            }
            void clear(){
                for(map_pointer node = start.node + 1; node < finish.node; ++node){
                    destroy(*node, *node + iterator::buffer_size());
                    data_allocator::deallocate(*node, iterator::buffer_size());
                }
                if(start.node != finish.node){
                    destroy(start.cur, start.last);
                    destroy(finish.first, finish.cur);
                    data_allocator::deallocate(finish.first, iterator::buffer_size());
                }else{
                    destroy(start.cur, finish.cur);
                }
                finish = start;
            }
            iterator erase(iterator pos){
                iterator next = pos;
                ++next;
                difference_type index = pos - start;
                if(index < size() / 2){
                    std::copy_backward(start, pos, next);
                    pop_front();
                }else{
                    std::copy(next, finish, pos);
                    pop_back();
                }
                return start + index;
            }
            iterator erase(iterator first, iterator last){
                if(first == start && last == finish){
                    clear();
                    return finish;
                }else{
                    difference_type n = last - first;
                    difference_type elems_before = first - start;
                    if(elems_before < (size() - n) / 2){
                        std::copy_backward(start, first, last);
                        iterator new_start = start + n;
                        destroy(start, new_start);
                        for(map_pointer cur = start.node; cur < new_start.node; ++cur){
                            data_allocator::deallocate(*cur, iterator::buffer_size());
                        }
                        start = new_start;
                    }else{
                        std::copy(last, finish, first);
                        iterator new_finish = finish - n;
                        destroy(new_finish, finish);
                        for(map_pointer cur = new_finish.node + 1; cur <= finish.node; ++cur){
                            data_allocator::deallocate(*cur, iterator::buffer_size());
                        }
                        finish = new_finish;
                    }
                    return start + elems_before;
                }
            }

    };
}


#endif