#include "schema/schema.capnp.h"
#include <capnp/message.h>
#include <capnp/serialize-packed.h>
#include <capnp/serialize.h>
#include <iostream>

int main()
{
    ::capnp::MallocMessageBuilder message;
    Date::Builder builder = message.initRoot<Date>();
    //builder.setName("Dolly");
    //::capnp::Text::Builder txtBuilder = builder.getName();
    auto arrOfArr = message.getSegmentsForOutput();
    std::cout << arrOfArr[0].asBytes().size() << "\n";
    //std::cout << txtBuilder.cStr() << "\n";
    //Person person;
    //Date date;
    return 0;
}