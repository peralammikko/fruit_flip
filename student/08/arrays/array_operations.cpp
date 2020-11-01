#include "array_operations.hh"
#include <iostream>



using namespace std;

int greatest_v1(int *itemptr, int size)
{
    int greatest = 0;
    while (itemptr < &size) {
        if (*itemptr > greatest) {
            greatest = *itemptr;
        }
        itemptr++;
    }
    return greatest;
}


int greatest_v2(int *itemptr, int *endptr)
{
    int greatest = 0;
    while (itemptr < endptr) {
        if (*itemptr > greatest) {
            greatest = *itemptr;
        }
        itemptr++;
    }
    return greatest;
}

void copy(int *itemptr, int *endptr, int *targetptr)
{
    while (itemptr < endptr) {
        *targetptr = *itemptr;
        itemptr++;
        targetptr++;
    }
}

void reverse(int *leftptr, int *rightptr)
{
    while (leftptr < rightptr) {
        int tempptr = *leftptr;
        rightptr--;
        *leftptr = *rightptr;
        *rightptr = tempptr;
        leftptr++;

    }
}
