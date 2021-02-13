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

def create_fn_parameter_str(rpc_info):
    if not "parameter" in rpc_info:
        return ""
    params = rpc_info["parameter"]
    ret = ""
    for param_name, param_type in params.items():
        ret += type_to_fn_parameter_pass_str(param_type) + " " + param_name
        if list(params.keys())[-1] != param_name:
            ret += ", "
    return ret

def create_return_type_str_client(service_name, rpc_name):
    return "Return" + service_name +  upperfirst(rpc_name)

def create_return_type_str_server(rpc_info, rpc_name):
    return "Return{}".format(upperfirst(rpc_name)) if "returns" in rpc_info else "void"

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

def create_member_cb_if(service_name):
    return "m_p{}If".format(upperfirst(service_name))

def create_member_cb_if_type(service_name):
    return "{}If".format(upperfirst(service_name))

def create_return_type_definition(return_type, content, tabs):
    struct_content = ""
    for member_name, member_type in content.items():
        struct_content += "{}\t{} {};\n".format(tabs, map_2_ret_type(member_type), member_name)
    return """\
{0}struct {1}
{0}{{
{2}{0}}};
""".format(tabs, return_type, struct_content)

#####################################################
################### CAPNP FILE ######################
#####################################################
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

    outStr += "struct RpcCoord {\n"
    outStr += "\tserviceId @0 :UInt16;\n"
    outStr += "\trpcId @1 :UInt16;\n"
    outStr += "}\n"

    # Create capnp type for parameter and return types
    for service_name in data["services"]:
        for rpc_name in data["services"][service_name]["rpc"]:
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_struct_str = "struct Parameter" + service_name +  upperfirst(rpc_name) + " {\n"
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

