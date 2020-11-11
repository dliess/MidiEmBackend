#ifndef GRPC_BASE_SERVER_H
#define GRPC_BASE_SERVER_H

#include "protos/BaseService.grpc.pb.h"

namespace uiadapter::googlerpc
{

void RunServer();

class BaseServiceImpl final : public BaseService::Service
{
public:
   ::grpc::Status registerForParameterChange(
      ::grpc::ServerContext* context,
      const ::RegisterForParameterChangeRequest* request,
      ::Empty* response) override;
   ::grpc::Status unregisterForParameterChange(
      ::grpc::ServerContext* context,
      const ::RegisterForParameterChangeRequest* request,
      ::Empty* response) override;
   ::grpc::Status getParameterChanges(
      ::grpc::ServerContext* context, const ::Empty* request,
      ::grpc::ServerWriter< ::GetParameterChangesAnswer>* writer) override;
};

} // uiadapter::grpc

#endif