//
// Created by 杨翊铎 on 2023/4/24.
//

#include "aera.h"

namespace kv {
    VectorAlloc::VectorAlloc() : alloc_ptr_(nullptr), alloc_bytes_remaining_(0), memory_usage_(0) {}
    VectorAlloc::~VectorAlloc() {
        for (uint32_t i = 0; i < blocks_.size(); ++i) {
            delete[] blocks_[i];
        }
    }

    char* VectorAlloc::AllocateFallback(uint32_t bytes) {
        auto tmp_ptr = AllocateNewBlock(bytes * 2);
        if (!alloc_ptr_) alloc_ptr_ = tmp_ptr;
        alloc_bytes_remaining_ = bytes * 2;
        return alloc_ptr_;
    }

    void* VectorAlloc::Allocate(uint32_t bytes) {
        const int align = (sizeof(void*) > 8) ? sizeof(void*) : 8;
        static_assert((align & (align - 1)) == 0, "Pointer size should be a power of 2");

        //上调8倍
        uint32_t current_mod = reinterpret_cast<uintptr_t>(alloc_ptr_) & (align - 1);
        uint32_t slop = (current_mod == 0 ? 0 : align - current_mod);
        uint32_t needed = bytes + slop;

        char* result = nullptr;
        if (needed <= alloc_bytes_remaining_) {
            result = alloc_ptr_ + slop;
            alloc_ptr_ += needed;
            alloc_bytes_remaining_ -= needed;
        } else {
            AllocateFallback(bytes);
            return Allocate(bytes);
        }
        return result;
    }

    char* VectorAlloc::AllocateNewBlock(uint32_t block_types) {
        char* result = new char[block_types];
        blocks_.push_back(result);
        memory_usage_.fetch_add(block_types + sizeof(char*), std::memory_order_relaxed);
        return result;
    }
}