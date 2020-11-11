#include "gRPCServerAsync.h"

#include "protos/BaseService.grpc.pb.h"
#include "grpcpp/server_builder.h"

#include <loguru.hpp>

class CallData
{
public:
   CallData(BaseService::AsyncService* service,
            grpc::ServerCompletionQueue* cq) :
      m_service(service),
      m_cq(cq), m_responder(&m_ctx), m_state(State::CREATE)
   {
      Proceed();
   }

   void Proceed()
   {
      switch (m_state)
      {
         case State::CREATE:
         {
            m_service->RequestregisterForParameterChange(
               &m_ctx, &m_request, &m_responder, m_cq, m_cq, this);
            m_state = State::PROCESS;
            break;
         }
         case State::PROCESS:
         {
            // Spawn a new CallData instance to serve new clients while we
            // process the one for this CallData. The instance will deallocate
            // itself as part of its FINISH state.
            new CallData(m_service, m_cq);

            // The actual processing.
            // std::string prefix("Hello ");
            // m_reply.set_message(prefix + m_request.name());
            LOG_F(INFO, "Processing grpc Request {} {}", m_request.deviceid(), m_request.voiceid());


            m_responder.Finish(m_reply, grpc::Status::OK, this);
            m_state = State::FINISH;
            break;
         }
         case State::FINISH:
         {
            GPR_ASSERT(m_state == State::FINISH);
            LOG_F(INFO, "In FINISH state");
            delete this;
            break;
         }
      }
   }

private:
   BaseService::AsyncService* m_service;
   grpc::ServerContext m_ctx;
   grpc::ServerCompletionQueue* m_cq;
   grpc::ServerAsyncResponseWriter<Empty> m_responder;
   enum class State
   {
      CREATE,
      PROCESS,
      FINISH
   };
   State m_state;
   RegisterForParameterChangeRequest m_request;
   Empty m_reply;
};

void uiadapter::googlerpc::startAsyncServer()
{
   BaseService::AsyncService service;
   grpc::ServerBuilder builder;
   builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());
   builder.RegisterService(&service);
   auto cq     = builder.AddCompletionQueue();
   auto server = builder.BuildAndStart();

   new CallData(&service, cq.get());
   void* tag; // uniquely identifies a request.
   bool ok;
   gpr_timespec deadline;
   deadline.clock_type = GPR_TIMESPAN;
   deadline.tv_sec = 0;
   deadline.tv_nsec = 0;
   while (true)
   {
      // Block waiting to read the next event from the completion queue. The
      // event is uniquely identified by its tag, which in this case is the
      // memory address of a CallData instance.
      //cq->Next(&tag, &ok);
      auto ret = cq->AsyncNext(&tag, &ok, deadline);
      if(ret == grpc::CompletionQueue::NextStatus::TIMEOUT)
      {
         continue;
      }
      GPR_ASSERT(ok);
      static_cast<CallData*>(tag)->Proceed();
   }
}