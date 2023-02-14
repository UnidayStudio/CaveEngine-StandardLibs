#ifndef CAVE_STD_HASH_TABLE_H
#define CAVE_STD_HASH_TABLE_H

#include <cstddef> // size_t
#include <utility> // std::move, std::forward
#include <cstdlib> // malloc, free

#include "Containers/Vector.h"
#include "Containers/Pair.h"
#include "Containers/Exception.h"

#include <iostream>


namespace cave {
    template <typename K, typename V>
    class HashMap{
        friend class Iterator;
    public:
        static constexpr size_t npos = -1;

        HashMap(size_t size=4096) : m_size(0) {
            m_buckets.resize(size);
        }
        HashMap(const HashMap& other) : m_size(0) {
            m_buckets.resize(other.m_buckets.size());
            for (auto& bucket : other.m_buckets){
                for (auto& it : bucket){
                    insert(it);
                }
            }
        }
        HashMap(HashMap&& other) : m_size(other.m_size), m_buckets(std::move(other.m_buckets)) {
            other.m_size = 0;
        }
        virtual ~HashMap(){
            clear();
        }

        struct Iterator {
            Iterator(const Iterator& other) : m_owner(other.m_owner), m_bucket(other.m_bucket), m_slot(other.m_slot) {}
            Iterator(const HashMap* owner, size_t bucket, size_t slot) : m_owner(owner), m_bucket(bucket), m_slot(slot) {}

            cave::Pair<K, V>& operator*() {
                return m_owner->m_buckets[m_bucket][m_slot];
            }
            cave::Pair<K, V>* operator->() const {
                return &m_owner->m_buckets[m_bucket][m_slot];
            }

            Iterator& operator++() {
                m_slot++;
                if (m_slot >= m_owner->m_buckets[m_bucket].size()){
                    const size_t ownerBucketCount = m_owner->m_buckets.size();
                    m_slot = 0;
                    do {
                        m_bucket++;
                        if (m_bucket >= ownerBucketCount){
                            m_bucket = npos;
                            m_slot = npos;
                            break;
                        }
                    } while (m_owner->m_buckets[m_bucket].size() == 0);
                }
                return *this;
            }
            Iterator& operator--() {
                if (m_bucket == npos){
                    m_bucket = m_owner->m_buckets.size() - 1;
                    m_slot = m_owner->m_buckets.back().size() - 1;
                }
                else {
                    if (m_slot == 0 && m_bucket > 0){
                        m_bucket--;
                        m_slot = m_owner->m_buckets[m_bucket].size() - 1;
                    }
                    else {
                        m_slot--;
                    }
                }
                while (m_bucket >= 0 && m_owner->m_buckets[m_bucket].size() == 0){
                    m_bucket--;
                    if (m_bucket >= 0){
                        m_slot = m_owner->m_buckets[m_bucket].size() - 1;
                    }
                }
                return *this;
            }

            //--

            Iterator operator+(int i) const {
                Iterator copy(*this);
                for (int j = 0; j < i; j++) {
                    copy++;
                }
                return copy;
            }
            Iterator operator-(int i) const {
                Iterator copy(*this);
                for (int j = 0; j < i; j++) {
                    copy--;
                }
                return copy;
            }
            Iterator& operator+=(int i) {
                for (int j = 0; j < i; j++) {
                    (*this)++;
                }
                return *this;
            }

            Iterator& operator-=(int i) {
                for (int j = 0; j < i; j++) {
                    (*this)--;
                }
                return *this;
            }

            Iterator operator++(int) {
                Iterator copy(*this);
                ++(*this);
                return copy;
            }
            Iterator operator--(int) {
                Iterator copy(*this);
                --(*this);
                return copy;
            }

            bool operator==(const Iterator& other) const {
                return m_owner == other.m_owner && m_bucket == other.m_bucket && m_slot == other.m_slot;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

        private:
            const HashMap* m_owner;
            size_t m_bucket;
            size_t m_slot;
        };

        Iterator begin() {
            size_t b = 0;
            for (auto& bucket : m_buckets){
                if (bucket.size() > 0){
                    return Iterator(this, b, 0);
                }
                b++;
            }
            return Iterator(this, npos, npos);
        }
        const Iterator begin() const {
            return Iterator((const HashMap*)this, npos, npos);
        }

        Iterator end() {
            return Iterator(this, npos, npos);
        }
        const Iterator end() const {
            return Iterator((const HashMap*)this, npos, npos);
        }

        void insert(const cave::Pair<K, V>& pair){
            const size_t hs = bucket(pair.first);
            m_buckets[hs].emplaceBack(pair);
            m_size++;
        }
        void insert(cave::Pair<K, V>&& pair){
            const size_t hs = bucket(pair.first);
            m_buckets[hs].emplaceBack(pair);
            m_size++;
        }
        void insert(const K& key, const V& value){
            const size_t hs = bucket(key);
            m_buckets[hs].emplaceBack(key, value);
            m_size++;
        }
        void insert(const K& key, V&& value){
            const size_t hs = bucket(key);
            m_buckets[hs].emplaceBack(key, value);
            m_size++;
        }
        void erase(const K& key) {
            const size_t hs = bucket(key);

            size_t id = 0;
            bool found = false;
            for (auto& e : m_buckets[hs]){
                if (e.first == key) {
                    found = true;
                    break;
                }
                id++;
            }
            if (found){
                m_buckets[hs].erase(id);
                m_size--;
            }
        }

        size_t count(const K& key) const {
            try {
                at(key);
                return 1;
            }
            catch (cave::OutOfRangeException&){
                return 0;
            }
        }
        bool exists(const K& key) const {
            return count(key) == 1;
        }

        V& operator[](const K& key) {
            const size_t hs = bucket(key);

            for (auto& e : m_buckets[hs]){
                if (e.first == key) {
                    return e.second;
                }
            }

            m_buckets[hs].emplaceBack(key, V());
            m_size++;
            return m_buckets[hs].back().second;
        }

        V& at(const K& key) {
            const size_t hs = bucket(key);

            size_t id = 0;
            bool found = false;
            for (auto& e : m_buckets[hs]){
                if (e.first == key) {
                    found = true;
                    break;
                }
                id++;
            }
            if (!found){
                throw cave::OutOfRangeException();
            }
            return m_buckets[hs][id].second;
        }

        const V& at(const K& key) const {
            const size_t hs = bucket(key);

            size_t id = 0;
            bool found = false;
            for (auto& e : m_buckets[hs]){
                if (e.first == key) {
                    found = true;
                    break;
                }
                id++;
            }
            if (!found){
                throw cave::OutOfRangeException();
            }
            return m_buckets[hs][id].second;
        }

        size_t size() const {
            return m_size;
        }
        bool empty() const {
            return m_size == 0;
        }

        size_t bucketCount() const {
            return m_buckets.size();
        }
        size_t bucket(const K& key) const {
            return hash(key) % bucketCount();
        }

        void rehash(size_t n) {
            if (n < m_size){
                return;
            }
            // TODO...
            *(int*)0 = 0;
        }

        void clear(){
            for (auto& bucket : m_buckets){
                bucket.clear();
            }
            m_size = 0;
        }

    private:
        size_t hash(const K& key) const {
            return std::hash<K>{}(key);
        }

        size_t m_size;
        Vector<Vector<cave::Pair<K, V>>> m_buckets;
    };
}

#endif // !CAVE_STD_HASH_TABLE_H