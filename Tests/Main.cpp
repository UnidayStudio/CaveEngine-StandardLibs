#include <iostream>

#include "STD/String.h"
#include "STD/Vector.h"

#include "StringTests.h"
#include "VectorTests.h"
#include "ListTests.h"

int main(){
    // Running the String tests:
    testCaveString();    

    // Running the Vector tests:
    testCaveVector();
    testCaveVectorBehavior();

    // Running the Linked List tests:
    testCaveList();
    testCaveListIterator();

    return 0;
}