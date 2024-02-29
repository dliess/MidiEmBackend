#include <gtest/gtest.h>
#include "function.h"
#include "spdlog/spdlog.h"

TEST(FunctionTest, CreateFunction) {
   util::function<64, int(int, int)> f([](int a, int b) { return a + b; });
   ASSERT_EQ(f(1, 2), 3);
}

TEST(FunctionTest, CreateFunctionWithCapture) {
   int x = 1;
   util::function<64, int(int)> f([x](int a) { return a + x; });
   ASSERT_EQ(f(2), 3);
}


TEST(FunctionTest, CopyTest) {
   struct A {
      A(const A&) { spdlog::info("A copy ctor"); }
      A(A&&) { spdlog::info("A move ctor"); }
      A() { spdlog::info("A ctor"); }
      ~A() { spdlog::info("A dtor"); }
      A& operator=(const A&) { spdlog::info("A copy assign"); return *this; }
      A& operator=(A&&) { spdlog::info("A move assign"); return *this;}
   };

   util::function<64, int(int, int)> f([obj = A()](int a, int b) { return a + b; });
   util::function<64, int(int, int)> g(std::move(f));
   ASSERT_EQ(g(1, 2), 3);
}
