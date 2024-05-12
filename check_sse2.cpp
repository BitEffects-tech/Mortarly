#include <iostream>

#if __WIN32
#include <intrin.h>
bool checkSSE2Support() {
    int info[4];
    __cpuid(info, 1);
    bool sse2Supported = (info[3] & (1 << 26)) != 0; // Check the 26th bit
    return sse2Supported;
}
#else
#include <cpuid.h>

bool checkSSE2Support() {
    unsigned int eax, ebx, ecx, edx;
    if (__get_cpuid(1, &eax, &ebx, &ecx, &edx)) {
        return (edx & (1 << 26)) != 0;  // Check the SSE2 bit in the edx register
    }
    return false;  // If __get_cpuid fails, assume SSE2 is not supported
}
#endif

int main() {
    if (checkSSE2Support()) {
        std::cout << "SSE2 is supported on this hardware.\n";
    } else {
        std::cout << "SSE2 is not supported on this hardware.\n";
    }
    return 0;
}
