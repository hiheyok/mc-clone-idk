#include <iostream>
#include <ctime>
#include <cstdlib>

#include "NumGen.h"

long long unsigned int count = 0;

long long int getRandomNumber() {

    count++;

    srand((unsigned)time(NULL) + count);

    return (float)rand() / RAND_MAX;
}

long long unsigned int ID_VAL = 0;

long long unsigned int getID() {
    ID_VAL++;
    return ID_VAL;
}