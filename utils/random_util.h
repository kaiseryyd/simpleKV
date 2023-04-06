//
// Created by 杨翊铎 on 2023/4/6.
//

#ifndef KV_RANDOM_UTIL_H
#define KV_RANDOM_UTIL_H
#include <math.h>
#include <cstdint>
#include <ctime>
#include <random>

namespace kv {
    class RandomUtil final {
    public:
        ~RandomUtil() = default;
        RandomUtil(uint32_t seed = 0) : seed_val_(seed) {
            if (seed_val_ > 0) {
                engine_.seed(seed_val_);
            } else {
                engine_.seed(std::time(0));
            }
        }
        int64_t GetSimpleRandomNum() { return rand(); }
        int64_t GetRandomNum() { return engine_(); }
    private:
        uint32_t seed_val_ = 0;
        std::default_random_engine engine_;
    };
}
#endif //KV_RANDOM_UTIL_H
