#include "alloc.h"

namespace mystl{
    char *_default_alloc::start_free_ = nullptr;
    char *_default_alloc::end_free_ = nullptr;
    size_t _default_alloc::heap_size_ = 0;

    _default_alloc::obj * volatile _default_alloc::free_list[_freelist_setting::NFREELISTS] = {
        nullptr, nullptr, nullptr, nullptr, nullptr, 
        nullptr, nullptr, nullptr, nullptr, nullptr,
    };

    typename _malloc_alloc::malloc_handler _malloc_alloc::__malloc_alloc_oom_handler = nullptr;
}