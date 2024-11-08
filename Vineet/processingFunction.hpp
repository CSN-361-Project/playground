#ifndef PROCESSINGFUNCTION_HPP
#define PROCESSINGFUNCTION_HPP

#include "includes.hpp"

// Declarations ----------------------------
varInt variablelenghtDecode(char *data, const int start);






// Implementations ----------------------------
varInt variablelenghtDecode(char *data, const int start){
    varInt number = 0;
    int i = start;
    int MSB = data[i] & 0xC0; // two most significant bits
    int sizeOfNumber = 0;// in bytes
    if (MSB == 0x00)
    {
        sizeOfNumber = 1;
    }
    else if(MSB == 0x40){
        sizeOfNumber = 2;
    }
    else if(MSB == 0x80){
        sizeOfNumber = 4;
    }
    else if(MSB == 0xC0){
        sizeOfNumber = 8;
    }
    number = data[i++] & 0x3F;

    for (int j = 1; j < sizeOfNumber; j++)
    {
        number = number << 8;
        number = number | data[i++];
    }
    return number;
}

Byte8 readByte8(char *data, const int start, int noOfBytes){
    Byte8 number = 0;
    int i = start;
    for (int j = 0; j < noOfBytes; j++)
    {
        number = number << 8;
        number = number | data[i++];
    }
    return number;
}


#endif // PROCESSINGFUNCTION_HPP