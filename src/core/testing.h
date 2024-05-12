#pragma once

#include "../pch.h"

namespace test {
    class GraphicalTest {
    public:
        GraphicalTest();
        ~GraphicalTest();

        void assertScreenshot(const std::string& fname);
    };

    class GraphicalTestResults {
    private:
        std::string moduleName;

    public:
        GraphicalTestResults(const std::string& moduleName);
        ~GraphicalTestResults();

        void addTest(const std::string& name);
    };
}
