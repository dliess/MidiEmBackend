#include "schema/schema.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <capnp/serialize.h>
#include <iostream>

int main()
{
    ::capnp::MallocMessageBuilder message;
    Person::Builder builder = message.initRoot<Person>();
    builder.setName("Dollyyyyyyyyyyyyyy.,sdmf.ssssssssssssssssssssggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggddss");
    builder.setEmail("Dollyyyyyyyyyyyyyy.,sdmf.ssssssssssssssssssssggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggggddss");

    //::capnp::Text::Builder txtBuilder = builder.getName();
    auto arrOfArr = message.getSegmentsForOutput();
    std::cout << "arrOfArr.size(): " << arrOfArr.size() << "\n";
    std::cout << "size in bytes: " << arrOfArr[0].asBytes().size() << "\n";
    //std::cout << txtBuilder.cStr() << "\n";
    //Person person;
    //Date date;
    return 0;
}