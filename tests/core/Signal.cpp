#include "core/Signal.h"

#if USING_GTEST

class Sample {
public:
    Sample(Reactable<int> computed) { }
};


TEST(SIGNAL, ImplicitReactableConversion) {
    Sample sample_A{ConstantValue{0}};
    Sample sample_B{Signal{0}};
    Sample sample_C{0};
    Sample sample_D{Computed<int>{[]() { return 0; } }};
    Sample sample_E{[]() { return 0; } };
}

#endif
