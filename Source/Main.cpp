#include <iostream>

#include "STD/String.h"
#include "STD/Vector.h"

#include "Tests/StringTests.h"
#include "Tests/VectorTests.h"

int main(){
    cave::String testStr;

    testCaveString();    

    testCaveVector();
    testCaveVectorBehavior();

    return 0;
}