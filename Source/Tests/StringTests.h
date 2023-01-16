#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "STD/String.h"


void testCaveString() {
    std::cout << "[STRING] Running tests...\n";

    // Test default constructor
    cave::String s1;
    assert(s1.empty());

    // Test operator== with empty string and char*
    assert(s1 == "");

    // Test constructor with char*
    cave::String s2("hello");
    assert(s2 == "hello");

    // Test copy constructor
    cave::String s3(s2);
    assert(s3 == "hello");

    // Test move constructor
    cave::String s4(std::move(s3));
    assert(s3.empty());
    assert(s4 == "hello");

    // Test operator!= with char*
    assert(s1 != "no");
    assert(s2 != "no");

    // Test operator!= with String
    assert(s1 != s2);

    // Test operator= with char*
    s1 = "world";
    assert(s1 == "world");

    // Test operator= with cave::String
    s1 = s4;
    assert(s1 == "hello");

    // Test operator+= with char*
    s1 += " ";
    assert(s1 == "hello ");

    // Test operator+= with cave::String
    s1 += s2;
    assert(s1 == "hello hello");

    // Test operator+ with char*
    cave::String s5 = s1 + " world";
    assert(s5 == "hello hello world");

        // Test operator+ with cave::String
    cave::String s6 = s5 + s1;
    assert(s6 == "hello hello worldhello hello");

    // Test operator[]
    assert(s6[5] == ' ');

    // Test at() method
    assert(s6.at(6) == 'h');
    try {
        s6.at(100);
        assert(false);  // out_of_range exception should be thrown
    } catch (cave::String::OutOfRangeException& e) {
        assert(true);
    }

    // Test front() and back() methods
    assert(s6.front() == 'h');
    assert(s6.back() == 'o');

    // Test c_str() method
    assert(strcmp(s6.c_str(), "hello hello worldhello hello") == 0);

    // Test data() method
    assert(s6.data()[5] == ' ');

    // Test begin() and end() methods
    assert(*s6.begin() == 'h');
    assert(*(s6.end() - 1) == 'o');
    

//    // Test rbegin() and rend() methods
//    assert(*s6.rbegin() == 'o');
//    assert(*(s6.rend() - 1) == 'h');

    // Test size() method
    assert(s6.size() == 28);

    // Test length() method
    assert(s6.length() == 28);

//    // Test resize() method
//    s6.resize(20);
//    assert(s6.size() == 20);
//    s6.resize(40, '-');
//    assert(s6.size() == 40);

    // Test empty() method
    assert(!s6.empty());

    // Test clear() method
    s6.clear();
    assert(s6.empty());

    // Test find() method
    s6 = "hello world";
    assert(s6.find("world") == 6);
    assert(s6.find("moon") == cave::String::npos);

    // Test substring() method
    assert(s6.substr(6, 5) == "world");

    // Test replace() method
    s6.replace(6, 5, "moon");
    assert(s6 == "hello moon");

    // Test insert() method
    s6.insert(6, "beautiful ");
    assert(s6 == "hello beautiful moon");

    // Test erase() method
    s6.erase(6, 10);
    assert(s6 == "hello moon");

    // Test compare() method
    assert(s6.compare("hello moon") == 0);
    assert(s6.compare("goodbye moon") > 0);
    assert(s6.compare("hello sun") < 0);

    // Test rfind() method
    cave::String s7 = "hello world world world worlld worl";
    assert(s7.rfind("world") == 18);

//    // Test swap() method
//    cave::String s8 = "goodbye";
//    s6.swap(s8);
//    assert(s6 == "goodbye");
//    assert(s8 == "hello moon");

    std::cout << "[STRING] All tests passed!" << std::endl;
}

