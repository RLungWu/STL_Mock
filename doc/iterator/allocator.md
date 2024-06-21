# Allocator Note
## 2.1 Standard Interface
1. allocator::value_type
2. allocator::pointer
3. allocator::const_pointer
4. allocator::reference
5. allocator::const_reference
6. allocator::size_type
7. allocator::difference_type
8. allocator::rebind
   - nested class template
   - represent another type
9.  allocator::allocator()                  
    -  default constructor
10. allocator::allocator(const allocator&)  
    - copy constructor
11. template<class U> allocator::allocator(const allocator<U>&)
12. allocator::~allocator()
13. pointer allocator::address(reference x) const  //~= &x
14. const_pointer allocator::allocate(size_type n, const void* = 0)
    - Allocate memory which can store n numbers of T
15. void allocator::deallocate(pointer p, size_type n)
16. size_type allocator::max_size() const
17. void allocator::construct(pointer p, const T& x)
18. void allocator::destroy(pointer p)
