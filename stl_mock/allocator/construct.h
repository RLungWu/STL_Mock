#pragma once

#include "../utils/type_traits.h"

#include <new>

namespace mystl{
    template <class T1, class T2>
    inline void construct(T1* p, const T2& value){
        new (p) T1(value);
    }

    template <class T>
    inline void destroy(T* p){
        p->~T();
    }

    // 利用 traits 大量解析
    template <class ForwardIterator>
    inline void destroy(ForwardIterator beg, ForwardIterator end){
        using is_POD_type = typename type_traits<ForwardIterator>::is_POD_type;
        _destroy_aux(beg, end, is_POD_type());
    }

    // 如果元素的value_type 不存在non-tririal destructor
    template<class ForwardIterator>
    inline void _destroy_aux(ForwardIterator beg, ForwardIterator end, false_type){
        for(; beg != end; ++beg){
            destroy(&*beg);
        }
    }

    //存在trivial destructor
    //如果對象的 destroy 函數是空的，那麼反覆調用是一種浪費
    template<class ForwardIterator>
    inline void _destroy_aux(ForwardIterator, ForwardIterator , true_type){}

    inline void destroy(char*, char*){}
    inline void destroy(wchar_t *, wchar_t *){}

}






