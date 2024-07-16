#pragma once

#include <cstddef>
#include <iostream>
#include <iterator>

namespace mystl{
// 五種迭代器
struct input_iterator_tag {};
struct output_iterator_tag {};
struct forward_iterator_tag : public input_iterator_tag {};
struct bidirectional_iterator_tag : public forward_iterator_tag {};
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

template<class Category, class T, class Distance = ptrdiff_t, class Pointer = T*, class Reference = T &>
struct iterator{
    using iterator_category = Category;
    using value_type = T;
    using difference_type = Distance;
    using pointer = Pointer;
    using refence = Reference;
};

// Traits 展現 Iterator 的所有特性
// 之所以不直接使用 template 是因為存在偏特化版本
// 如果要考慮特化版本可能要使用更高深的語法，先忽略
template <class Iterator>
struct iterator_traits{
    using iterator_category = typename Iterator::iterator_category;
    using value_type = typename Iterator::value_type;
    using difference_type = typename Iterator::difference_type;
    using pointer = typename Iterator::pointer;
    using reference = typename Iterator::reference;
};

// 針對 raw pointer 偏特化版本
template <class T>
struct iterator_traits<T *>{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = T*;
    using reference = T&;  
};

//針對 pointer to const 的特化版本
template <class T>
struct iterator_traits<const T*>{
    using iterator_category = random_access_iterator_tag;
    using value_type = T;
    using difference_type = ptrdiff_t;
    using pointer = const T*;
    using reference = const T&;
};

//模仿 c++14 type_traits_t 而設定的模板
template <class Iterator>
using iterator_category_t = 
    typename iterator_traits<Iterator>::iterator_category;

template <class Iterator>
using value_type_t = 
    typename iterator_traits<Iterator>::value_type;

template <class Iterator>
using difference_type_t = 
    typename iterator_traits<Iterator>::difference_type;

template <class Iterator>
using pointer_t = 
    typename iterator_traits<Iterator>::pointer;

template <class Iterator>
using reference_t = 
    typename iterator_traits<Iterator>::reference;

//整組的 distance 函數
template <class InputIterator>
inline difference_type_t<InputIterator> _distance(  InputIterator first, 
                                                    InputIterator last, 
                                                    input_iterator_tag){
    difference_type_t<InputIterator> n = 0;
    while(first != last){
        ++first;
        ++n;
    }
    return n;
}


template <class InputIterator>
inline difference_type_t<InputIterator> _distance(  InputIterator first, 
                                                    InputIterator last,     
                                                    random_access_iterator_tag){
    return last - first;
}

template<class InputIterator>
inline difference_type_t<InputIterator> distance(   InputIterator first,   
                                                    InputIterator last){
    return _distance(first, last, iterator_category_t<InputIterator>());
}

//advance 函數
template<class InputIterator, class Distance>
inline void _advance(InputIterator &it, Distance n,
                    bidirectional_iterator_tag){
    while(n--){
        ++it;
    }
}

template<class InputIterator, class Distance>
inline void _advance(InputIterator &it, Distance n,
                    random_access_iterator_tag){
    if(n >= 0){
        while (n--){
            ++it;
        }
    }else{
        while (n++){
            --it;
        }
    }
}

template<class InputIterator, class Distance>
inline void _advance(InputIterator &it, Distance n,
                    input_iterator_tag){
    it += n;
}

template<class InputIterator, class Distance>
inline void advance(InputIterator &it, Distance n){
    _advance(it, n, iterator_category_t<InputIterator>());
}

//以下為三種迭代適配器



}
