#include <immintrin.h>
#include <iostream> // for cout
#include <stdlib.h>
#include <stdio.h>
#include <memory>   
#include <exception>

// CREDIT: 
// Pointer based memory alignment code comes from online C++ documentation:
/*
https://en.cppreference.com/w/cpp/memory/align
https://www.cplusplus.com/doc/tutorial/exceptions/
*/

using namespace std;

    int main() {
        cout << "Hello, World!" << endl;
        void* p5 = _aligned_malloc(512, 16);

        if (p5 == nullptr) {
            cout << "null reference on p5" << endl;
            return -1;
        }
        std::cout << "allocated an int at " << (void*)p5 << " (16 byte alignment)\n";

        int* beforepoint = (int*)p5;
        cout << " BEFORE POINT:" << endl;
        for (int i = 0; i < 512; i++) {
            cout << beforepoint[i] << endl;
        }

        _fxsave(p5);
       // _xsave(p5, 0x11111);
        int* intpoint = (int*) p5;

        // Bytes 464:511 are available to software use.The processor does not write to bytes 464 : 511 of an FXSAVE area.
        cout << "AFTER POINT" << endl;
        for (int i = 0; i < 512; i++) {
            cout << intpoint[i] << endl;
        }

        free(p5);
        return 0;
    }