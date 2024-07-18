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
// insert, reserve, stream
//insert::back_insert, front_insert, insert
template<class Container>
class back_insert_iterator{
    protected:
        Container *container; //Container

    public:
        using iterator_category = output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;

    public:
        explicit back_insert_iterator(Container &x) : container(&x){}

        back_insert_iterator &operator=(const typename Container::value_type &value){
            container->push_back(value);//本質上是使用pushback
            return *this;
        }

        back_insert_iterator &operator*(){return *this;}
        back_insert_iterator &operator++(){return *this;}
        back_insert_iterator &operator++(int){return *this;}
};

template<class Container>
inline back_insert_iterator<Container> back_inserter(Container &x){
    return back_insert_iterator<Container>(x);
}

template<class Container>
class front_insert_iterator{
    protected:
        Container *container;
    public:
        using iterator_category = output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
    
    public:
        explicit front_insert_iterator(Container &x) : container(&x){}
        front_insert_iterator &operator=(
            const typename Container::value_type &value){
                container->push_front(value);
                return *this;
        }

        front_insert_iterator &operator*(){return *this;}
        front_insert_iterator &operator++(){return *this;}
        front_insert_iterator &operator++(int){return *this;}
        
};

template<class Container>
inline front_insert_iterator<Container> front_inserter(Container &x){
    return front_insert_iterator<Container>(x);
}

template<class Container>
class insert_iterator{
    protected:
        Container *container;
        typename Container::iterator iter;
    public:
        using iterator_category = output_iterator_tag;
        using value_type = void;
        using difference_type = void;
        using pointer = void;
        using reference = void;
    
    public:
        explicit insert_iterator(Container &x, typename Container::iterator i) : container(&x), iter(i){}
        insert_iterator &operator=(const typename Container::value_type &value){
            iter = container->insert(iter, value);      //使用insert
            ++iter;                                     //確保下一次插入的位置
            return *this;
        }

        insert_iterator &operator*(){return *this;}
        insert_iterator &operator++(){return *this;}
        insert_iterator &operator++(int){return *this;}
};

template<class Container, class Iterator>
inline insert_iterator<Container> inserter(Container &x, Iterator i){
    using category = typename iterator_traits<Iterator>::iterator_category;
    return insert_iterator<Container>(x, i);
}

template<class Iterator>
class __reverse_iterator {
  template<class It>
  friend bool operator==(const __reverse_iterator<It> &,
                         const __reverse_iterator<It> &);
  template<class It>
  friend bool operator!=(const __reverse_iterator<It> &,
                         const __reverse_iterator<It> &);

 protected:
  Iterator current;//与之对应的正向迭代器

 public:
  using iterator_category = iterator_category_t<Iterator>;
  using value_type = value_type_t<Iterator>;
  using difference_type = difference_type_t<Iterator>;
  using pointer = pointer_t<Iterator>;
  using reference = reference_t<Iterator>;

  using iterator_type = Iterator; //正向迭代器
  using self = __reverse_iterator;//反向迭代器

 public:
  __reverse_iterator() {}
  explicit __reverse_iterator(iterator_type value) : current(value) {}
  __reverse_iterator(const self &value) : current(value.current) {}

  iterator_type base() const { return current; }
  reference operator*() const {
    Iterator temp = current;
    return *--temp;//对逆向迭代器的取值等价于对应的正向迭代器后退一个单位取值
  }

  pointer operator->() const { return &(operator*()); }

  self &operator++() {
    --current;
    return *this;
  }

  self operator++(int) {
    self temp = *this;
    --current;
    return temp;
  }

  self &operator--() {
    ++current;
    return *this;
  }

  self operator--(int) {
    self temp = *this;
    ++current;
    return temp;
  }

  self operator+(difference_type n) const { return self(current - n); }

  self &operator+=(difference_type n) const {
    current -= n;
    return *this;
  }

  self operator-(difference_type n) const { return self(current + n); }

  self &operator-=(difference_type n) const {
    current += n;
    return *this;
  }

  reference operator[](difference_type n) const { return *(*this + n); }

  bool operator==(const self &rhs) const { return current == rhs.current; }
  bool operator!=(const self &rhs) const { return !((*this) == rhs); }
};

template<class Iterator>
inline bool operator==(const __reverse_iterator<Iterator> &lhs,
                       const __reverse_iterator<Iterator> &rhs) {
  return lhs.operator==(rhs);
}

template<class Iterator>
inline bool operator!=(const __reverse_iterator<Iterator> &lhs,
                       const __reverse_iterator<Iterator> &rhs) {
  return !(lhs == rhs);
}

template<class Iterator>
inline bool operator<(const __reverse_iterator<Iterator> &lhs,
                      const __reverse_iterator<Iterator> &rhs) {
  return rhs.base() < lhs.base();
}

template<class Iterator>
inline bool operator>(const __reverse_iterator<Iterator> &lhs,
                      const __reverse_iterator<Iterator> &rhs) {
  return rhs < lhs;
}

template<class Iterator>
inline bool operator<=(const __reverse_iterator<Iterator> &lhs,
                       const __reverse_iterator<Iterator> &rhs) {
  return !(rhs < lhs);
}

template<class Iterator>
inline bool operator>=(const __reverse_iterator<Iterator> &lhs,
                       const __reverse_iterator<Iterator> &rhs) {
  return !(lhs < rhs);
}

template<class Iterator>
inline typename __reverse_iterator<Iterator>::difference_type operator-(
    const __reverse_iterator<Iterator> &lhs,
    const __reverse_iterator<Iterator> &rhs) {
  return rhs.base() - lhs.base();
}

template<class Iterator>
inline __reverse_iterator<Iterator> operator+(
    typename __reverse_iterator<Iterator>::difference_type n,
    const __reverse_iterator<Iterator> &x) {
  return __reverse_iterator<Iterator>(x.base() - n);
}

// stream:input_stream,output_stream

template<class T, class Distance = ptrdiff_t>
class istream_iterator {
 protected:
  std::istream *stream;
  T value;
  bool end_marker;
  void read() {
    end_marker = (*stream) ? true : false;
    if (end_marker) *stream >> value;
    //完成输入后，stream狀態可能发生了改變，再次判定
    end_marker = (*stream) ? true : false;
  }

 public:
  using iterator_category = input_iterator_tag;
  using value_tyep = T;
  using difference_type = Distance;
  using pointer = const T *;//只能读取
  using reference = const T &;

  istream_iterator() : stream(&std::cin), end_marker(false) {}
  istream_iterator(std::istream &s) : stream(&s) { read(); }

  reference operator*() const { return value; }
  pointer operator->() const { return &(operator*()); }

  istream_iterator &operator++() {
    read();
    return *this;
  }

  istream_iterator operator++(int) {
    istream_iterator temp = *this;
    read();
    return temp;
  }
};

template<class T>
class ostream_iterator {
 protected:
  std::ostream *stream;
  const char *interval;//输出间隔符

 public:
  using iterator_category = output_iterator_tag;
  using value_tyep = void;
  using difference_type = void;
  using pointer = void;
  using reference = void;

  ostream_iterator() : stream(&std::cout), interval(nullptr) {}
  ostream_iterator(std::ostream &s, const char *c)
      : stream(&s), interval(c) {}

  ostream_iterator &operator=(const T &value) {
    *stream << value;
    if (interval) *stream << interval;
    return *this;
  }

  ostream_iterator &operator*() { return *this; }
  ostream_iterator &operator++() { return *this; }
  ostream_iterator operator++(int) { return *this; }
};






}
