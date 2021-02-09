#include "schema/schema.capnp.h"
#include <capnp/message.h>
//#include <capnp/serialize-packed.h>
#include <iostream>

int main()
{
    ::capnp::MallocMessageBuilder message;
    Person::Builder builder = message.initRoot<Person>();
    builder.setName("Dolly");
    ::capnp::Text::Builder txtBuilder = builder.getName();
    std::cout << txtBuilder.cStr() << "\n";
    //Person person;
    //Date date;
    return 0;
}