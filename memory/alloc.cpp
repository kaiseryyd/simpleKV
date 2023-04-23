//
// Created by 杨翊铎 on 2023/4/23.
//

#include "alloc.h"


namespace kv {
    int32_t FreeListAlloc::roundUp(int32_t bytes) {
        return (bytes + alignBytes - 1) &~ (alignBytes - 1); //调整到8的倍数
    }

    int32_t FreeListAlloc::freeListIndex(int32_t bytes) {
        return (bytes + alignBytes - 1) / alignBytes - 1;
    }

    char* FreeListAlloc::chunkAlloc(int32_t bytes, int32_t &nobjs) {
        char* result;
        uint32_t total_bytes = bytes * nobjs;
        uint32_t bytes_left = free_list_end_pos_ - free_list_start_pos_;
        if (bytes_left >= total_bytes) {
            result = free_list_start_pos_;
            free_list_start_pos_ += total_bytes;
            memory_usage_.fetch_add(total_bytes, std::memory_order::relaxed);
            return result;
        } else if (bytes_left >= bytes) {
            nobjs = bytes_left / bytes;
            total_bytes = bytes * nobjs;
            result = free_list_start_pos_;
            free_list_start_pos_ += total_bytes;
            memory_usage_.fetch_add(total_bytes, std::memory_order::relaxed);
            return result;
        } else {
            //重新分配内存
            int32_t bytes_to_get = 2 * total_bytes + roundUp(heap_size_ >> 4);
            if (bytes_left > 0) {
                FreeList* volatile* cur_free_list = freeList_ + freeListIndex(bytes_left);
                ((FreeList*)free_list_start_pos_)->next = *cur_free_list;
                *cur_free_list = (FreeList*)free_list_start_pos_;
            }
            free_list_start_pos_ = (char*)malloc(bytes_to_get);
            if (free_list_start_pos_ == nullptr) {
                FreeList* volatile* my_free_list = nullptr, *p = nullptr;
                for (int32_t index = bytes; index <= smallObject; index += alignBytes) {
                    my_free_list = freeList_ + freeListIndex(index);
                    p = *my_free_list;
                    if (p) {
                        *my_free_list = p->next;
                        free_list_start_pos_ = (char*)p;
                        free_list_end_pos_ = free_list_start_pos_ + index;
                        return chunkAlloc(bytes, nobjs);
                    }
                }
                free_list_end_pos_ = nullptr;
                free_list_start_pos_ = (char*) malloc(bytes_to_get);
                if (free_list_start_pos_ == nullptr) {
                    exit(1);
                }
            }
            heap_size_ += bytes_to_get;
            free_list_end_pos_ = free_list_start_pos_ + bytes_to_get;
            memory_usage_.fetch_add(bytes_to_get, std::memory_order_relaxed);
            return chunkAlloc(bytes, nobjs);
        }
    }




}