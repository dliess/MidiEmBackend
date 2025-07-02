#include <gtest/gtest.h>
#include "functionMv.h"
#include "spdlog/spdlog.h"

TEST(FunctionMvTest, CreateFunction) {
   util::functionMv<64, int(int, int)> f([](int a, int b) { return a + b; });
   ASSERT_EQ(f(1, 2), 3);
}

TEST(FunctionMvTest, CreateFunctionWithCapture) {
   int x = 1;
   util::functionMv<64, int(int)> f([x](int a) { return a + x; });
   ASSERT_EQ(f(2), 3);
}


TEST(FunctionMvTest, CopyTest) {
   struct A {
      A() { spdlog::info("A default constructor"); }
      ~A() { spdlog::info("A destructor"); }
      A(A&&) { spdlog::info("A move constructor"); }
      A& operator=(A&&) { spdlog::info("A move assignment operator"); return *this;}
      int val = 5;
   };

   util::functionMv<64, int(int, int)> f([obj = A()](int a, int b) { return a + b + obj.val; });
   util::functionMv<64, int(int, int)> g(std::move(f));
   // util::functionMv<64, int(int, int)> g(f); This should cause a compile time error and it is good so :)
   ASSERT_EQ(g(1, 2), 8);
}
