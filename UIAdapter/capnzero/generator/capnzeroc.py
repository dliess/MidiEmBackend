#!/usr/bin/python3

import toml
import getopt
import sys
import os.path

def upperfirst(x):
    return x[:1].upper() + x[1:]

decapitalize = lambda s: s[:1].lower() + s[1:] if s else ''

def to_snake_case(name):
    import re
    return re.sub(r'(?<!^)(?=[A-Z])', '_', name)

def is_integral_type(type):
    return type == "Int8" or \
           type == "Int16" or \
           type == "Int32" or \
           type == "Int64" or \
           type == "UInt8" or \
           type == "UInt16" or \
           type == "UInt32" or \
           type == "UInt64" or \
           type == "Float32" or \
           type == "Float64"

def type_to_fn_parameter_pass_str(type):
    if is_integral_type(type):
        return type
    else:
        return "const {}&".format(type)

def create_fn_parameter_str(params):
    ret = ""
    for param_name, param_type in params.items():
        ret += type_to_fn_parameter_pass_str(param_type) + " " + param_name
        if list(params.keys())[-1] != param_name:
            ret += ", "
    return ret

def create_return_type_str(service_name, rpc_name):
    return "Return" + service_name +  rpc_name.capitalize()

def create_capnp_return_type_str(service_name, rpc_name):
    return "CAPNPReturn" + service_name +  rpc_name.capitalize()

def map_descr_type_to_capnp_type(type):
    import re
    p = re.compile(r'Data<\d+>')
    if p.match(type) or type == "Span":
        return "Data"
    else:
        return type

def map_2_ret_type(type):
    if(type == "Data"):
        return "std::vector<uint8_t>"
    elif type == "Span":
        return "NOT A TYPE"  # TODO: replace with toml verification function
    else:
        return type

def create_capnp_file_content_str(data):
    outStr = """\
@0x9c9f9131bf231692;

"""

    if "capnpdata" in data:
        outStr += data["capnpdata"]

    # Create capnp enum for ServiceId 
    service_enum_str = "enum ServiceId {\n"
    for idx, service_name in enumerate(data["services"]):
        service_enum_str += "\t" + decapitalize(service_name) + " @" + str(idx) + ";\n"
    service_enum_str += "}\n"

    outStr += service_enum_str

    # Create capnp enum for rpc Ids 
    # convention for enum name: <service_name> + "RpcIds
    rpc_enum_strings = []
    for service_name in data["services"]:
        rpc_enum_string = "enum " + service_name + "RpcIds {\n"
        for idx, rpc_name in enumerate(data["services"][service_name]["rpc"]):
            rpc_enum_string += "\t" + rpc_name + " @" + str(idx) + ";\n"
        rpc_enum_string += "}\n"
        rpc_enum_strings.append(rpc_enum_string)

    for rpc_enum in rpc_enum_strings:
        outStr += rpc_enum

    # Create capnp type for parameter and return types
    for service_name in data["services"]:
        rpc_coord_str = "struct RpcCoord" + service_name + " {\n" + \
                                    "serviceId @0 :ServiceId;\n" + \
                                    "rpcId @1 :" + service_name + "RpcIds;\n" + \
                                    "}\n"
        outStr += rpc_coord_str
        for rpc_name in data["services"][service_name]["rpc"]:
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_struct_str = "struct Parameter" + service_name +  rpc_name.capitalize() + " {\n"
                params = data["services"][service_name]["rpc"][rpc_name]["parameter"]
                for idx, key in enumerate(params.keys()):
                    parameter_struct_str += "\t" + key + " @" + str(idx) + " :" + map_descr_type_to_capnp_type(params[key]) + ";\n"  
                parameter_struct_str += "}\n"
                outStr += parameter_struct_str
            if "returns" in data["services"][service_name]["rpc"][rpc_name]:
                return_struct_str = "struct " + create_capnp_return_type_str(service_name, rpc_name) + " {\n"
                members = data["services"][service_name]["rpc"][rpc_name]["returns"]
                for idx, key in enumerate(members.keys()):
                    return_struct_str += "\t" + key + " @" + str(idx) + " :" + map_descr_type_to_capnp_type(members[key]) + ";\n"  
                return_struct_str += "}\n"
                outStr += return_struct_str

    return outStr

