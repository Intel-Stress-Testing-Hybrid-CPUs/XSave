#include <iostream>
using namespace std;
#include <immintrin.h>
#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <cstdio>
#include <cstdlib>

// main() is where program execution begins.
template <std::size_t N>
int main() {


	void* ptr = (void*)malloc(sizeof(long) * 1000);
	std::size_t sz;
	std::size_t space = size;
	void* aligned = std::align(16, 1024, ptr, space);


	//void* p = nullptr;
	_xsave(aligned, 0x11111);

	cout << "Hello World"; // prints Hello World
	return 0;
}