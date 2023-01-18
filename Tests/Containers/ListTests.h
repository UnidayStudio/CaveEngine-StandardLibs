#pragma once

#include <cassert>
#include <cstring>
#include <iostream>

#include "Containers/List.h"
#include "Containers/Exception.h"

void printList(const cave::List<int>& list){
    return;

    std::cout << "List (" << list.size() << " elements): ";
    for (auto i : list){
        std::cout << i << ", ";
    }
    std::cout << "\n";
}

void testCaveList() {
    std::cout << "[LIST] Running tests...\n";

    cave::List<int> list;

    assert(list.size() == 0);
    assert(list.empty());

    //Testing pushBack
    list.pushBack(1);
    assert(list.front() == 1);
    assert(list.back() == 1);
    assert(list.size() == 1);
    assert(!list.empty());

    //Testing emplaceBack
    list.emplace_back(2);
    assert(list.back() == 2);
    assert(list.size() == 2);

    //Testing pushFront
    list.push_front(0);
    assert(list.front() == 0);
    assert(list.size() == 3);

    //Testing emplaceFront
    list.emplace_front(-1);
    assert(list.front() == -1);
    assert(list.size() == 4);

    // To test the copy ctor
    cave::List<int> list2(list);
    assert(list2.size() == list.size());

    //Testing operator[]
    list[1] = 5;
    assert(list[1] == 5);

    // Testing if the copy ctor really copied everything
    assert(list2[1] == 0);

    //Testing at
    list.at(2) = 6;
    assert(list.at(2) == 6);

    //Testing insert
    list.insert(list.begin() + 2, 7);
    assert(list.at(2) == 7);
    assert(list.size() == 5);

    //Testing erase
    list.erase(list.begin() + 1);
    assert(list.at(1) == 7);
    assert(list[1] == 7);
    assert(list.size() == 4);
    
    printList(list);

    //Testing clear
    list.clear();
    assert(list.empty());
    assert(list.size() == 0);
    
    std::cout << "[LIST] All tests passed!" << std::endl;
}

void testCaveListIterator() {
    std::cout << "[LIST | ITERATOR] Running tests...\n";

    cave::List<int> list;
    
    list.pushBack(1);
    list.pushBack(2);
    list.pushBack(3);
    
    //Testing begin() and end()
    assert(list.begin() != list.end());
    assert(list.begin() == list.begin());
    assert(list.end() == list.end());
    
    //Testing operator*
    assert(*list.begin() == 1);
    
    //Testing operator++
    cave::List<int>::Iterator it = list.begin();
    assert(*it++ == 1);
    assert(*it++ == 2);
    assert(*it++ == 3);
    assert(it == list.end());

    //Testing operator--
    assert(*--it == 3);
    assert(*--it == 2);
    assert(*--it == 1);
    assert(it == list.begin());
    
    //Testing operator== and operator!=
    assert(it == list.begin());
    assert(it != list.end());

    std::cout << "[LIST | ITERATOR] All tests passed!" << std::endl;
}

