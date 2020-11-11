#include "gRPCServer.h"

#include <grpcpp/server_builder.h>
#include <memory>
#include <iostream>

using namespace grpc;

void uiadapter::googlerpc::RunServer()
{
   std::string server_address("0.0.0.0:50051");
   BaseServiceImpl service;
   ServerBuilder builder;
   builder.AddListeningPort(server_address, InsecureServerCredentials());
   builder.RegisterService(&service);
   std::unique_ptr<Server> server(builder.BuildAndStart());
   std::cout << "Server listening on " << server_address << std::endl;
   server->Wait();
}

Status uiadapter::googlerpc::BaseServiceImpl::registerForParameterChange(
   ServerContext* context, const ::RegisterForParameterChangeRequest* request,
   ::Empty* response)
{
   return Status::OK;
}

Status uiadapter::googlerpc::BaseServiceImpl::unregisterForParameterChange(
   ServerContext* context, const ::RegisterForParameterChangeRequest* request,
   ::Empty* response)
{
   return Status::OK;
}

Status uiadapter::googlerpc::BaseServiceImpl::getParameterChanges(
   ServerContext* context, const ::Empty* request,
   ServerWriter< ::GetParameterChangesAnswer>* writer)
{
   return Status::OK;
}
