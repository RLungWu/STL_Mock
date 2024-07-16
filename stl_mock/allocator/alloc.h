#pragma once

#include <cstddef>
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

    //第二級配置器 => 對 malloc_alloc 的封裝
    class _default_alloc{
        private:
            //將bytes 上調到8的倍數
            static size_t ROUND_UP(size_t bytes){
                return (((bytes) + static_cast<size_t>(ALIGN) - 1) & ~(static_cast<size_t>(ALIGN) - 1));
            }

            // Free list的節點，由於union的特性，不需要占用額外的 memory
            union obj{
                union obj *free_list_link_;  //指向下一個節點
                char client_data_[1];        //客戶端可用的 memory
            };

            static obj *volatile free_list[_freelist_setting::NFREELISTS]; //Free list
            
            //決定使用第幾個節點
            static size_t FREELIST_INDEX(size_t bytes){
                return (((bytes) + static_cast<size_t>(ALIGN) - 1) / static_cast<size_t>(ALIGN) - 1);
            }

            static void *refill(size_t n);

            static char *chunk_alloc(size_t size, int &nobjs);

            // Chunk allocation state
            static char *start_free_;    //記錄 memory pool 的起始位置
            static char *end_free_;      //記錄 memory pool 的結束位置
            static size_t heap_size_;    //記錄 memory pool 的大小

            public:
                static void *allocate(size_t n);
                static void deallocate(void *p, size_t n);
                static void *reallocate(void *p, size_t old_size, size_t new_size);
    };

    // 當free_list 沒有可用區域時，重新填充空間
    // 新空間取自 memory pool，並將空間分配給 free_list
    // 預設的空間大小為 20個節點
    inline void *_default_alloc::refill(size_t n){
        int nobjs = 20;

        char *chunk = chunk_alloc(n, nobjs);

        obj *volatile *my_free_list;
        obj *result;
        obj *current_obj, *next_obj;


        //如果只獲取了一個區塊，我們就直接分配給使用者，不加入 free_list
        // 因為一個區塊就夠使用者使用了
        if(nobjs == 1){
            return chunk;
        }
        my_free_list = free_list + FREELIST_INDEX(n);

        result = reinterpret_cast<obj *>(chunk);
        *my_free_list = next_obj = reinterpret_cast<obj *>(chunk + n);

        for(int i = 1; ; ++i){
            current_obj = next_obj;
            next_obj = reinterpret_cast<obj *>(reinterpret_cast<char *>(next_obj) + n);
            if(nobjs - 1 == i){
                current_obj->free_list_link_ = nullptr;
                break;
            }else{
                current_obj->free_list_link_ = next_obj;
            }
        }
        return result;
    }

    inline char *_default_alloc::chunk_alloc(size_t size, int &nobjs){
        char *result;
        size_t total_bytes = size * nobjs;
        size_t bytes_left = end_free_ - start_free_;
        if(bytes_left >= total_bytes){      //足夠
            result = start_free_;
            start_free_ += total_bytes;
            return result;
        }else if(bytes_left >= size){       //只能提供一個
            nobjs = static_cast<int>(bytes_left / size);
            total_bytes = size * nobjs;
            result = start_free_;
            start_free_ += total_bytes;
            return result;
        }else{      //無法提供
            size_t bytes_to_get = 2 * total_bytes + ROUND_UP(heap_size_ >> 4);
            if(bytes_left > 0){
                obj *volatile *my_free_list = free_list + FREELIST_INDEX(bytes_left);
                reinterpret_cast<obj *>(start_free_)->free_list_link_ = *my_free_list;
                *my_free_list = reinterpret_cast<obj *>(start_free_);
            }
            //配置 heap 空間以補充 memory pool
            start_free_ = reinterpret_cast<char *>(malloc(bytes_to_get));
            if(!start_free_){   //heap 空間不夠，malloc 失敗
                obj *volatile *my_free_list, *p;

                for(size_t i = size; i < static_cast<size_t>(MAX_BYTES); i += static_cast<size_t>(ALIGN)){
                    my_free_list = free_list + FREELIST_INDEX(i);
                    p = *my_free_list;
                    if(p){  //存在足夠大的 memory
                        *my_free_list = p->free_list_link_;
                        start_free_ = reinterpret_cast<char *>(p);
                        end_free_ = start_free_ + i;
                        return chunk_alloc(size, nobjs);
                    }        
                }
                end_free_ = nullptr;    //到處都找不到 memory
                start_free_ = reinterpret_cast<char *>(malloc_alloc::allocate(bytes_to_get));
            }

            heap_size_ += bytes_to_get;             //以占用的
            end_free_ = start_free_ + bytes_to_get;
            
            return chunk_alloc(size, nobjs);
        }
    }

    inline void *_default_alloc::allocate(size_t n){
        obj *volatile *my_free_list;
        obj *result;

        //如果大於 128 bytes，則使用第一級配置器
        //請求太大
        if(n > static_cast<size_t>(MAX_BYTES)){
            return malloc_alloc::allocate(n);
        }

        my_free_list = free_list + FREELIST_INDEX(n);
        result = *my_free_list;
        if(result == nullptr){
            return refill(ROUND_UP(n));
        }
        *my_free_list = result->free_list_link_;
        return result;
    }

    inline void _default_alloc::deallocate(void *p, size_t n){
        if(n > static_cast<size_t>(MAX_BYTES)){
            malloc_alloc::deallocate(p, n);
            return;
        }

        //尋找對應的free list
        obj *q = reinterpret_cast<obj *>(p);
        obj *volatile *my_free_list = free_list + FREELIST_INDEX(n);

        //調整free list
        q->free_list_link_ = *my_free_list;
        *my_free_list = q;
    }

    inline void *_default_alloc::reallocate(void *p, size_t old_size, size_t new_size){
        deallocate(p, old_size);
        p = allocate(new_size);
        return p;
    }

    



}