#include <gtest/gtest.h>
#include "itcQueue.h"
#include "itcAction.h"
#include "itcActionSender.h"

#include <type_traits>
#include <iostream>
#include <memory>
#include <thread>

using namespace util::itc;

struct Parameter{
   Parameter() noexcept { std::cout << "I got default constructed" << std::endl; }
   ~Parameter() noexcept { std::cout << "I got destructed" << std::endl; }
   Parameter(const Parameter& rhs) noexcept { std::cout << "I got copy-constructed" << std::endl; }
   Parameter(Parameter&& rhs) noexcept { std::cout << "I got move-constructed" << std::endl; }
   Parameter& operator=(const Parameter& rhs) noexcept { std::cout << "I got copy assigned" << std::endl; return *this; }
   Parameter& operator=(Parameter&& rhs) { std::cout << "I got move assigned" << std::endl; return *this; }
   int a;
};

using ParameterPtr =  std::shared_ptr<Parameter>;

struct ActionTag1 {};
struct ActionTag2 {};
using Action1 = Action<ActionTag1, int>;
using Action2 = Action<ActionTag1, int, float>;
using Action3 = Action<ActionTag1, ParameterPtr>;


struct ActionReceiverEmpty {};
struct ActionReceiver1 {
   void action(ActionTag1, int) { std::cout << "Action1 called by AR1" << std::endl; };
   void action(ActionTag1, int, float) { std::cout << "Action2 called by AR1" << std::endl; };
   void action(ActionTag1, ParameterPtr&& p) { std::cout << "Action3 called by AR1" << std::endl; };
};

struct ActionReceiver2 {
   void action(ActionTag1, int) { std::cout << "Action1 called by AR2" << std::endl; };
   void action(ActionTag2, int) { std::cout << "Action4 called by AR2" << std::endl; };
};


TEST(ItcActionReceiverTest, ActionReceiverIsTest){
   static_assert(!ActionReceiver::is<ActionReceiverEmpty, Action1>);
   static_assert(ActionReceiver::is<ActionReceiver1, Action1>);
   static_assert(ActionReceiver::is<ActionReceiver1, Action2>);
}


TEST(ItcQueueTest, tryPushTest){
   Queue queue;
   ActionReceiver1 ar;
   ActionData<Action2> params({std::make_tuple(3, 4.4f)});
   queue.tryPush(ar, params);
   queue.tryPush(ar, ActionData<Action2>({std::make_tuple(30, 41.4f)}));

   queue.tryPush(ar, ActionData<Action3>({ std::make_tuple(std::make_shared<Parameter>()) }));
   
   auto thread = std::thread([&queue](){
      queue.popCallAll();
   });
   thread.join();
}

/*
TEST(ItcActionSenderTest, pushTest1){
   Queue queue;
   ActionReceiver1 ar1;
   ActionReceiver2 ar2;
   ActionSender<ActionReceiver1, ActionReceiver2> actionSender(queue, ar1, ar2);
   actionSender.push( ActionData<Action1>( {std::make_tuple(11)} ) );
   actionSender.push( ActionData<Action4>( {std::make_tuple(13)} ) );
   queue.popCallAll();
}
*/

TEST(ItcActionSenderTest, pushTest2){
   Queue queue;
   ActionReceiver1 ar1;
   ActionReceiver2 ar2;
   ActionSender<ActionReceiver1, ActionReceiver2> actionSender(queue, ar1, ar2);
   actionSender.push( ActionTag1(), 4);
   actionSender.push( ActionTag2(), 54);
   
   actionSender.push( ActionTag1(), std::make_shared<Parameter>());
   auto thread = std::thread([&queue](){
      queue.popCallAll();
   });
   thread.join();
   
}