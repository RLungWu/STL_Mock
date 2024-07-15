#pragma once

#include <cstdlib>      //malloc and free
#include <cstring>      //memcpy
#include <new>          //bad_alloc

namespace mystl{

//一級配置器 => 對 free 與 malloc 的封裝
class _malloc_alloc{
    public:
        using malloc_handler = void(*)(void);//指向函數的指針
    
    private:
        //以下函數指針處理 memory 不足的情況
        //oom = out of memory
        static void *oom_malloc(size_t);
        static void *oom_realloc(void*, size_t);
        static malloc_handler __malloc_alloc_oom_handler;
    
    public:
        static void *allocate(size_t n){
            void *result = malloc(n);
            if(result == nullptr){
                result = oom_malloc(n);
            }
            return result;
        }
        
        static void deallocate(void *p, size_t){
            free(p);
        }

        static void *reallocate(void *p, size_t, size_t new_size){
            void *result = realloc(p, new_size);
            if(result == nullptr){
                result = oom_realloc(p, new_size);
            }
            return result;
        }

        // 模擬 set_new_handler
        // 我們並沒有使用 new 與 delete 來配置 memory
        // 因此無法使用 set_new_handler
        static malloc_handler set_malloc_handler(malloc_handler f){
            malloc_handler old = __malloc_alloc_oom_handler;
            __malloc_alloc_oom_handler = f;
            return old;
        }
};
    inline void *_malloc_alloc::oom_malloc(size_t n){
        malloc_handler new_alloc_handler;
        void *result;

        while(true){
            new_alloc_handler = __malloc_alloc_oom_handler;

            if(new_alloc_handler == nullptr){
                throw std::bad_alloc();
            }

            new_alloc_handler();    //Free memory
            result = malloc(n);
            if(result){
                return result;
            }
        }
    }

    inline void *_malloc_alloc::oom_realloc(void *p, size_t n){
        malloc_handler new_alloc_handler;
        void *result;

        while(true){
            new_alloc_handler = __malloc_alloc_oom_handler;

            if(new_alloc_handler == nullptr){
                throw std::bad_alloc();
            }

            new_alloc_handler();    //Free memory
            result = realloc(p, n);
            if(result){
                return result;
            }
        }
    }

    using malloc_alloc = _malloc_alloc;

    enum _freelist_setting{
        ALIGN = 8,
        MAX_BYTES = 128,
        NFREELISTS = MAX_BYTES / ALIGN,
    };

    



}