#include <gtest/gtest.h>
#include "function_ref.h"

int ho_fn(util::function_ref<int(int, int)> fn, int a, int b)
{
    return fn(a, b);
}

TEST(FunctionRefTest, CopyConstruction) {
    int a = 5;
    util::function_ref<int(int, int)> fn = [a](int lhs, int rhs) -> int { return lhs + rhs + a; };
    ASSERT_EQ(fn(3, 4), 12);
    ASSERT_EQ(ho_fn(fn, 3, 4), fn(3, 4));
}