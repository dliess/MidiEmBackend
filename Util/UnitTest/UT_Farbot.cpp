#include <gtest/gtest.h>
#include <farbot/AsyncCaller.hpp>
#include "functionMv.h"
#include <spdlog/spdlog.h>

using namespace util;

using AsyncCaller = farbot::AsyncCaller<farbot::fifo_options::concurrency::single, util::functionMv<120, void()>>; 

TEST(FarbotTest, usage) {
   int number = 55;
   util::functionMv<120, void()> f([&number](){
         spdlog::info("Testing testing {}", number);
         });
   AsyncCaller ac;
   ac.callAsync(std::move(f));
   ac.callAsync([](){  
      spdlog::info("I am the second callback");
         });
   spdlog::info("Before process() call");
   ac.process();
}
