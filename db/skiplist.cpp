//
// Created by 杨翊铎 on 2023/4/6.
//

#include "skiplist.h"

namespace kv {
    template<typename Key, class Comparator, typename Allocator>
    void SkipList<Key, Comparator, Allocator>::Insert(const Key &key) {
        Node* preStart[SkipListOption::kMaxHeight] { nullptr };
        Node* node = findGreaterOrEqual(key, preStart);
        if (nullptr != node) {
            if (equal(node->key, key)) {
                //exist
                return;
            }
        }

        int32_t height = randomHeight();
        int32_t cur_max_height = getMaxHeight();

        if (height > cur_max_height) {
            for (int32_t index = cur_max_height; index < height; ++index) {
                preStart[index] = head_;
            }
            cur_height_.store(height);
        }

        Node* n = newNode(key, height);
        /*
         * 1
         * ｜
         * 1    2   3               9
         * ｜   ｜   ｜              ｜
         * 1 —— 2 —— 3 —— 4 —— 5 —— 9 —— 10
         * insert 6
         */
        for (int32_t index = 0; index < height; ++index){
            n->NoBarrier_Next(index, preStart[index]->NoBarrier_Next(index));
            preStart[index]->NoBarrier_SetNext(index, n);
        }
    }

    template<typename Key, class Comparator, typename Allocator>
    bool SkipList<Key, Comparator, Allocator>::equal(const Key &a, const Key &b) {
        return comparator_.Compare(a, b) == 0;
    }


    template<typename Key, class Comparator, typename Allocator>
    typename SkipList<Key, Comparator, Allocator>::Node* SkipList<Key, Comparator, Allocator>::newNode(const Key &key,
                                                                                              int32_t height) {
        char* node_memory = (char*)arena_.Allocate(
                sizeof(Node) + sizeof(std::atomic<Node*>) * (height - 1)
                );
        return new (node_memory) Node(key);

    }

    //构造P=3/4的几何分布
    template<typename Key, class Comparator, typename Allocator>
    int32_t SkipList<Key, Comparator, Allocator>::randomHeight() {
        int32_t height = 1;
        while (height < SkipListOption::kMaxHeight &&
                (randomUtil_.GetSimpleRandomNum() % SkipListOption::kBranching) == 0) {
            height++;
        }
        return height;
    }

    template<typename Key, class Comparator, typename Allocator>
    int32_t SkipList<Key, Comparator, Allocator>::getMaxHeight() {
        return cur_height_.load(std::memory_order_relaxed);
    }

    template<typename Key, class Comparator, typename Allocator>
    bool SkipList<Key, Comparator, Allocator>::keyIsAfterNode(const Key &key, SkipList::Node *n) {
        return (nullptr != n && comparator_.Compare(n->key, key) < 0);
    }

    template<typename Key, class Comparator, typename Allocator>
    typename SkipList<Key, Comparator, Allocator>::Node* SkipList<Key, Comparator, Allocator>::findGreaterOrEqual(
            const Key &key, SkipList::Node **preStart) {
        Node* cur = head_;
        int32_t level = getMaxHeight();
        while (true) {
            Node* next = cur->Next(level);
            if (keyIsAfterNode(key, next)) {
                cur = next;
            } else {
                if (preStart != nullptr) {
                    preStart[level] = cur;
                }
                if (level == 0) return next;
                level--;
            }
        }
    }

    template<typename Key, class Comparator, typename Allocator>
    typename SkipList<Key, Comparator, Allocator>::Node* SkipList<Key, Comparator, Allocator>::findLessThan(
            const Key &key) {
        Node* cur = head_;
        int32_t level = getMaxHeight() - 1;
        while (true) {
            Node* next = cur->Next(level);
            int32_t cmp = (next == nullptr) ? 1 : comparator_.Compare(next->key, key);
            if (cmp >= 0) {
                if (level == 0) {
                    return cur;
                } else {
                    level--;
                }
            } else {
                cur = next;
            }
        }
    }

    template<typename Key, class Comparator, typename Allocator>
    typename SkipList<Key, Comparator, Allocator>::Node* SkipList<Key, Comparator, Allocator>::findLast() {
        Node* cur = head_;
        static constexpr uint32_t kBaseLevel = 0;
        while (true) {
            Node* next = cur->Next(kBaseLevel);
            if (nullptr == next) {
                return cur;
            }
            cur = next;
        }
    }
}