//
// Created by 杨翊铎 on 2023/4/23.
//

#ifndef KV_ALLOC_H
#define KV_ALLOC_H
#include <stdint.h>
#include <atomic>
namespace kv {

    struct FreeList {
        struct FreeList* next;
    };

    class FreeListAlloc final {
    public:
        FreeListAlloc() = default;
        ~FreeListAlloc();

        void* Allocate(int32_t n);
        void Deallocate(void* p, int32_t n);
        void* Reallocate(void* p, int32_t old_size, int32_t new_size);

    private:
        int32_t roundUp(int32_t bytes);
        int32_t freeListIndex(int32_t bytes);
        void* refill(int32_t bytes);
        char* chunkAlloc(int32_t bytes, int32_t& obj);

    private:
        static constexpr uint32_t alignBytes = 8;
        static constexpr uint32_t smallObject = 4096;
        static constexpr uint32_t freeListMaxNum = smallObject / alignBytes;

        char* free_list_start_pos_ = nullptr;
        char* free_list_end_pos_ = nullptr;
        int32_t heap_size_ = 0;
        FreeList* freeList_[freeListMaxNum] = {nullptr };
        std::atomic<uint32_t> memory_usage_;
    };
}

#endif //KV_ALLOC_H
