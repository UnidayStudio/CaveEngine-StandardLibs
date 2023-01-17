#include "STD/Exception.h"

cave::Exception::Exception(){

}
cave::Exception::~Exception(){
    
}

cave::OutOfRangeException::OutOfRangeException(size_t p) : pos(p) {

}
cave::OutOfRangeException::~OutOfRangeException(){
    
}