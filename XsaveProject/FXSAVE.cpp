// File for FXSAVE instruction testing

#include <immintrin.h>
#include <intrin.h> // for reading __cpuid()
#include <iostream> // for cout
#include <stdlib.h>
#include <stdio.h>
#include <memory>   
#include <exception>
#include "FP_loop_test.h"
#include "InstructionSet.h" // for seeing which Instructions are supported/enabled

// CREDIT: 
// Pointer based memory alignment code comes from online C++ documentation:
/*
https://en.cppreference.com/w/cpp/memory/align
https://www.cplusplus.com/doc/tutorial/exceptions/
*/

using namespace std;

int main(int argc, char* argv[]) {

    cout << "Hello, World!" << endl;
    void* p5 = _aligned_malloc(128 * sizeof(int), 16);
    cout << "size of pointer is " << sizeof(p5) << endl;

    if (p5 == nullptr) {
        cout << "null reference on p5" << endl;
        return -1;
    }
    std::cout << "allocated an int at " << (void*)p5 << " (16 byte alignment)\n";

    // check if XSAVE is supported either by CPU or OS
    // uses InstructionSet.cpp
    check_instructions();

    int* beforepoint = (int*)p5;
    cout << " BEFORE POINT:" << endl;
    for (int i = 0; i < 72; i++) {
        cout << beforepoint[i] << endl;
    }

    _fxsave(p5);
    // _xsave(p5, 0x11111);
    int* intpoint = (int*)p5;

    // Bytes 464:511 are available to software use.The processor does not write to bytes 464 : 511 of an FXSAVE area.
    cout << "AFTER POINT" << endl;
    for (int i = 0; i < 72; i++) {
        cout << intpoint[i] << endl;
    }

    /*
    for (int i = 0; i < 72; i++) {
        intpoint[i] = 3;
    }
    */

    // stall some time before an FXRSTR

    // WE CURRENTLY ARE NOT IN 64 BIT MODE!!!
    // alignment is good

    //_fxrstor(p5);

    run_fpu();
    _fxsave(p5);

    cout << endl;
    cout << endl;
    cout << "AFTER POINT 2" << endl;
    for (int i = 0; i < 72; i++) {
        cout << intpoint[i] << endl;
    }


    _aligned_free(p5);
    return 0;
}
