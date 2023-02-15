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
    public:
        static constexpr size_t npos = -1;

        HashMap(size_t size=4096) : m_size(0) {
            m_buckets.resize(size);
        }
        HashMap(const HashMap& other) : m_size(0) {
            m_buckets.resize(other.m_buckets.size());
            for (auto& bucket : other.m_buckets){
                for (auto& it : bucket){
                    insert(it.value);
                }
            }
        }
        HashMap(HashMap&& other) : m_size(other.m_size), m_buckets(std::move(other.m_buckets)) {
            other.m_size = 0;
        }
        virtual ~HashMap(){
            clear();
        }

        struct Container {
            Container() {}
            Container(const Container& other)  : value(other.value), previous(other.previous), next(other.next) {}
            Container(const Container&& other) : value(std::move(other.value)), previous(other.previous), next(other.next) {}
            Container(const K& k, const V& v)  : value(k, v) {}
            Container(const K&& k, const V&& v): value(std::move(k), std::move(v)) {}
            Container(const cave::Pair<K, V>& pair)  : value(pair) {}
            Container(const cave::Pair<K, V>&& pair) : value(std::move(pair)) {}
            
            cave::Pair<K, V>& operator*() {
                return value;
            }
            cave::Pair<K, V>* operator->() const {
                return &value;
            }

            bool operator==(const Container& other) const {
                return value == other.value;
            }

            bool operator!=(const Container& other) const {
                return !(*this == other);
            }

            cave::Pair<K, V> value;

            Container* previous = nullptr;
            Container* next = nullptr;
        };

        struct Iterator {
            Iterator() {}
            Iterator(Container* container) : element(container) {}
            Iterator(const Iterator& other) : element(other.element) {}

            cave::Pair<K, V>& operator*() {
                // Should I do something if element is null? Perhaps throw an exception...?
                return element->value;
            }
            cave::Pair<K, V>* operator->() const {
                if (element){
                    return &(element->value);
                }
                return nullptr;
            }

            Iterator& operator++() {
                if (element){
                    element = element->next;
                }
                return *this;
            }
            Iterator& operator--() {
                if (element){
                    element = element->previous;
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
                if (element && other.element){
                    return *element == *other.element;
                }
                else if (element == nullptr && other.element == nullptr){
                    return true;
                }
                return false;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            Container* element;
        };

        Iterator begin() {
            return Iterator(m_firstContainer);
        }
        const Iterator begin() const {
            return Iterator(m_firstContainer);
        }

        Iterator end() {
            return Iterator(nullptr);
        }
        const Iterator end() const {
            return Iterator(nullptr);
        }

        void insert(const cave::Pair<K, V>& pair){
            const size_t hs = bucket(pair.first);
            m_buckets[hs].emplaceBack(pair);
            m_size++;

            updateNewContainerForIteration(hs);
        }
        void insert(cave::Pair<K, V>&& pair){
            const size_t hs = bucket(pair.first);
            m_buckets[hs].emplaceBack(pair);
            m_size++;

            updateNewContainerForIteration(hs);
        }
        void insert(const K& key, const V& value){
            const size_t hs = bucket(key);
            m_buckets[hs].emplaceBack(key, value);
            m_size++;

            updateNewContainerForIteration(hs);
        }
        void insert(const K& key, V&& value){
            const size_t hs = bucket(key);
            m_buckets[hs].emplaceBack(key, value);
            m_size++;

            updateNewContainerForIteration(hs);
        }
        void erase(const K& key) {
            const size_t hs = bucket(key);

            size_t id = 0;
            bool found = false;
            for (auto& e : m_buckets[hs]){
                if (e.value.first == key) {
                    if (e.previous){
                        e.previous->next = e.next;
                    }
                    if (e.next){
                        e.next->previous = e.previous;
                    }
                    if (m_firstContainer == &e){
                        m_firstContainer = e.next;
                    }

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
                if (e.value.first == key) {
                    return e.value.second;
                }
            }

            m_buckets[hs].emplaceBack(key, V());
            m_size++;
            updateNewContainerForIteration(hs);

            return m_buckets[hs].back().value.second;
        }

        V& at(const K& key) {
            const size_t hs = bucket(key);

            size_t id = 0;
            bool found = false;
            for (auto& e : m_buckets[hs]){
                if (e.value.first == key) {
                    found = true;
                    break;
                }
                id++;
            }
            if (!found){
                throw cave::OutOfRangeException();
            }
            return m_buckets[hs][id].value.second;
        }

        const V& at(const K& key) const {
            const size_t hs = bucket(key);

            size_t id = 0;
            bool found = false;
            for (auto& e : m_buckets[hs]){
                if (e.value.first == key) {
                    found = true;
                    break;
                }
                id++;
            }
            if (!found){
                throw cave::OutOfRangeException();
            }
            return m_buckets[hs][id].value.second;
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
            m_firstContainer = nullptr;
            m_size = 0;
        }

    private:    
        void updateNewContainerForIteration(size_t hs) {
            auto& back = m_buckets[hs].back();
            back.next = m_firstContainer;
            
            if (m_firstContainer){
                m_firstContainer->previous = &back;
            }
            m_firstContainer = &back;
        }

        size_t hash(const K& key) const {
            return std::hash<K>{}(key);
        }

        size_t m_size;
        Vector<Vector<Container>> m_buckets;

        Container* m_firstContainer = nullptr;
    };
}

#endif // !CAVE_STD_HASH_TABLE_H