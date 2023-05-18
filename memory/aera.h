//
// Created by 杨翊铎 on 2023/4/24.
//

#ifndef KV_AERA_H
#define KV_AERA_H
#include <cstdint>
#include <vector>
#include <atomic>

namespace kv {
    class VectorAlloc {
    public:
        VectorAlloc();

        VectorAlloc(const VectorAlloc&) = delete;
        VectorAlloc& operator=(const VectorAlloc&) = delete;

        ~VectorAlloc();

        void* Allocate(uint32_t bytes);
    private:
        char* AllocateFallback(uint32_t bytes);
        char* AllocateNewBlock(uint32_t block_types);

        char* alloc_ptr_;
        uint32_t alloc_bytes_remaining_;

        std::vector<char*> blocks_;
        std::atomic<uint32_t> memory_usage_;
    };
}



#endif //KV_AERA_H
