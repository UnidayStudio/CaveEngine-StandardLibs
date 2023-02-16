#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "Containers/String.h"
#include "Containers/StringHash.h"

#include "Containers/HashMap.h"
#include "Containers/Exception.h"


void testCaveHashMap() {
    std::cout << "[HASH MAP] Running tests...\n";

    cave::HashMap<cave::String, int> map;

    //Testing insert
    //map.insert({"first", 1});
    map.insert("first", 1);
    assert(map.size() == 1);
    assert(map.count("first") == 1);
    assert(map["first"] == 1);
    assert(map.at("first") == 1);

    //Testing erase
    map.erase("first");
    assert(map.empty());
    assert(map.size() == 0);

    //Testing operator[] and at()
    map["second"] = 2;
    assert(map.size() == 1);
    assert(map.count("second") == 1);
    assert(map["second"] == 2);
    assert(map.at("second") == 2);
    try {
        map.at("not_exist");
        assert(false); // should throw an OutOfRangeException
    } catch (cave::OutOfRangeException&) {
        assert(true);
    }

    //Testing clear
    map.clear();
    assert(map.empty());
    assert(map.size() == 0);
    
    //Testing Iterator
    map["it first"] = 1;
    {
        // Iterator when the map only have a single element...
        int i = 0;
        for (auto& it : map){
            i += it.second;
        }
        assert(i == 1);
    }
    map["it second"] = 2;
    map["it third"] = 3;
    {
        int i = 0;
        for (auto& it : map){
            i += it.second;
        }
        assert(i == 6);
    }
    {
        int i = 0;
        for(auto it = map.begin(); it != map.end(); it++ ){
            i += it->second;
        }
        assert(i == 6);
    }

    // Test find
    assert(map.find("it second") != map.end());
    assert(map.find("invalid key") == map.end());
    
    std::cout << "[HASH MAP] All tests passed!" << std::endl;
}

/*===========================================================================//
//===========================================================================*/


struct HashMapTestMock {
    HashMapTestMock() {
        ++ctorCount;
    }
    HashMapTestMock(const HashMapTestMock&) {
        ++copyCtorCount;
    }
    HashMapTestMock(HashMapTestMock&&) {
        ++moveCtorCount;
    }
    HashMapTestMock& operator=(const HashMapTestMock&) {
        ++copyAssignmentCount;
        return *this;
    }
    HashMapTestMock& operator=(HashMapTestMock&&) {
        ++moveAssignmentCount;
        return *this;
    }
    ~HashMapTestMock() {
        ++dtorCount;
    }

    static int ctorCount;
    static int copyCtorCount;
    static int moveCtorCount;
    static int copyAssignmentCount;
    static int moveAssignmentCount;
    static int dtorCount;
};

int HashMapTestMock::ctorCount = 0;
int HashMapTestMock::copyCtorCount = 0;
int HashMapTestMock::moveCtorCount = 0;
int HashMapTestMock::copyAssignmentCount = 0;
int HashMapTestMock::moveAssignmentCount = 0;
int HashMapTestMock::dtorCount = 0;

#define HASH_MAP_MOCK_ASSERT(cTor, cCtor, mCtor, cAssign, mAssign, dTor) \
    assert(HashMapTestMock::ctorCount            == cTor   ); \
    assert(HashMapTestMock::copyCtorCount        == cCtor  ); \
    assert(HashMapTestMock::moveCtorCount        == mCtor  ); \
    assert(HashMapTestMock::copyAssignmentCount  == cAssign); \
    assert(HashMapTestMock::moveAssignmentCount  == mAssign); \
    assert(HashMapTestMock::dtorCount            == dTor   );

void testCaveHashMapBehavior() {
    std::cout << "[HASH MAP | BEHAVIOR] Running tests...\n";

    // Inserting a value
    HashMapTestMock::ctorCount = 0;
    HashMapTestMock::copyCtorCount = 0;
    HashMapTestMock::moveCtorCount = 0;
    HashMapTestMock::copyAssignmentCount = 0;
    HashMapTestMock::moveAssignmentCount = 0;
    HashMapTestMock::dtorCount = 0;

    {
        cave::HashMap<cave::String, HashMapTestMock> map;

        // Test inserting to the map
        HashMapTestMock value;
        map.insert("first", value);
        HASH_MAP_MOCK_ASSERT(1, 1, 0, 0, 0, 0);

        // Test Copying the map
        cave::HashMap<cave::String, HashMapTestMock> map2(map);
        HASH_MAP_MOCK_ASSERT(1, 2, 0, 0, 0, 0);

        // Test Moving the map
        cave::HashMap<cave::String, HashMapTestMock> map3(std::move(map));
        // NOTE: In this case, It will not invoke the mock's move ctor, since
        // it will "steal" the old map's data and keep the elements in the
        // same memory address (remember that the elements are pointers).
        HASH_MAP_MOCK_ASSERT(1, 2, 0, 0, 0, 0);

        // Clearing the map
        map3.clear();
        HASH_MAP_MOCK_ASSERT(1, 2, 0, 0, 0, 1);
    }

    std::cout << "[HASH MAP | BEHAVIOR] All tests passed!" << std::endl;
}


// Performance checks...

#include <cstdio>
#include <chrono>
#include <unordered_map>

void testHashMapPerformance() {
    const int N = 100000;

    std::cout << " - (We'll be testing it with " << N << " elements.)\n";

    printf("          | std::unordered_map |  cave::HashMap |\n");
    size_t dur1 = 0;
    size_t dur2 = 0;

    std::unordered_map<int, int> map1;
    cave::HashMap<int, int> map2;
    
    // Test adding performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        map1[i] = i;
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        map2[i] = i;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("   Adding | %15zu us | %11zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");


    // Test random access performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        map1.at(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        map2.at(1);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("R. Access | %15zu us | %11zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");


    // Test iteration performance
    start = std::chrono::high_resolution_clock::now();
    int count1 = 0;
    for (auto& it : map1) {
        count1 += it.second;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    int count2 = 0;
    for (auto& it : map2) {
        count2 += it.second;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("Iterating | %15zu us | %11zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");

    assert(count1 == count2); // Little assert just to make sure...


    // Test removing performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        map1.erase(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        map2.erase(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();

    printf(" Removing | %15zu us | %11zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");
}
