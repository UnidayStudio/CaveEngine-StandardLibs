#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "Containers/String.h"
#include "Containers/StringHash.h"
#include "Containers/Exception.h"


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
    } catch (cave::OutOfRangeException& e) {
        assert(true);
    }

    // Test front() and back() methods
    assert(s6.front() == 'h');
    assert(s6.back() == 'o');

    // Test c_str() method
    assert(strcmp(s6.c_str(), "hello hello worldhello hello") == 0);

    // Test c_str() method when the string is empty!
    cave::String emptyStr;
    assert(strcmp(emptyStr.c_str(), "") == 0);

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

    // Testing pushBack method
    cave::String sPush1;
    sPush1.pushBack('H');
    sPush1.pushBack('e');
    sPush1.pushBack('l');
    sPush1.pushBack('l');
    sPush1.pushBack('o');
    assert(sPush1 == "Hello");

    // Testing popBack method
    cave::String sPush2("Hello");
    sPush2.popBack();
    sPush2.popBack();
    std::cout << sPush2 << "\n";
    assert(sPush2 == "Hel");
    sPush2.popBack();
    sPush2.popBack();
    sPush2.popBack();
    assert(sPush2 == "");
    sPush2.popBack();
    assert(sPush2 == "");

    // Test string hashing...
    cave::String hashTest = "hmmm";
    auto hs = std::hash<cave::String>{}(hashTest);
    assert(hs == 5405861693744609278);

    std::cout << "[STRING] All tests passed!" << std::endl;
}



// Performance checks...

#include <cstdio>
#include <chrono>
#include <string>

void testStringPerformance() {
    const int N = 50000;

    std::cout << " - (We'll be testing it with " << N << " elements.)\n";

    printf("          |  std::string | cave::String |\n");
    size_t dur1 = 0;
    size_t dur2 = 0;

    std::string s1;
    cave::String s2;

    // Test appending performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        s1 += 'a';
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        s2 += 'a';
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("   Adding | %9zu us | %9zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");


    // Test iteration performance
    start = std::chrono::high_resolution_clock::now();
    int count1 = 0;
    for (auto& i : s1) {
        count1 += int(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    int count2 = 0;
    for (auto& i : s2) {
        count2 += int(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("Iterating | %9zu us | %9zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");

    assert(count1 == count2); // Little assert just to make sure...

    {
        // Test reserve performance
        start = std::chrono::high_resolution_clock::now();
        std::string testS1;
        for (int i=1; i<100; i++){
            testS1.reserve(i*100);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        dur1 = duration.count();

        start = std::chrono::high_resolution_clock::now();
        cave::String testS2;
        for (int i=1; i<100; i++){
            testS2.reserve(i*100);
        }
        end = std::chrono::high_resolution_clock::now();
        duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        dur2 = duration.count();
        printf("  Reserve | %9zu us | %9zu us |", dur1, dur2);
        if (dur1 < dur2){ printf(" BAD!"); }
        printf("\n");
    }


    // Test removing performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        s1.pop_back();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        s2.popBack();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();

    printf(" Removing | %9zu us | %9zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");
}
