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

   struct Fn {
      int a_;
      Fn() noexcept { spdlog::info("Fn ctor"); }
      Fn(const Fn&) noexcept { spdlog::info("Fn copy ctor"); }
      Fn(Fn&&) noexcept { spdlog::info("Fn move ctor"); }
      Fn& operator=(const Fn&) noexcept { spdlog::info("Fn copy assign"); return *this; }
      Fn& operator=(Fn&&) noexcept { spdlog::info("Fn move assign"); return *this; }
      int operator()(int a, int b) { return a + b + a_; }
   };
   //util::function<64, int(int, int)> f([obj = A()](int a, int b) { return a + b; });
   Fn fn;
   util::function<64, int(int, int)> f(fn);
   //util::function<64, int(int, int)> g(f);
   ASSERT_EQ(f(1, 2), 3);
   //ASSERT_EQ(g(1, 2), 3);
}
