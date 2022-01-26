// File for XSAVE instruction testing

#include <immintrin.h>
#include <intrin.h> // for reading __cpuid()
#include <iostream> // for cout
#include <string> // for to_string() method to go from int to string
#include <stdlib.h>
#include <stdio.h>
#include <memory> // for void *align
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

// this is 512 bytes (128 * 4), aligned on 64 byte boundary
int total_bytes = 512;
int byte_boundary_size = 64;

// print out the entire xsave data state region
void print_xsave(int* xsavedata_int) {
    for (int i = 0; i < (total_bytes / 4); i++) {
        int last_byte = (i + 1) * 4 - 1;
        int first_byte = (i + 1) * 4 - 4;
        cout << "byte " << to_string(first_byte) << "-" << to_string(last_byte) << ": " << to_string(xsavedata_int[i]) << endl;
    }
}

void change_xsave_data_random(int* xsavedata_int) {
    for (int i = 0; i < (total_bytes / 4); i++) {
        xsavedata_int[i] = rand();
    }
}

int main(int argc, char *argv[]) {

    /*
    Allocate 128 * 4 = 512 bytes aligned on a 64 byte boundary for XSAVE. This differs from FXSAVE which requires 16 byte boundary
    https://www.felixcloutier.com/x86/xsave
    */

    // allocate the xsave data region given by 'total_bytes' on a 'byte_boundary_size' boundary
    void* xsavedata = _aligned_malloc(total_bytes, byte_boundary_size);
    if (xsavedata == nullptr) {
        throw exception("Aligned malloc failed, returned nullptr");
    }

    // clear the data inside the region to all 0s
    memset(xsavedata, 0, total_bytes);

    // check if XSAVE is supported either by CPU or OS
    // uses InstructionSet.cpp
    check_instructions();

    while (true) {

        // call xsave and save the processor state (note this is 32 bit mode)
        // saves everything in processor state that can be saved
        _xsave(xsavedata, 0xFu);

        // change the data
        change_xsave_data_random((int*)xsavedata);

        // realign the memory region
        std::size_t space = 512;
        xsavedata = align(64, 512, xsavedata, space);

        _xrstor(xsavedata, 0xFu);
    }

    // print out the results
    print_xsave((int*)xsavedata);
    
    // free allocated data region using special _aligned_free() method
    // TODO: fix _aligned_free(xsavedata)
    _aligned_free(xsavedata);

    return 0;
}
