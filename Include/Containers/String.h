#ifndef CAVE_STD_STRING_H
#define CAVE_STD_STRING_H

#include <cstddef> // size_t
#include <ostream> // operator<<
#include <string>  // std::to_string


namespace cave {
    class String {
    public:
        static constexpr size_t npos = -1;

        String();
        String(const char* str);
        String(const std::string& other);
        String(const String& other);
        String(String&& other) noexcept;
        virtual ~String();

        using iterator = char*;
        using const_iterator = const char*;

        iterator       begin();
        const_iterator begin() const;

        iterator       end();
        const_iterator end()   const;

        char& front();
        const char& front() const;

        char& back();
        const char& back() const;

        explicit operator const char* () const { return c_str(); }

        friend auto operator<<(std::ostream& os, const String& str) -> std::ostream& {
            if (str.m_data) { os << str.c_str(); }
            return os;
        }

        bool operator==(const char* other)   const;
        bool operator==(const String& other) const;

        bool operator!=(const char* other)   const;
        bool operator!=(const String& other) const;

        bool operator<(const char* other)   const;
        bool operator<(const String& other) const;
        bool operator>(const char* other)   const;
        bool operator>(const String& other) const;

        bool operator<=(const char* other)   const;
        bool operator<=(const String& other) const;
        bool operator>=(const char* other)   const;
        bool operator>=(const String& other) const;

        String& operator=(const char* str);
        String& operator=(const std::string& other);
        String& operator=(const String& other);
        String& operator=(String&& other);

        String& operator+=(const char str);
        String& operator+=(const char* str);
        String& operator+=(const String& other);

        String operator+(const char str);
        String operator+(const char* str);
        String operator+(const String& other);

        friend String operator+(const char* lStr, const String& rStr);
        friend String operator+(const String& lStr, const char* rStr);
        friend String operator+(const String& lStr, const String& rStr);

        char operator[](size_t pos) const;
        char& operator[](size_t pos);

        inline const char* cStr() const { return c_str(); }
        const char* c_str() const;
        const char* data() const;

        char& at(size_t pos);
        const char& at(size_t pos) const;

        size_t size()  const;
        size_t length() const;
        bool empty() const;

        void clear();

        void assign(const char* str);
        void assign(const String& other);

        void append(const char str);
        void append(const char* str);
        void append(const String& other);

        void pushBack(const char str);
        void popBack();

        int compare(const char* str) const;
        int compare(const String& other) const;

        size_t find(const char* str, size_t pos = 0) const;
        size_t find(const String& other, size_t pos = 0) const;

        size_t rfind(const char* str, size_t pos = npos) const;
        size_t rfind(const String& other, size_t pos = npos) const;

        String substr(size_t pos, size_t count) const;

        String& replace(size_t pos, size_t len, const char* str);
        String& replace(size_t pos, size_t len, const String& str);

        String& insert(size_t pos, const char* str);
        String& insert(size_t pos, const String& str);

        String& erase(size_t pos = 0, size_t len = npos);

        void reserve(size_t n);
        size_t capacity() const;

    private:
        char* m_data;
        size_t m_size;
        size_t m_allocated;
    };

    template <typename T>
    cave::String toString(const T& val) {
        return cave::String(std::to_string(val).c_str());
    }
}

#endif // ! CAVE_STD_STRING_H