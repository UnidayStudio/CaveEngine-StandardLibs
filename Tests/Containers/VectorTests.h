#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "Containers/Vector.h"
#include "Containers/Exception.h"


void testCaveVector() {
    std::cout << "[VECTOR] Running tests...\n";

    cave::Vector<int> vec;
    assert(vec.empty());
    assert(vec.size() == 0);
    assert(vec.capacity() == 0);

    // Test iterating an empty vector
    try {
        int count = 0;
        for (int e : vec){
            count += e;
        }
    } catch (cave::OutOfRangeException&){
        assert(false);
    }

    // test push_back
    for (int i=0; i<3; i++){
        vec.pushBack(i + 1);
    }
    assert(vec.size() == 3);
    for (int i=0; i<3; i++){
        assert(vec[i] == i + 1);
    } 
    // Test iterating a filled vector:
    try {
        int count = 1;
        for (int e : vec){
            assert(e == count++);
        }
    } catch (cave::OutOfRangeException&){
        assert(false);
    }

    // Test find
    assert(vec.find(2) == 1);

    // Test front and back
    assert(vec.front() == 1);
    assert(vec.back() == 3);

    // Test at
    assert(vec.at(0) == 1);
    assert(vec.at(1) == 2);
    assert(vec.at(2) == 3);

    // Test the iterators
    cave::Vector<int>::Iterator it = vec.begin();
    assert(*it == 1); ++it;
    assert(*it == 2); ++it;
    assert(*it == 3); ++it;
    assert(it == vec.end());

    // test pop_back
    vec.popBack();
    assert(vec.size() == 2);
    assert(vec[0] == 1);
    assert(vec[1] == 2);

    // test insert
    vec.insert(1, 4);

    assert(vec.size() == 3);
    assert(vec[0] == 1);
    assert(vec[1] == 4);
    assert(vec[2] == 2);

    // test erase
    vec.erase(1);
    assert(vec.size() == 2);
    assert(vec[0] == 1);
    assert(vec[1] == 2);

    // test emplace_back
    vec.emplaceBack(5);
    assert(vec.size() == 3);
    assert(vec[0] == 1);
    assert(vec[1] == 2);
    assert(vec[2] == 5);

    // test clear
    vec.clear();
    assert(vec.empty());
    assert(vec.size() == 0);

    // test begin, end, rbegin, rend
    vec.pushBack(1);
    vec.pushBack(2);
    vec.pushBack(3);
    assert(*vec.begin() == 1);
    assert(*(vec.end()-1) == 3);
//  assert(*vec.rbegin() == 3);
//  assert(*(vec.rend()-1) == 1);

    // test reserve
    vec.reserve(10);
    assert(vec.capacity() == vec.minAllocatedSlots);

    vec.reserve(vec.minAllocatedSlots + 10);
    assert(vec.capacity() > vec.minAllocatedSlots);

    // test resize
    vec.resize(5, 1337);
    assert(vec.size() == 5);
    assert(vec[4] == 1337);

    vec.resize(8);
    assert(vec.size() == 8);

    vec.resize(2);
    assert(vec.size() == 2);

    std::cout << "[VECTOR] All tests passed!" << std::endl;
}


/*===========================================================================//
//===========================================================================*/

// Just to get rid of the unused warning, since 
// those testMocks really don't matter!
#define MARKUSED(X)  ((void)(&(X)))

struct VectorTestMock {
    VectorTestMock() { ++defaultCtorCunt; }
    VectorTestMock(const VectorTestMock& other) { MARKUSED(other); ++copyCtorCount; }
    VectorTestMock(VectorTestMock&& other) { MARKUSED(other); ++moveCtorCount; }
    ~VectorTestMock() { ++dtorCount; }

    static int defaultCtorCunt;
    static int copyCtorCount;
    static int moveCtorCount;
    static int dtorCount;
};

int VectorTestMock::defaultCtorCunt = 0;
int VectorTestMock::copyCtorCount = 0;
int VectorTestMock::moveCtorCount = 0;
int VectorTestMock::dtorCount = 0;

struct VectorTestPtrMock {
    VectorTestPtrMock() { 
        if (initialized){ throw int(0xDEADBEEF); }
        initialized = true;
    }
    VectorTestPtrMock(const VectorTestPtrMock& other) { 
        MARKUSED(other);
        if (initialized){ throw int(0xDEADC0DE); }
        initialized = true;
    }
    VectorTestPtrMock(VectorTestPtrMock&& other) { 
        MARKUSED(other);
        if (initialized){ throw int(0xBADDCAFE); }
        initialized = true;
    }
    ~VectorTestPtrMock() {
        initialized = false;
    }

    static bool initialized;
};

bool VectorTestPtrMock::initialized = false;

void printTestMockStatus(){
    return; // Disabled...

    std::cout << "-------------------\n";
    std::cout << "defaultCtorCunt: " << VectorTestMock::defaultCtorCunt << "\n";
    std::cout << "copyCtorCount:   " << VectorTestMock::copyCtorCount   << "\n";
    std::cout << "moveCtorCount:   " << VectorTestMock::moveCtorCount   << "\n";
    std::cout << "dtorCount:       " << VectorTestMock::dtorCount       << "\n";
    std::cout << "-------------------\n";
}

void testCaveVectorBehavior() {
    std::cout << "[VECTOR | BEHAVIOR] Running tests...\n";

    VectorTestMock::defaultCtorCunt = 0;
    VectorTestMock::copyCtorCount = 0;
    VectorTestMock::moveCtorCount = 0;
    VectorTestMock::dtorCount = 0;

    {
        cave::Vector<VectorTestMock> vec;

        // Test default constructor
        vec.resize(3);
        printTestMockStatus();

        assert(VectorTestMock::defaultCtorCunt == 3);
        assert(VectorTestMock::copyCtorCount == 0);
        assert(VectorTestMock::moveCtorCount == 0);
        assert(VectorTestMock::dtorCount == 0);

        // Test copy constructor
        cave::Vector<VectorTestMock> vec2 = vec;
        printTestMockStatus();

        assert(VectorTestMock::defaultCtorCunt == 3);
        assert(VectorTestMock::copyCtorCount == 3);
        assert(VectorTestMock::moveCtorCount == 0);
        assert(VectorTestMock::dtorCount == 0);

        // Test move constructor
        cave::Vector<VectorTestMock> vec3 = std::move(vec);
        printTestMockStatus();
        
        assert(VectorTestMock::defaultCtorCunt == 3);
        assert(VectorTestMock::copyCtorCount == 3);
        assert(VectorTestMock::moveCtorCount == 0);
        assert(VectorTestMock::dtorCount == 0);

        // Test destructor
    }
    printTestMockStatus();

    assert(VectorTestMock::defaultCtorCunt == 3);
    assert(VectorTestMock::copyCtorCount == 3);
    //assert(VectorTestMock::moveCtorCount == 3);
    assert(VectorTestMock::dtorCount == 6);

    // Test how it behaves with pointers...
    VectorTestPtrMock* ptr = new VectorTestPtrMock();
    try {
        cave::Vector<VectorTestPtrMock*> vec;

        vec.push_back(ptr);

        // Forcing it to move things around...
        vec.push_back(nullptr);
        vec.reserve(10);
        vec.reserve(500);
        vec.resize(32, nullptr);
    } catch (int e){
        assert(false);
    }
    assert(VectorTestPtrMock::initialized);
    delete ptr;

    std::cout << "[VECTOR | BEHAVIOR] All tests passed!" << std::endl;
}