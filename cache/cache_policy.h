//
// Created by 杨翊铎 on 2023/6/7.
//

#ifndef KV_CACHE_POLICY_H
#define KV_CACHE_POLICY_H
#include "cache_node.h"

namespace kv {
    template <typename KeyType, typename ValueType>
    class CachePolicy {
    public:
        virtual ~CachePolicy() = default;
        virtual void Insert(const KeyType& key, ValueType* value, uint32_t ttl = 0) = 0;
        virtual CacheNode<KeyType, ValueType>* Get(const KeyType& key) = 0;
        virtual void Release(CacheNode<KeyType, ValueType>* node) = 0;
        virtual void Prune() = 0;
        virtual void Erase(const KeyType& key) = 0;
        virtual void RegistCleanHandle(std::function<void(const KeyType& key, ValueType* value)> destructor) = 0;
    };
}

#endif //KV_CACHE_POLICY_H