def create_capnzero_client_file_h_content_str(data):
    outStr = """\
#ifndef CAPNZERO_CLIENT_H
#define CAPNZERO_CLIENT_H

#include <zmq.hpp>
#include <thread>
#include "capnzero_typedefs.h"

namespace capnp { class MallocMessageBuilder; }

namespace capnzero
{

class Client
{
public:
    Client();
"""
    for service_name in data["services"]:
        for rpc_name in data["services"][service_name]["rpc"]:
            return_type_str = "void"
            if "returns" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = create_return_type_str(service_name, rpc_name)
                return_struct_str = "\tstruct " + return_type_str + " {\n"
                members = data["services"][service_name]["rpc"][rpc_name]["returns"]
                for member_name, member_type in members.items():
                    return_struct_str += "\t\t" + map_2_ret_type(member_type) + " " + member_name + ";\n"  
                return_struct_str += "\t};\n"
                outStr += return_struct_str

            if "returnType" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = data["services"][service_name]["rpc"][rpc_name]["returnType"]
            parameter_str = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_str = create_fn_parameter_str(data["services"][service_name]["rpc"][rpc_name]["parameter"])
            method_name = service_name + "__" + rpc_name
            outStr +=  "\t" + return_type_str
            outStr += " " if len(return_type_str) < 8 else "\n\t"
            outStr += method_name + "(" + parameter_str + ");\n"

    outStr += """\
private:
    zmq::context_t m_zmqContext;
    zmq::socket_t m_zmqReqSocket;
    void send(::capnp::MallocMessageBuilder& message,
              const zmq::send_flags& sendFlags);

};
} // namespace capnzero
#endif // CAPNZERO_CLIENT_H
"""
    return outStr


