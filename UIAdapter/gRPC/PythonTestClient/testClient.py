#!/usr/bin/python3

import grpc
import generated.BaseService_pb2 as Proto
import generated.BaseService_pb2_grpc as GRPC

def run():
   channel = grpc.insecure_channel('172.17.0.2:50051')
   stub = GRPC.BaseServiceStub(channel)
   req = Proto.RegisterForParameterChangeRequest(deviceId = "Moog/Mother32", voiceId = 0)
   stub.registerForParameterChange(req)
   
if __name__ == '__main__':
    run()