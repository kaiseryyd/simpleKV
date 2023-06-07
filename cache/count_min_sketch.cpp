//
// Created by 杨翊铎 on 2023/6/7.
//
#include "count_min_sketch.h"
#include <algorithm>
#include "../utils/random_util.h"

namespace kv {
    void CountMinSketch::SingleGroup::Init(uint32_t counter_num) {
        if (counter_num == 0) {
            counter_num = 1;
        }
        base_.resize(counter_num);
    }

    void CountMinSketch::SingleGroup::Increment(uint32_t counter_index) {
        if (base_.empty()) {
            return;
        }
        auto value = base_[counter_index].to_ullong();
        if (!base_[counter_index].all()) {
            CounterType tmp(value + 1);
            base_[counter_index] = tmp;
        }
    }
}