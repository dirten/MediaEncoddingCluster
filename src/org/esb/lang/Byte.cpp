#include "Byte.h"

using namespace org::esb::lang;

Byte::Byte(char byte){
    _byte=static_cast<char>(byte);
}

Byte::~Byte(){

}

char Byte::byteValue(){
    return _byte;
}

short Byte::shortValue(){
    return _byte;
}

int Byte::intValue(){
    return _byte;
}

long Byte::longValue(){
    return _byte;
}

double Byte::doubleValue(){
    return _byte;
}

float Byte::floatValue(){
    return _byte;
}