#####################################################
################### CLIENT H ########################
#####################################################
def create_capnzero_client_file_h_content_str(data, file_we):
    outStr = """\
#ifndef {0}_CLIENT_H
#define {0}_CLIENT_H

#include <zmq.hpp>
#include <thread>
#include "capnzero_typedefs.h"

namespace capnp {{ class MallocMessageBuilder; }}

namespace capnzero
{{

class {1}Client
{{
public:
    {1}Client();
""".format(file_we.upper(), file_we)
    for service_name in data["services"]:
        for rpc_name in data["services"][service_name]["rpc"]:
            return_type_str = "void"
            if "returns" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = create_return_type_str_client(service_name, rpc_name)
                return_struct_str = "\tstruct " + return_type_str + " {\n"
                members = data["services"][service_name]["rpc"][rpc_name]["returns"]
                for member_name, member_type in members.items():
                    return_struct_str += "\t\t" + map_2_ret_type(member_type) + " " + member_name + ";\n"  
                return_struct_str += "\t};\n"
                outStr += return_struct_str

            #if "returnType" in data["services"][service_name]["rpc"][rpc_name]:
            #    return_type_str = data["services"][service_name]["rpc"][rpc_name]["returnType"]
            parameter_str = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_str = create_fn_parameter_str(data["services"][service_name]["rpc"][rpc_name])
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
#endif
"""
    return outStr

#####################################################
################### CLIENT CPP ######################
#####################################################
def create_capnzero_client_file_cpp_content_str(data, file_we):
    outStr = '''\
#include "{0}_Client.h"
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "{0}.capnp.h"
#include "capnzero_utils.h"

using namespace capnzero;

{0}Client::{0}Client():
    m_zmqContext(0),
    m_zmqReqSocket(m_zmqContext, zmq::socket_type::req)
{{}}

'''.format(file_we)
    for service_name in data["services"]:
        for rpc_name in data["services"][service_name]["rpc"]:
            return_type_str = "void "
            if "returns" in data["services"][service_name]["rpc"][rpc_name]:
                return_type_str = file_we + "Client::" + create_return_type_str_client(service_name, rpc_name) + "\n"
            parameter_str = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                parameter_str = create_fn_parameter_str(data["services"][service_name]["rpc"][rpc_name])
            method_name = service_name + "__" + rpc_name
            outStr +=  return_type_str + file_we + "Client::" + method_name + "(" + parameter_str + "){\n"
            outStr += """\
    ::capnp::MallocMessageBuilder message;
    {{
        auto builder = message.initRoot<RpcCoord>();
        builder.setServiceId(to_underlying(ServiceId::{0}));
        builder.setRpcId(to_underlying({1}RpcIds::{2}));
""".format(to_snake_case(service_name).upper(), \
           service_name, to_snake_case(rpc_name).upper())

            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                outStr += "\t\tsend(message, zmq::send_flags::sndmore);\n"
            else:
                outStr += "\t\tsend(message, zmq::send_flags::dontwait);\n"
            outStr += "\t}\n"
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                outStr += " \t{\n"
                outStr += "\t\tauto paramBuilder = message.initRoot<{0}>();\n".format("Parameter" + service_name +  upperfirst(rpc_name))
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
                outStr += "\t" + create_return_type_str_client(service_name, rpc_name) + " retVal;\n"
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
void {0}Client::send(::capnp::MallocMessageBuilder& message,
                  const zmq::send_flags& sendFlags){{
    kj::Array<capnp::word> words = messageToFlatArray(message);
    kj::ArrayPtr<kj::byte> bytes = words.asBytes();
    m_zmqReqSocket.send(
        zmq::const_buffer(bytes.begin(), bytes.size()),
        sendFlags);
}}
""".format(file_we)
    return outStr

#####################################################
################### SERVER H ########################
#####################################################
def create_capnzero_server_file_h_content_str(data, file_we):
    cbif_includes = ""
    cbif_members = ""
    for service_name in data["services"]:
        cbif_includes += "#include \"{}{}.h\"\n".format(file_we, create_member_cb_if_type(service_name))
        cbif_members += "\tstd::unique_ptr<{}> {};\n".format(create_member_cb_if_type(service_name), \
                                                             create_member_cb_if(service_name))

    outStr = """\
#ifndef {0}_SERVER_H
#define {0}_SERVER_H

#include <zmq.hpp>
#include <thread>
#include <memory>
#include "capnzero_typedefs.h"

{1}

namespace capnzero
{{

class {2}Server
{{
public:
    {2}Server();
    void peekForRequests();
""".format(file_we.upper(), cbif_includes, file_we)

    outStr += """\
private:
    zmq::context_t m_zmqContext;
    zmq::socket_t m_zmqRepSocket;
{}
}};
}} // namespace capnzero
#endif
""".format(cbif_members)

    return outStr

#####################################################
################### SERVER CPP ######################
#####################################################
def create_capnzero_server_file_cpp_content_str(data, file_we):

    cases_str = ""
    for service_name in data["services"]:
        cases_str += "\t\tcase to_underlying(ServiceId::{}):\n".format(to_snake_case(service_name).upper())
        cases_str +=  "\t\t{\n"
        cases_str += "\t\t\tswitch(coordReader.getRpcId())\n"
        cases_str += "\t\t\t{\n"
        for rpc_name in data["services"][service_name]["rpc"]:
            cases_str += "\t\t\t\tcase to_underlying({0}RpcIds::{1}):\n".format(service_name, to_snake_case(rpc_name).upper())
            cases_str += "\t\t\t\t{\n"
            params = ""
            if "parameter" in data["services"][service_name]["rpc"][rpc_name]:
                cases_str += "\t\t\t\t\tzmq::message_t paramBuf;\n"
                cases_str += "\t\t\t\t\tauto res2 = m_zmqRepSocket.recv(paramBuf, zmq::recv_flags::dontwait);\n"
                cases_str += "\t\t\t\t\tif (!res2) { throw std::runtime_error(\"No received msg\"); }\n"
                cases_str += "\t\t\t\t\tauto paramReader = getReader<Parameter{}{}>(paramBuf);\n".format(service_name, upperfirst(rpc_name))
                for param_name, param_type in data["services"][service_name]["rpc"][rpc_name]["parameter"].items():
                   params += "pffui," 
            cases_str += "\t\t\t\t\t//{}->{}({});\n".format(create_member_cb_if(service_name), rpc_name, params)
            cases_str += "\t\t\t\t\tbreak;\n"
            cases_str += "\t\t\t\t}\n"
        cases_str += "\t\t\t}\n"
        cases_str +=  "\t\t\tbreak;\n"
        cases_str +=  "\t\t}\n"


    outStr = '''\
#include "{0}_Server.h"
#include <capnp/message.h>
#include <capnp/serialize.h>
#include "{0}.capnp.h"
#include "capnzero_utils.h"

using namespace capnzero;

template<typename T, typename MsgBuf>
typename T::Reader getReader(MsgBuf& msgBuf)
{{
	::capnp::FlatArrayMessageReader msgReader(
		kj::ArrayPtr<const capnp::word>(
			reinterpret_cast<const capnp::word*>( msgBuf.data() ), 
			msgBuf.size() / sizeof(capnp::word)
		)
	);
	return msgReader.getRoot<T>();
}}

{0}Server::{0}Server():
    m_zmqContext(0),
    m_zmqRepSocket(m_zmqContext, zmq::socket_type::rep)
{{}}

void {0}Server::peekForRequests() {{
    zmq::message_t rpcCoordBuf;
    auto res1 = m_zmqRepSocket.recv(rpcCoordBuf, zmq::recv_flags::dontwait);
    if (!res1) {{ return; }}
    auto coordReader = getReader<RpcCoord>(rpcCoordBuf);
    switch(coordReader.getServiceId())
    {{
{1}
    }}
}}

'''.format(file_we, cases_str)

    return outStr


#####################################################
############ RPC INTERFACE HEADERS ##################
#####################################################
def create_capnzero_cbif_h_content_str(service_name, rpc_infos, file_we):
    if_member_fns = ""
    for rpc_name in rpc_infos:
        return_type = create_return_type_str_server(rpc_infos[rpc_name], rpc_name)
        if "void" != return_type:
            if_member_fns += create_return_type_definition(return_type, rpc_infos[rpc_name]["returns"], "\t")
        parameter = create_fn_parameter_str(rpc_infos[rpc_name])
        if_member_fns += "\tvirtual {} {}({}) = 0;\n".format(return_type, rpc_name, parameter)

    outStr = """\
#ifndef {0}_H
#define {0}_H

#include "capnzero_typedefs.h"

namespace capnzero
{{

class {1}
{{
public:
    virtual ~{1}() = default;
{2}}};

}}
#endif
""".format(to_snake_case(file_we).upper(), create_member_cb_if_type(service_name), if_member_fns)

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
    open_file.write(create_capnzero_client_file_h_content_str(data, file_we))

with open(client_cpp_file, 'w') as open_file:
    open_file.write(create_capnzero_client_file_cpp_content_str(data, file_we))

with open(server_h_file, 'w') as open_file:
    open_file.write(create_capnzero_server_file_h_content_str(data, file_we))

with open(server_cpp_file, 'w') as open_file:
    open_file.write(create_capnzero_server_file_cpp_content_str(data, file_we))

for service_name in data["services"]:
    service = data["services"][service_name]
    if "rpc" in service:
        rpc_if_filename_we = file_we + create_member_cb_if_type(service_name)
        with open(outdir + "/" + rpc_if_filename_we + ".h", 'w') as open_file:
            open_file.write(create_capnzero_cbif_h_content_str(service_name, service["rpc"], rpc_if_filename_we))
