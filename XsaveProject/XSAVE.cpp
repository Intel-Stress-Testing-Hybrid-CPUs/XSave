// File for XSAVE instruction testing
#include <Windows.h>
#include <immintrin.h>
#include <xmmintrin.h>
#include <intrin.h> // for reading __cpuid()
#include <iostream> // for cout
#include <sstream> // for std::stringstream
#include <string> // for to_string() method to go from int to string
#include <stdlib.h>
#include <stdio.h>
#include <memory> // for void *align
#include <exception>
//#include "FP_loop_test.h"
#include "InstructionSet.h" // for seeing which Instructions are supported/enabled

// CREDIT: 
// Pointer based memory alignment code comes from online C++ documentation:
/*
https://en.cppreference.com/w/cpp/memory/align
https://www.cplusplus.com/doc/tutorial/exceptions/
*/

using namespace std;

/* GLOBALS TO DEFINE XSAVE MEMORY START AND END INDICES FOR SPECIFIC REGIONS*/
// this is 512 bytes (128 * 4), aligned on 64 byte boundary
int total_bytes = 1024;
int byte_boundary_size = 64;

int XMM_start_byte = 160;
int XMM_end_byte = 287;
int XMM_width = 16;

// GLOBALS
int num_tests = 100;
//#define DEBUG 1 // comment out if you don't want full information (excluding printing every xsave region)
//#define DEBUGPRINT 1 // comment out if you don't want full printing of all xsave regions
 
/*
Inputs: xsavedata is the xsave memory region
Outputs: None
Prints out the data of an x-save memory region on a per-byte basis, prints out number of bytes = total_bytes
*/
void print_xsave(char* xsavedata) {
    for (int i = 0; i < (total_bytes); i++) {
        cout << "byte " << to_string(i) << ": " << to_string(xsavedata[i]) << endl;
    }
}

/*
Inputs: xsavedata and xsavedata2 are xsave memory regions
Outputs: None
Prints out the data of two x-save memory regions on a per-byte basis, prints out number of bytes = 288 bytes
*/
void print_two_xsave(char* xsavedata, char* xsavedata2) {
    for (int i = 0; i < 288; i++) { // looking at 288 bytes
        cout << "byte " << to_string(i) << ": " << to_string(xsavedata[i]) << " " << to_string(xsavedata2[i]) << endl;
    }
}

/*
Inputs: xsavedata is the xsave memory region
Outputs: None
Randomly changes data in the xsave data region
*/
void change_xsave_data_random(char* xsavedata) {
    for (int i = 0; i < (total_bytes); i++) {
        xsavedata[i] = rand() % 256;
    }
}

/*
Inputs: two x-saved memory regions and num_bytes is the first number of bytes to check between both memory regions
Outputs: Returns 0 if num_bytes are same in both memory regions, non-zero value otherwise
Checks if two XSAVE memory regions are the same
*/
int checker(char* xsave1, char* xsave2, int num_bytes) {
    return memcmp(xsave1, xsave2, num_bytes);
}

/*
Inputs: two x-saved memory regions and num_bytes is the first number of bytes to check between both memory regions
Outputs: None
Checks if two XSAVE memory regions are the same and if not, prints out which bytes differ. Otherwise prints a message saying both are same
*/
void advanced_checker(char* xsave1, char* xsave2, int num_bytes) {
    bool noDiff = true;
    cout << endl; // space out each advanced check
    for (int i = 0; i < num_bytes; i++) {
        if (xsave1[i] != xsave2[i]) {
            cout << "byte " << i << " is different between the two xsave regions" << endl;
            noDiff = false;
        }
    }
    if (noDiff == true) {
        cout << "both xsave regions are equivalent" << endl;
    }
}

typedef unsigned char uchar;

float bytesToFloat(uchar b0, uchar b1, uchar b2, uchar b3)
{
    float output;
    *((uchar*)(&output) + 3) = b0;
    *((uchar*)(&output) + 2) = b1;
    *((uchar*)(&output) + 1) = b2;
    *((uchar*)(&output) + 0) = b3;

    return output;
}

int values[4] = { 1, 2, 3, 4 };

int main(int argc, char *argv[]) {
    //cout << foo();
    
    /*
    Allocate 128 * 4 = 512 bytes aligned on a 64 byte boundary for XSAVE. This differs from FXSAVE which requires 16 byte boundary
    https://www.felixcloutier.com/x86/xsave
    */

    // allocate the xsave data region given by 'total_bytes' on a 'byte_boundary_size' boundary
    void* xsavedata = _aligned_malloc(total_bytes, byte_boundary_size);
    void* xsavedata2 = _aligned_malloc(total_bytes, byte_boundary_size);
    if (xsavedata == nullptr || xsavedata2 == nullptr) {
        throw exception("Aligned malloc failed, returned nullptr");
    }

    // clear the data inside the region to all 0s
    memset(xsavedata, 0, total_bytes);
    memset(xsavedata2, 0, total_bytes);

    // check if XSAVE is supported either by CPU or OS
    // uses InstructionSet.cpp
    check_instructions();

    // Start XSAVE test
    cout << "Starting XSAVE Test" << endl;

    float float_rand_array[4];
    float float_val = 0.0;

    int counter = 0;
    while (counter < num_tests) {

        // call xsave and save the processor state (note this is 32 bit mode)
        // saves everything in processor state that can be saved
        _xsave(xsavedata, 0xFu);

        // Create a floating point number
        for (int i = 0; i < 4; i++) {
            float_rand_array[i] = (float)rand();
            #ifdef DEBUG
                cout << float_rand_array[i] << endl;
            #endif
        }

        // load stacked 4 32-bit single precision floating points into xmm register
        _mm_loadu_ps(float_rand_array);

        // save processor state again
        _xsave(xsavedata2, 0xFu);

        // print out both regions
        #ifdef DEBUGPRINT
            print_two_xsave((char*)xsavedata, (char*)xsavedata2);
        #endif  

        char* xsavedata2_char = (char*)xsavedata2;

        float float_val2;
        int cur_index = 160;
        int failure_flag = 0;
        for (int i = 0; i < 4; i++) {
            float_val2 = bytesToFloat(xsavedata2_char[cur_index + 3], xsavedata2_char[cur_index + 2], xsavedata2_char[cur_index + 1], xsavedata2_char[cur_index]);
            if (float_rand_array[i] != float_val2) {
                cout << "FAILURE ON THIS TEST" << endl;
                #ifdef DEBUG
                    cout << "val1: " << float_rand_array[i] << ", " << " val2: " << float_val2 << endl;
                #endif
                failure_flag = 1;
            }
            else {
                #ifdef DEBUG
                    cout << "val1: " << float_rand_array[i] << ", " << " val2: " << float_val2 << endl;
                #endif          
            }
            cur_index += 4;
        }

        if (failure_flag == 0) {
            cout << "Test " << std::to_string(counter) << " successful" << endl;
        }
        else {
            cout << "Test " << std::to_string(counter) << " failed" << endl;
        }

        counter++;
    }

 
    // free allocated data region using special _aligned_free() method
    try {
        _aligned_free(xsavedata);
        _aligned_free(xsavedata2);
    }
    catch (const std::exception& e) {
        cout << e.what() << endl;
    }

    return 0;
}
