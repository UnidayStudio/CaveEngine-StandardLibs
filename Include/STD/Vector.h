#ifndef CAVE_STD_VECTOR_H
#define CAVE_STD_VECTOR_H

#include <cstddef> // size_t
#include <utility> // std::move, std::forward
#include <cstdlib> // malloc, free

#include "STD/Exception.h"


namespace cave {
    template <typename T>
    class Vector{
    public:
        static constexpr size_t npos = -1;
        static constexpr size_t minAllocatedSlots = 64;

        Vector() : m_data(nullptr), m_size(0), m_allocated(0) {}
        Vector(const Vector& other) : m_data(nullptr), m_size(0), m_allocated(0) {
            fitNewSize(other.m_size);
            
            m_size = other.m_size;
            for (size_t i=0; i<m_size; i++){
                new(&m_data[i]) T(other.m_data[i]);
            }
        }
        Vector(Vector&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_allocated(other.m_allocated){
            other.m_data = nullptr;
            other.m_size = 0;
            other.m_allocated = 0;
        }
        virtual ~Vector(){
            // Destroying the remaining objects
            clear();

            if (m_data){
                free(m_data);
            }
        }

        using iterator = T*;
        using const_iterator = const T*;

        iterator       begin() {
            return &at(0);
        }
        const_iterator begin() const {
            return &at(0);
        }

        iterator       end() {
            return &at(m_size);
        }
        const_iterator end()   const {
            return &at(m_size);
        }

        T&       front() {
            return m_data[0];
        }
        const T& front() const {
            return m_data[0];
        }

        T&       back() {
            return m_data[m_size - 1];
        }
        const T& back() const {
            return m_data[m_size - 1];
        }

        T  operator[](size_t pos) const {
            return m_data[pos];
        }
        T& operator[](size_t pos) {
            return m_data[pos];
        }

        T& at(size_t pos) {
            if (pos > m_size || m_data == nullptr){
                throw cave::OutOfRangeException(pos);
            }
            return m_data[pos];
        }
        const T& at(size_t pos) const {
            if (pos > m_size || m_data == nullptr){
                throw cave::OutOfRangeException(pos);
            }
            return m_data[pos];
        }

        void pushBack(const T& value){
            fitNewSize(m_size + 1);
            new(&m_data[m_size]) T(value);
            m_size++;
        }
        void pushBack(T&& value){
            fitNewSize(m_size + 1);
            m_data[m_size] = std::move(value);
            m_size++;
        }
        template<typename... Args>
        void emplaceBack(Args&&... args) {
            fitNewSize(m_size + 1);
            new(&m_data[m_size]) T(std::forward<Args>(args)...);
            m_size++;
        }

        void popBack() {
            if (m_size > 0){
                m_size--;
                m_data[m_size].~T();
            }
        }

        void shringToFit(){
            fitNewSize(m_size, true);
        }

        // For compatibility with the std style naming:
        inline void push_back(const T& value) { pushBack(value); }
        inline void push_back(T&& value) { pushBack(value); }
        template<typename... Args>
        inline void emplace_back(Args&&... args) { emplaceBack(std::forward<Args>(args)...); }
        inline void pop_back() { popBack(); }
        inline void shrink_to_fit() { shringToFit(); }

        void erase(size_t pos){
            m_data[pos].~T();

            for (size_t i = pos + 1; i< m_size; i++){
                new(&m_data[i - 1]) T(std::move(m_data[i]));
                //m_data[i - 1] = std::move(m_data[i]);
            }
            m_size--;
        }
        void erase(size_t first, size_t last){
            if (last < first){ return; }
            if (last > m_size){
                last = m_size;
            }
            for (size_t i = first; i < last; i++){
                m_data[i]->~T();
                m_size--;
            }
            for (size_t i = last; i< m_size; i++){
                new(&m_data[i - 1]) T(std::move(m_data[i]));
                //m_data[i - 1] = std::move(m_data[i]);
            }
        }

        void insert(size_t pos, const T& value){
            fitNewSize(m_size + 1);

            for (size_t i = m_size-1; i >= pos; i--){
                new(&m_data[i + 1]) T(std::move(m_data[i]));
                //m_data[i + 1] = std::move(m_data[i]);
            }
            new(&m_data[pos]) T(value);
            m_size++;
        }
        void insert(size_t pos, T&& value){
            fitNewSize(m_size + 1);

            for (size_t i = m_size-1; i >= pos; i--){
                new(&m_data[i + 1]) T(std::move(m_data[i]));
                //m_data[i + 1] = std::move(m_data[i]);
            }
            new(&m_data[pos]) T(std::move(value));
            //m_data[pos] = std::move(value);            
            m_size++;
        }

        size_t find(const T& object) const {
            for (size_t i=0; i<m_size; i++){
                if (m_data[i] == object){
                    return i;
                }
            }
            return npos;
        }

        size_t size()  const{
            return m_size;
        }
        bool empty() const{
            return m_size == 0;
        }

        T* data() noexcept {
            return m_data;
        }
        const T* data() const noexcept {
            return m_data;
        }

        size_t capacity() const{
            return m_allocated;
        }
        void reserve(size_t n) {
            fitNewSize(n);
        }
        void resize(size_t n) {
            if (n == m_size) { return; }
            fitNewSize(n);

            if (n > m_size){
                // Initializing the extra objects
                for (size_t i=m_size; i< n; i++){
                    new(&m_data[i]) T();
                }
            }
            else {
                // Destroying the exceeded objects
                for (size_t i=m_size; i< n; i++){
                    m_data[i].~T();
                }
            }
            m_size = n;
        }
        void resize(size_t n, const T& val) {
            if (n == m_size) { return; }
            fitNewSize(n);

            if (n > m_size){
                // Initializing the extra objects
                for (size_t i=m_size; i< n; i++){
                    new(&m_data[i]) T(val);
                }
            }
            else {
                // Destroying the exceeded objects
                for (size_t i=m_size; i< n; i++){
                    m_data[i].~T();
                }
            }
            m_size = n;
        }

        void clear(){
            // Destroying all the objects
            for (size_t i=0; i < m_size; i++){
                m_data[i].~T();
            }
            m_size = 0;
        }
    private:
        void fitNewSize(size_t newSize, bool shrink=false){
            if (newSize > m_allocated || shrink){
                // The minimum allocation size will be 64 slots!
                size_t v = minAllocatedSlots;

                if (newSize >= v){
                    // It's finding the next power of two to use
                    v = newSize - 1;
                    v |= v >> 1; v |= v >> 2; 
                    v |= v >> 4; v |= v >> 8; 
                    v |= v >> 16; v++;
                }
                m_allocated = v;

                // Not constructing this!
                // PS: I'll allocate an extra slot just for safety...
                T* newData = (T*)malloc((v + 1) * sizeof(T));

                if (m_data){
                    for (size_t i=0; i<m_size; i++){
                        new(&newData[i]) T(std::move(m_data[i]));
                        //newData[i] = std::move(m_data[i]);
                    }
                    // Also not destructing this!
                    free(m_data);
                }
                m_data = newData;
            }
        }

        T* m_data;
        size_t m_size;
        size_t m_allocated;
    };
}

#endif  // !CAVE_STD_VECTOR_H