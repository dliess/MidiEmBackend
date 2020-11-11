PROTO_BASE_NAME := BaseService
INSTALL_DIR := generated
PROTO_PATH := ../protos
PROTO_FILE := $(PROTO_PATH)/$(PROTO_BASE_NAME).proto

dep_all:
	mkdir -p "$(INSTALL_DIR)"
	python3 -m grpc_tools.protoc -I$(PROTO_PATH)  --python_out="$(INSTALL_DIR)" --grpc_python_out="$(INSTALL_DIR)" "$(PROTO_FILE)"
	sed -i "s/import $(PROTO_BASE_NAME)_pb2 as $(PROTO_BASE_NAME)__pb2/from . import $(PROTO_BASE_NAME)_pb2 as $(PROTO_BASE_NAME)__pb2/" $(INSTALL_DIR)/$(PROTO_BASE_NAME)_pb2_grpc.py