def create_capnzero_client_file_cpp_content_str(data, header_filename):
    outStr = '''\
#include "{0}"
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "Interface.capnp.h"

using namespace capnzero;

Client::Client():
    m_zmqContext(0),
    m_zmqReqSocket(m_zmqContext, zmq::socket_type::req)
{{}}

'''.format(header_filename)
    for service_name in data["services"]:
        for rpc_name in data["services"][service_name]["rpc"]:
            return_type_str = "void "
            if "returns" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = "Client::" + create_return_type_str(service_name, rpc_name) + "\n"
            parameter_str = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_str = create_fn_parameter_str(data["services"][service_name]["rpc"][rpc_name]["parameter"])
            method_name = service_name + "__" + rpc_name
            outStr +=  return_type_str + "Client::" + method_name + "(" + parameter_str + "){\n"
            outStr += """\
    ::capnp::MallocMessageBuilder message;
    {{
        auto builder = message.initRoot<{0}>();
        builder.setServiceId(ServiceId::{1});
        builder.setRpcId({2}RpcIds::{3});
""".format("RpcCoord" + service_name, \
           to_snake_case(service_name).upper(), \
           service_name, to_snake_case(rpc_name).upper())

            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                outStr += "\t\tsend(message, zmq::send_flags::sndmore);\n"
            else:
                outStr += "\t\tsend(message, zmq::send_flags::dontwait);\n"
            outStr += "\t}\n"
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                outStr += " \t{\n"
                outStr += "\t\tauto paramBuilder = message.initRoot<{0}>();\n".format("Parameter" + service_name +  rpc_name.capitalize())
                params = data["services"][service_name]["rpc"][rpc_name]["parameter"]
                for param_name, param_type in params.items():
                    if(map_descr_type_to_capnp_type(param_type) == 'Data'):
                        outStr += "\t\tcapnp::Data::Reader reader({0}.data(), {0}.size());\n".format(param_name)
                        outStr += "\t\tparamBuilder.set{0}(reader);\n".format(upperfirst(param_name))
                    else:
                        outStr += "\t\tparamBuilder.set{0}({1});\n".format(upperfirst(param_name), param_name)
                outStr += "\t\tsend(message, zmq::send_flags::dontwait);\n"
                outStr += "\t}\n"
            outStr += "\tzmq::message_t revcMsg;\n"
            outStr += "\tauto recvRes = m_zmqReqSocket.recv(revcMsg);\n"
            outStr += "\tif(!recvRes)\n"
            outStr += "\t{\n"
            outStr += "\t\tthrow std::runtime_error(\"recv failed, nothing received\");\n"
            outStr += "\t}\n"

            if "returns" in data["services"][service_name]["rpc"][rpc_name]:
                outStr += "\t::capnp::FlatArrayMessageReader readMessage(\n"
                outStr += "\tkj::ArrayPtr<const capnp::word>(reinterpret_cast<const capnp::word*>(revcMsg.data()), revcMsg.size() / sizeof(capnp::word) )\n"
                outStr += "\t);\n"
                outStr += "\tauto reader = readMessage.getRoot<{}>();\n".format(create_capnp_return_type_str(service_name, rpc_name))
                outStr += "\t" + create_return_type_str(service_name, rpc_name) + " retVal;\n"
                ret_members = data["services"][service_name]["rpc"][rpc_name]["returns"]
                for member_name, member_type in ret_members.items():
                    if map_descr_type_to_capnp_type(member_type) == 'Data':
                        outStr += "\tauto src = reader.get{}();\n".format(upperfirst(member_name))
                        outStr += "\tassert(src.size() == retVal.{}.size());\n".format(member_name)
                        outStr += "\tstd::copy(src.begin(), src.end(), retVal.{}.begin());\n".format(member_name)
                    else:
                        outStr += "\tretVal.{0} = reader.get{1}();\n".format(member_name, upperfirst(member_name))
                outStr += "\treturn retVal;\n"
            outStr +=  "}\n\n"

    outStr += """\
void Client::send(::capnp::MallocMessageBuilder& message,
                  const zmq::send_flags& sendFlags){
    kj::Array<capnp::word> words = messageToFlatArray(message);
    kj::ArrayPtr<kj::byte> bytes = words.asBytes();
    m_zmqReqSocket.send(
        zmq::const_buffer(bytes.begin(), bytes.size()),
        sendFlags);
}
"""
    return outStr


def create_capnzero_server_file_h_content_str(data):
    outStr = """\
int i;
"""
    return outStr


def create_capnzero_server_file_cpp_content_str(data, header_filename):
    outStr = """\
#include "{0}"
""".format(header_filename)
    return outStr




outdir="undefined"
descrfile="undefined"
options, remainder = getopt.getopt(sys.argv[1:], ['o:d:'], ['outdir=', 'descrfile='])
for opt, arg in options:
    if opt in ('-o', '--outdir'):
        outdir = arg
    elif opt in ('-d', '--descrfile'):
        descrfile = arg

file_we = os.path.splitext(os.path.basename(descrfile))[0]

print("outdir: " + outdir)
print("descrfile: " + descrfile)
print("file_we: " + file_we)

capnp_file = outdir + "/" + file_we + ".capnp"
client_h_file = outdir + "/" + file_we + "_Client.h"
client_cpp_file = outdir + "/" + file_we + "_Client.cpp"
server_h_file = outdir + "/" + file_we + "_Server.h"
server_cpp_file = outdir + "/" + file_we + "_Server.cpp"

data = toml.load(descrfile)
with open(capnp_file, 'w') as open_file:
    open_file.write(create_capnp_file_content_str(data))

with open(client_h_file, 'w') as open_file:
    open_file.write(create_capnzero_client_file_h_content_str(data))

with open(client_cpp_file, 'w') as open_file:
    open_file.write(create_capnzero_client_file_cpp_content_str(data, file_we + "_Client.h"))

with open(server_h_file, 'w') as open_file:
    open_file.write(create_capnzero_server_file_h_content_str(data))

with open(server_cpp_file, 'w') as open_file:
    open_file.write(create_capnzero_server_file_cpp_content_str(data, file_we + "_Server.h"))