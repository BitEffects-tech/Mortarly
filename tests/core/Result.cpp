#include "core/Result.h"
#include <gtest/gtest.h>

#if USING_GTEST

class TestClass { };

TEST(CORE, SuccessfulResult) {
    Result<TestClass> result(std::make_shared<TestClass>());

    result.result([](const TestClass& i) { })
        .error([](const std::string& err) { GTEST_FAIL(); });
}

TEST(CORE, FailedResult) {
    const auto& result = Result<TestClass>("Invalid Test Class");

    result.result([](const TestClass& i) { GTEST_FAIL(); })
        .error([](const std::string& err) { });
}

#endif
