#include "schema/schema.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <capnp/serialize.h>
#include <capnp/compat/json.h>
#include <iostream>

int main()
{
    ::capnp::MallocMessageBuilder message;
    Person::Builder builder = message.initRoot<Person>();
    builder.setName("Gogoooo");
    builder.setEmail("gogoo@gmail.com");

    //::capnp::Text::Builder txtBuilder = builder.getName();
    auto arrOfArr = message.getSegmentsForOutput();
    std::cout << "arrOfArr.size(): " << arrOfArr.size() << "\n";
    std::cout << "size in bytes: " << arrOfArr[0].asBytes().size() << "\n";
    //std::cout << txtBuilder.cStr() << "\n";
    //Person person;
    //Date date;
    capnp::JsonCodec json;
    kj::String encoded = json.encode(message.getRoot<Person>());
    std::cout << encoded.cStr() << "\n";
    return 0;
}