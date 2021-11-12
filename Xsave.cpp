#include <immintrin.h>
#include <iostream>
#include <stdlib.h>
#include <memory>

template <std::size_t N>
struct MyAllocator
{
    char data[N];
    void* p;
    std::size_t sz;
    MyAllocator() : p(data), sz(N) {}
    template <typename T>
    T* aligned_alloc(std::size_t a = alignof(T))
    {
        if (std::align(a, sizeof(T), p, sz))
        {
            T* result = reinterpret_cast<T*>(p);
            p = (char*)p + sizeof(T);
            sz -= sizeof(T);
            return result;
        }
        return nullptr;
    }
};

using namespace std;

int main() {
	cout << "Hello, World!" << endl;

    MyAllocator<512> a;

	//static void* memory = (void*) calloc(10000, sizeof(float));
	void* memory = nullptr;

	std::size_t space = 512;
	void* aligned = std::align(16, 512, memory, space);

    int* p3 = a.aligned_alloc<int>(16);
    std::cout << "allocated an int at " << (int*)p3 << " (32 byte alignment)\n";

	_xsave(p3, 0x11111);
	free(memory);
	return 0;
}
