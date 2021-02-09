#!/usr/bin/python3

import toml
import getopt
import sys
import os.path

decapitalize = lambda s: s[:1].lower() + s[1:] if s else ''

def create_capnp_file_content_str(data):
    outStr = "@0x9c9f9131bf231692;\n\n"

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
        for rpc_name in data["services"][service_name]["rpc"]:
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_struct_str = "struct Parameter" + service_name +  rpc_name.capitalize() + " {\n" + \
                                        data["services"][service_name]["rpc"][rpc_name]["parameter"] + "}\n"
                full_parameter_struct_str = "struct FullParameter" + service_name +  rpc_name.capitalize() + " {\n" + \
                                            "serviceId @0 :ServiceId;\n" + \
                                            "rpcId @1 :" + service_name + "RpcIds;\n" + \
                                            "parameter @2 :Parameter" + service_name +  rpc_name.capitalize() + ";\n" + \
                                            "}\n"
                outStr += parameter_struct_str
                outStr += full_parameter_struct_str
    return outStr


def create_capnzero_client_file_h_content_str(data):
    outStr = """\
#ifndef CAPNZERO_CLIENT_H
#define CAPNZERO_CLIENT_H

#include <zmq.hpp>
#include <thread>
#include "Interface.capnp.h"

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
            if "returnType" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = data["services"][service_name]["rpc"][rpc_name]["returnType"]
            input_parameter_type_str = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                input_parameter_type_str = "const Parameter" + service_name +  rpc_name.capitalize() + " &param"
            method_name = service_name + "__" + rpc_name
            outStr +=  "\t" + return_type_str + " " + method_name + "(" + input_parameter_type_str + ");\n"

    outStr += """\
private:
    zmq::context_t m_zmqContext;
    zmq::socket_t m_zmqReqSocket;
};
} // namespace capnzero
#endif // CAPNZERO_CLIENT_H
"""
    return outStr


def create_capnzero_client_file_cpp_content_str(data, header_filename):
    outStr = '''\
#include "{0}"

using namespace capnzero;

Client::Client():
    m_zmqContext(0),
    m_zmqReqSocket(m_zmqContext, zmq::socket_type::req)
{{}}

'''.format(header_filename)
    for service_name in data["services"]:
        for rpc_name in data["services"][service_name]["rpc"]:
            return_type_str = "void"
            if "returnType" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = data["services"][service_name]["rpc"][rpc_name]["returnType"]
            input_parameter_type_str = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                input_parameter_type_str = "const Parameter" + service_name +  rpc_name.capitalize() + " &param"
            method_name = service_name + "__" + rpc_name
            outStr +=  return_type_str + " Client::" + method_name + "(" + input_parameter_type_str + "){\n"
            outStr +=  "}\n\n"

    return outStr


def create_capnzero_server_file_h_content_str(data):
    outStr = """\
    int i;
"""
    return outStr


def create_capnzero_server_file_cpp_content_str(data, header_filename):
    outStr = """\
    int j;
"""
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