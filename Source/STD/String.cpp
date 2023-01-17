#include "STD/String.h"

#include <cstring>
#include <stdio.h>
#include <string.h>

#include "STD/Exception.h"


cave::String::String() : m_data(nullptr), m_size(0), m_allocated(0) {}
cave::String::String(const char* str) : m_data(nullptr), m_size(0), m_allocated(0) {
    assign(str);
}
cave::String::String(const cave::String& other) : m_data(nullptr), m_size(0), m_allocated(0) {
    assign(other);
}
cave::String::String(cave::String&& other) noexcept : m_data(other.m_data), m_size(other.m_size), m_allocated(other.m_allocated) {
    other.m_data = nullptr;
    other.m_size = 0;
    other.m_allocated = 0;
}
cave::String::~String(){
    if (m_data){
        delete[] m_data;
    }
}

cave::String::iterator cave::String::begin() { 
    return &at(0); 
}
cave::String::const_iterator cave::String::begin() const { 
    return &at(0); 
}
cave::String::iterator cave::String::end() { 
    return &at(m_size); 
}
cave::String::const_iterator cave::String::end() const { 
    return &at(m_size); 
}

char&       cave::String::front() {
    return at(0);
}
const char& cave::String::front() const {
    return at(0);    
}

char&       cave::String::back() {
    return at(m_size - 1);
}
const char& cave::String::back() const {
    return at(m_size - 1);    
}

bool cave::String::operator==(const char* other) {
    return compare(other) == 0;
}
bool cave::String::operator==(const cave::String& other) {
    return compare(other) == 0;
}

bool cave::String::operator!=(const char* other) {
    return compare(other) != 0;
}
bool cave::String::operator!=(const cave::String& other) {
    return compare(other) != 0;
}

bool cave::String::operator<(const char* other) {
    return compare(other) < 0;
}
bool cave::String::operator<(const cave::String& other) {
    return compare(other) < 0;
}
bool cave::String::operator>(const char* other) {
    return compare(other) > 0;
}
bool cave::String::operator>(const cave::String& other) {
    return compare(other) > 0;
}

bool cave::String::operator<=(const char* other) {
    return compare(other) <= 0;
}
bool cave::String::operator<=(const cave::String& other) {
    return compare(other) <= 0;
}
bool cave::String::operator>=(const char* other) {
    return compare(other) >= 0;
}
bool cave::String::operator>=(const cave::String& other) {
    return compare(other) >= 0;
}

cave::String& cave::String::operator=(const char* str) {
    assign(str);
    return *this;
}
cave::String& cave::String::operator=(const cave::String& other) {
    if (this != &other){
        assign(other);
    }
    return *this;
}
cave::String& cave::String::operator=(cave::String&& other)  {
    if (this != &other){
        if (m_data){
            delete[] m_data;
        }
        m_data = other.m_data;
        m_size = other.m_size;
        m_allocated = other.m_allocated;
        other.m_data = nullptr;
        other.m_size = 0;
        other.m_allocated = 0;
    }
    return *this;
}

cave::String& cave::String::operator+=(const char* str) {
    append(str);
    return *this;
}
cave::String& cave::String::operator+=(const cave::String& other) {
    append(other);
    return *this;
}

cave::String cave::String::operator+(const char* str) {
    String out(*this);
    out.append(str);
    return out;
}
cave::String cave::String::operator+(const cave::String& other) {
    String out(*this);
    out.append(other);
    return out;
}

char cave::String::operator[](size_t pos) const {
    return m_data[pos];
}
char& cave::String::operator[](size_t pos) {
    return m_data[pos];
}

const char* cave::String::c_str() const {
    return m_data;
}
const char* cave::String::data() const {
    return m_data;
}

char& cave::String::at(size_t pos) {
    if (pos > m_size || m_data == nullptr){
        throw cave::OutOfRangeException(pos);
    }
    return m_data[pos];
}
const char& cave::String::at(size_t pos) const {
    if (pos > m_size || m_data == nullptr){
        throw cave::OutOfRangeException(pos);
    }
    return m_data[pos];
}

size_t cave::String::size()  const {
    return m_size;
}
size_t cave::String::length() const {
    return m_size;
}
bool cave::String::empty() const {
    return m_size == 0;
}

void cave::String::clear() {
    assign("");
}

void cave::String::assign(const char* str) {
    m_size = strlen(str);
    const size_t newBuffSize = ((m_size + 1) / bufferSize + 1) * bufferSize;

    if (newBuffSize > m_allocated || newBuffSize < m_allocated - bufferSize){
        m_allocated = newBuffSize;
        
        if (m_data){
            delete[] m_data;
        }
        m_data = new char[m_allocated];
    }
    strcpy(m_data, str);
}
void cave::String::assign(const cave::String& other) {
    assign(other.m_data);
}

void cave::String::append(const char* str) {
    const size_t newSize = m_size + strlen(str);
    const size_t newBuffSize = ((newSize + 1) / bufferSize + 1) * bufferSize;

    if (newBuffSize > m_allocated){
        char* newBuff = new char[newBuffSize];
        if (m_data){
            memcpy(newBuff, m_data, m_allocated);
            delete[] m_data;
        }
        m_data = newBuff;
        m_allocated = newBuffSize;
    }
    strcpy((char*)(m_data + m_size), str);
    m_size = newSize;
}
void cave::String::append(const cave::String& other) {
    append(other.m_data);
}

int cave::String::compare(const char* str) const {
    if (m_data == nullptr){
        return strcmp("", str);
    }
    return strcmp(m_data, str);
}
int cave::String::compare(const cave::String& other) const {
    return compare(other.m_data);
}

size_t cave::String::find(const char* str,          size_t pos) const {
    char* found = strstr((char*)(m_data + pos), str);
    if (found){
        return (size_t)(found - m_data);
    }
    return npos;
}
size_t cave::String::find(const cave::String& other, size_t pos) const {
    return find(other.m_data, pos);
}

size_t cave::String::rfind(const char* str,           size_t pos) const {
    if (pos == npos){
        pos = m_size - 1;
    }
    const size_t strSize = strlen(str);

    for (long long int i=pos; i>=0; i--){
        if (strncmp(m_data + i, str, strSize) == 0){
            return i;
        }
    }
    return npos;
}
size_t cave::String::rfind(const cave::String& other, size_t pos) const {
    return rfind(other.m_data, pos);
}

cave::String cave::String::substr(size_t pos, size_t count) const {
    if (pos > m_size) {
        pos = m_size;
    }
    if (count > m_size - pos) {
        count = m_size - pos;
    }
    String result;
    result.m_size = count;
    result.m_allocated = ((count + 1) / bufferSize + 1) * bufferSize;
    result.m_data = new char[result.m_allocated];
    strncpy(result.m_data, m_data + pos, count);
    result.m_data[count] = '\0';
    return result;
}

cave::String& cave::String::replace(size_t pos, size_t len, const char* str) {
    assign(substr(0, pos) + str + substr(pos + len, npos));
    return *this;
}
cave::String& cave::String::replace(size_t pos, size_t len, const String& str) {
    assign(substr(0, pos) + str + substr(pos + len, npos));
    return *this;
}

cave::String& cave::String::insert(size_t pos, const char* str) {
    assign(substr(0, pos) + str + substr(pos, npos));
    return *this;
}
cave::String& cave::String::insert(size_t pos, const String& str) {
    assign(substr(0, pos) + str + substr(pos, npos));
    return *this;
}

cave::String& cave::String::erase(size_t pos, size_t len) {
    assign(substr(0, pos) + substr(pos + len, npos));    
    return *this;
}
