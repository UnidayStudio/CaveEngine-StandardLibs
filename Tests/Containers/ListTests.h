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

    // To Test pop front and back
    cave::List<int> listPop;
    listPop.pushBack(1);
    listPop.pushBack(2);
    listPop.pushBack(3);

    // Test popFront
    assert(listPop.front() == 1);
    listPop.popFront();
    assert(listPop.front() == 2);
    assert(listPop.size() == 2);
    listPop.popFront();
    assert(listPop.front() == 3);
    assert(listPop.size() == 1);

    // Test popBack
    assert(listPop.back() == 3);
    listPop.popBack();
    assert(listPop.empty() == true);
    assert(listPop.size() == 0);
    
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

// Performance checks...

#include <cstdio>
#include <chrono>
#include <list>

void testListPerformance() {
    const int N = 100000;

    std::cout << " - (We'll be testing it with " << N << " elements.)\n";

    printf("          |  std::list | cave::List |\n");
    size_t dur1 = 0;
    size_t dur2 = 0;

    std::list<int> l1;
    cave::List<int> l2;

    // Test adding performance
    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        l1.push_back(i);
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        l2.pushBack(i);
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("   Adding | %7zu us | %7zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");


    // Test iteration performance
    start = std::chrono::high_resolution_clock::now();
    int count1 = 0;
    for (auto& i : l1) {
        count1 += i;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    int count2 = 0;
    for (auto& i : l2) {
        count2 += i;
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();
    printf("Iterating | %7zu us | %7zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");

    assert(count1 == count2); // Little assert just to make sure...


    // Test removing performance
    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        l1.pop_back();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur1 = duration.count();

    start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < N; i++) {
        l2.popBack();
    }
    end = std::chrono::high_resolution_clock::now();
    duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
    dur2 = duration.count();

    printf(" Removing | %7zu us | %7zu us |", dur1, dur2);
    if (dur1 < dur2){ printf(" BAD!"); }
    printf("\n");
}
