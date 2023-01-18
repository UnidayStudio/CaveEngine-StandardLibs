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
