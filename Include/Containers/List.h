#ifndef CAVE_STD_LIST_H
#define CAVE_STD_LIST_H

#include <cstddef> // size_t
#include <utility> // std::move, std::forward
#include <cstdlib> // malloc, free
#include <cstring> // memset
#include <initializer_list>

#include "Containers/Exception.h"


namespace cave {
    template <typename T>
    class List{
    private:
        struct Node;
    public:
        List() : m_first(nullptr), m_last(nullptr), m_size(0) {}
        List(std::initializer_list<T> initList) : m_first(nullptr), m_last(nullptr), m_size(0) {
            for (const auto& obj: initList){
                pushBack(obj);
            }
        }
        List(const List& other) : m_first(nullptr), m_last(nullptr), m_size(0){
            for (size_t i=0; i< other.size(); i++){
                emplaceBack(other.at(i));
            }
        }
        List(List&& other) noexcept : m_first(other.m_first), m_last(other.m_last), m_size(other.m_size) {
            other.m_first = nullptr;
            other.m_last = nullptr;
            other.m_size = 0;
        }
        virtual ~List(){
            clear();
        }

        struct Iterator {
            Iterator(const Iterator& other) : current(other.current), endPrev(other.endPrev) {}
            Iterator(Node* current, Node* end=nullptr) : current(current), endPrev(end) {}

            T& operator*() {
                return current->value;
            }

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

            Iterator& operator++() {
                if (current->next == nullptr){
                    endPrev = current;
                }
                current = current->next;
                return *this;
            }

            Iterator operator++(int) {
                Iterator copy(*this);
                ++(*this);
                return copy;
            }

            Iterator& operator--() {
                if (current){
                    current = current->prev;
                }
                else {
                    current = endPrev;
                }
                return *this;
            }

            Iterator operator--(int) {
                Iterator copy(*this);
                --(*this);
                return copy;
            }

            bool operator==(const Iterator& other) const {
                return current == other.current;
            }

            bool operator!=(const Iterator& other) const {
                return !(*this == other);
            }

            Node* current;
            Node* endPrev;
        };

        Iterator begin() {
            return Iterator(m_first);
        }
        const Iterator begin() const {
            return Iterator(m_first);
        }

        Iterator end() {
            return Iterator(nullptr, m_last);
        }
        const Iterator end() const {
            return Iterator(nullptr, m_last);
        }

        T&       front() {
            return m_first->value;
        }
        const T& front() const {
            return m_first->value;
        }
        T&       back() {
            return m_last->value;
        }
        const T& back() const {
            return m_last->value;
        }

        T operator[](size_t pos) const {
            return getNodeInternal(pos)->value;
        }
        T& operator[](size_t pos) {
            return getNodeInternal(pos)->value;
        }

        T& at(size_t pos) {
            Node* node = getNodeInternal(pos);
            if (node == nullptr){
                throw cave::OutOfRangeException(pos);
            }
            return node->value;
        }
        const T& at(size_t pos) const {
            Node* node = getNodeInternal(pos);
            if (node == nullptr){
                throw cave::OutOfRangeException(pos);
            }
            return node->value;
        }

        void pushBack(const T& value){
            addToEndInternal(new Node(value));
        }
        void pushBack(T&& value){
            addToEndInternal(new Node(value));          
        }
        template<typename... Args>
        void emplaceBack(Args&&... args){
            addToEndInternal(buildNodeInternal(std::forward<Args>(args)...));
        }

        void pushFront(const T& value){
            addToStartInternal(new Node(value));
        }
        void pushFront(T&& value){
            addToStartInternal(new Node(value));          
        }
        template<typename... Args>
        void emplaceFront(Args&&... args){
            addToStartInternal(buildNodeInternal(std::forward<Args>(args)...));
        }

        void popBack(){
            if (m_last){
                removeNodeInternal(m_last);
            }
        }
        void popFront(){
            if (m_first){
                removeNodeInternal(m_first);
            }
        }

