//
// Created by 杨翊铎 on 2023/6/7.
//

#ifndef KV_CACHE_NODE_H
#define KV_CACHE_NODE_H

#include <functional>
namespace kv {
    template <typename KeyType, typename ValueType>
    struct CacheNode {
        KeyType key;
        ValueType* value;

        // 引用计数
        uint32_t refs = 0;
        uint32_t hash = 0;

        bool in_cache = false;
        uint64_t last_access_time = 0;

        uint64_t ttl = 0;
    };
}

#endif //KV_CACHE_NODE_H
