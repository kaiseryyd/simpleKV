//
// Created by 杨翊铎 on 2023/4/6.
//

#ifndef KV_SKIPLIST_H
#define KV_SKIPLIST_H
#include <assert.h>
#include <atomic>

#include "../utils/random_util.h"

namespace kv {
    struct SkipListOption {
        static constexpr int32_t kMaxHeight = 20;
        static constexpr unsigned int kBranching = 4;
    };

    template<typename Key, class Comparator, typename Allocator>
    class SkipList final {
        struct Node;
    public:
        SkipList(Comparator comparator);

        //禁止拷贝 复制
        SkipList(const SkipList &) = delete;

        SkipList &operator=(const SkipList &) = delete;

        void Insert(const Key& key);

    private:
        Node* newNode(const Key& key, int32_t height);
        bool equal(const Key& a, const Key& b);
        int32_t randomHeight();
        int32_t getMaxHeight();
        bool keyIsAfterNode(const Key& key, Node* n);
        Node* findGreaterOrEqual(const Key& key, Node** preStart);
        Node* findLessThan(const Key& key);
        Node* findLast();

    private:
        Node* head_ = nullptr;
        std::atomic<int32_t> cur_height_;
        Allocator arena_;
        RandomUtil randomUtil_;
        Comparator comparator_;

    };


    template<typename Key, class Comparator, typename Allocator>
    struct SkipList<Key, Comparator, Allocator>::Node {
        explicit Node(const Key& k) : key(k) {}
        const Key key;

        Node* Next(int32_t n) {
            return next_[n].load(std::memory_order_acquire); // why
        }

        void SetNext(int n, Node* x) {
            assert(n >= 0);
            next_[n].store(x, std::memory_order_release);
        }

        Node* NoBarrier_Next(int n) {
            return next_[n].load(std::memory_order_relaxed);
        }

        void NoBarrier_SetNext(int n, Node* x) {
            next_[n].store(x, std::memory_order_relaxed);
        }

    private:
        std::atomic<Node*> next_[1];
    };

}
#endif //KV_SKIPLIST_H
