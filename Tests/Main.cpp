#include <iostream>

#include "STD/String.h"
#include "STD/Vector.h"

#include "StringTests.h"
#include "VectorTests.h"

int main(){
    cave::String testStr;

    testCaveString();    

    testCaveVector();
    testCaveVectorBehavior();

    return 0;
}