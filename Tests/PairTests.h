#pragma once

#include <cassert>
#include <cstring>
#include <utility>
#include <iostream>

#include "STD/Pair.h"
#include "STD/String.h"
#include "STD/Exception.h"

void testCavePair() {
    std::cout << "[PAIR] Running tests...\n";

    // Testing default constructor
    cave::Pair<int, double> p1;
    assert(p1.first == 0);
    assert(p1.second == 0.0);

    // Testing constructor with initial values
    cave::Pair<cave::String, bool> p2("hello", true);
    assert(p2.first == "hello");
    assert(p2.second == true);

    // Testing move constructor
    cave::String s = "world";
    cave::Pair<cave::String, int> p3(std::move(s), 42);
    assert(p3.first == "world");
    assert(p3.second == 42);

    // Testing copy assignment operator
    cave::Pair<int, double> p4;
    p4 = p1;
    assert(p4.first == 0);
    assert(p4.second == 0.0);

    // Testing move assignment operator
    cave::Pair<cave::String, bool> p5;
    p5 = std::move(p2);
    assert(p5.first == "hello");
    assert(p5.second == true);

    // Testing operator ==
    cave::Pair<int, double> testP1(1, 2.5);
    cave::Pair<int, double> testP2(1, 2.5);
    cave::Pair<int, double> testP3(2, 2.5);
    cave::Pair<int, double> testP4(1, 3.5);
    assert(testP1 == testP2);
    assert(!(testP1 == testP3));
    assert(!(testP1 == testP4));

    // Testing operator !=
    assert(!(testP1 != testP2));
    assert(testP1 != testP3);
    assert(testP1 != testP4);

    std::cout << "[PAIR] All tests passed!" << std::endl;
}
