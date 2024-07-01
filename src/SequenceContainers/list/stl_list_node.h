#ifndef STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_STL_LIST_NODE_H_
#define STL_MOCK_SRC_SEQUENCECONTAINERS_LIST_STL_LIST_NODE_H_



namespace mystl {
    template <class T>
    struct _list_node{
        using link_type = _list_node<T>*;
        
        link_type prev;
        link_type next;
        T data;
    };
}



#endif