        // For compatibility with the std style naming:
        inline void push_back(const T& value) { pushBack(value); }
        inline void push_back(T&& value) { pushBack(value); }
        template<typename... Args>
        inline void emplace_back(Args&&... args) { emplaceBack(std::forward<Args>(args)...); }
        inline void push_front(const T& value) { pushFront(value); }
        inline void push_front(T&& value) { pushFront(value); }
        template<typename... Args>
        inline void emplace_front(Args&&... args) { emplaceFront(std::forward<Args>(args)...); }
        inline void pop_back() { popBack(); }
        inline void pop_front() { popFront(); }

        size_t size() const {
            return m_size;
        }
        bool empty() const {
            return m_size == 0;
        }

        void clear() {
            Node* next = m_first;

            while(next){
                Node* current = next;
                next = current->next;

                //delete current;
                current->value.~T();
                free(current);
            }
            m_first = nullptr;
            m_last = nullptr;
            m_size = 0;
        }
        
        void erase(size_t pos){
            Node* node = getNodeInternal(pos);
            removeNodeInternal(node);
        }
        void erase(Iterator pos){
            Node* node = pos.current;
            removeNodeInternal(node);
        }
        void erase(size_t first, size_t last){
            Node* next = getNodeInternal(first);
            Node* lNode = getNodeInternal(last);

            while (next && next != lNode){
                Node* current = next;
                next = current->next;
                removeNodeInternal(current);
            }
        }

        void insert(size_t pos, const T& value) {
            addToPosInternal(pos, new Node(value));
        }
        void insert(size_t pos, T&& value) {
            addToPosInternal(pos, new Node(value));
        }
        void insert(Iterator pos, const T& value) {
            addToPosInternal(pos.current, new Node(value));
        }
        void insert(Iterator pos, T&& value) {
            addToPosInternal(pos.current, new Node(value));
        }

    private:
        Node* getNodeInternal(size_t pos) const {
            size_t id = 0;

            Node* current = m_first;
            while (current && id++ < pos){
                current = current->next;
            }
            return current;
        }

        template<typename... Args>
        Node* buildNodeInternal(Args&&... args) {
            Node* node = (Node*)malloc(sizeof(Node));
            memset((void*)node, 0, sizeof(Node));
            new(&node->value) T(std::forward<Args>(args)...);
            return node;
        }
        void removeNodeInternal(Node* node) {
            Node* prev = node->prev;
            Node* next = node->next;

            if (prev){
                prev->next = next;
            }
            if (next){
                next->prev = prev;
            }
            if (m_first == node){
                m_first = next;
            }
            if (m_last == node){
                m_last = prev;
            }
            delete node;
            m_size--;
        }
        void addToStartInternal(Node* node) {
            if (m_first){
                m_first->prev = node;
                node->next = m_first;
            }
            else if (m_last == nullptr){
                m_last = node;
            }
            m_first = node;
            m_size++;
        }
        void addToPosInternal(size_t pos, Node* node) {
            if (pos == 0){
                addToStartInternal(node);
            }
            else if (pos == m_size){
                addToEndInternal(node);
            }
            else {
                Node* old = getNodeInternal(pos);
                if (old == nullptr){
                    throw cave::OutOfRangeException(pos);
                }
                addToPosInternal(old, node);
            }
        }
        void addToPosInternal(Node* old, Node* node){
            node->prev = old->prev;
            old->prev->next = node;

            old->prev = node;
            node->next = old;

            m_size++;
        }
        void addToEndInternal(Node* node) {
            if (m_last){
                m_last->next = node;
                node->prev = m_last;
            }
            else if (m_first == nullptr){
                m_first = node;
            }
            m_last = node;
            m_size++;
        }

        struct Node {
            Node(const T& v) : value(v), prev(nullptr), next(nullptr) {}
            Node(T&& v) : value(std::move(v)), prev(nullptr), next(nullptr) {}

            T value;

            Node* prev;
            Node* next;
        };
        
        Node* m_first;
        Node* m_last;
        size_t m_size;
    };
}

#endif  // !CAVE_STD_LIST_H