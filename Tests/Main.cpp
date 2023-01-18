#include <iostream>

//#include "STD/String.h"
//#include "STD/Vector.h"
//#include "STD/List.h"
//#include "STD/HashMap.h"

#include "StringTests.h"
#include "VectorTests.h"
#include "ListTests.h"
#include "HashMapTests.h"
#include "PairTests.h"

int main(){
    // Running the String tests:
    testCaveString();    

    std::cout << "\n";
    // Running the Vector tests:
    testCaveVector();
    testCaveVectorBehavior();

    std::cout << "\n";
    // Running the Linked List tests:
    testCaveList();
    testCaveListIterator();

    std::cout << "\n";
    // Running the Pair tests:
    testCavePair();

    std::cout << "\n";
    // Running the Hash Map (unordered_map) tests:
    testCaveHashMap();
    testCaveHashMapBehavior();


    return 0;
}