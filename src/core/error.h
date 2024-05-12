#pragma once

#include <iostream>

#ifdef _WIN32
    #include <windows.h>

    inline LONG WINAPI exceptionFilter(EXCEPTION_POINTERS* pExceptionPointers) {
        if (pExceptionPointers->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION) {
            std::cerr << "Access violation detected." << std::endl;
            return EXCEPTION_EXECUTE_HANDLER;
        }
        return EXCEPTION_CONTINUE_SEARCH;
    }

    inline void initDefaultExceptionHandler() {
        SetUnhandledExceptionFilter(exceptionFilter);
    }
#else
    #include <execinfo.h>
    #include <csignal>
    #include <unistd.h>
    #include <iostream>
    #include <cstdlib>

    inline void signalHandler(int signal) {
        std::cerr << "Error: signal " << signal << " (segmentation fault) caught." << std::endl;
        _exit(1);
    }

    inline void initDefaultExceptionHandler() {
        signal(SIGSEGV, signalHandler);
    }

    inline void printStackTrace() {
        void* callstack[128];
        int frames = backtrace(callstack, 128);
        char** strs = backtrace_symbols(callstack, frames);
        for (int i = 0; i < frames; ++i) {
            std::cout << strs[i] << std::endl;
        }
        free(strs);
    }
#endif
