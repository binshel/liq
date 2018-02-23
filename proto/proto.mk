
CXX += --std=gnu++11 -Wall
CXXFLAGS = -fPIC -g -O0 -I../include -I./cpp/proto
LDFLAGS = -fPIC

RPC_GEN_PATH = ../src/rpc_gen/pb2cpp
RPC_CHECK_PATH = ../src/rpc_gen/pbvalide
PROTOC_FLAGS = -I./src -I/usr/include 

proto_dir = src
cpp_dir = cpp/proto
pb_dir = pb
idl_dir = idl


# 全部的 proto 文件
protos = $(patsubst ${proto_dir}/%,%,$(shell find ${proto_dir} -name '*.proto'))
proto_names = $(patsubst %.proto,%,${protos})

# 包含 service 定义的 proto 文件
services = $(shell for proto in $(protos) ; do \
			   service=`grep '\<service\>' ${proto_dir}/$${proto}`; \
			   if [ "X$${service}" != "X" ] ; then \
				   echo $${proto}; \
			   fi \
		   done)
service_names = $(patsubst %.proto,%,${services})

pbs = $(foreach name, ${service_names}, ${pb_dir}/${name}.pb)
cpp_pbs = $(foreach name, ${proto_names}, ${cpp_dir}/${name}.pb.cc)
cpp_pb_objects = $(foreach name, ${proto_names}, ${cpp_dir}/${name}.pb.o)
stubs = $(foreach name, ${service_names}, ${cpp_dir}/${name}.stub.so)
skeletons = $(foreach name, ${service_names}, ${cpp_dir}/${name}.skeleton.so)
headers = $(foreach name, ${service_names}, ${cpp_dir}/${name}.h)
idls = $(foreach name, ${service_names}, ${idl_dir}/${name}.idl)



all: all-pre pb cpp_pb stub skeleton header idl liq.pb.so
all-pre:
	echo ${protos}
	echo ${proto_names}
	echo ${cpp_pbs}
	echo ${service_names}
	$(MAKE) -C $(dir $(RPC_GEN_PATH))
install: all
	mkdir -p ${prefix}/pb/liq
	mkdir -p ${prefix}/idl/liq
	mkdir -p ${prefix}/lib/liq
	cp -ar ${pb_dir}/*  ${prefix}/pb/liq
	cp -ar ${idl_dir}/*  ${prefix}/idl/liq
	cp -ar liq.pb.so ${cpp_dir}/*  ${prefix}/lib/liq
uninstall:
	rm -fr ${prefix}/pb/liq
	rm -fr ${prefix}/idl/liq
	rm -fr  ${prefix}/lib/liq
	rm -f ${prefix}/lib/liq/liq.pb.so
clean:
	rm -fr ${pb_dir} ${idl_dir} ${cpp_dir}
	rm -f liq.pb.so

pb: ${pbs}
cpp_pb: ${cpp_pbs}
stub: ${stubs}
skeleton: ${skeletons}
header: ${headers}
idl: ${idls}

# 生成 pb 文件
${pb_dir}/%.pb: ${proto_dir}/%.proto
	mkdir -p $(dir $@)
	protoc --include_imports $(PROTOC_FLAGS) -o$@ $<

# 生成 c++ 源文件
${cpp_dir}/%.pb.h ${cpp_dir}/%.pb.cc: ${proto_dir}/%.proto
	mkdir -p ${cpp_dir}
	protoc $(PROTOC_FLAGS) --cpp_out=${cpp_dir} $<
${cpp_dir}/%.stub.cc ${cpp_dir}/%.skeleton.cc ${cpp_dir}/%.h ${idl_dir}/%.idl: ${proto_dir}/%.proto
	mkdir -p $(dir ${cpp_dir}/$*)
	mkdir -p $(dir ${idl_dir}/$*)
	$(RPC_GEN_PATH) $(PROTOC_FLAGS) -o$(dir ${cpp_dir}/$*) -i$(dir ${idl_dir}/$*) -p ${dir $*} -f $<

# 生成 stub 与 skeleton
${cpp_dir}/%.cc: $(cpp_pbs)
${cpp_dir}/%.stub.so: ${cpp_dir}/%.stub.o
	$(CC) --shared $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $@ $<
${cpp_dir}/%.skeleton.so: ${cpp_dir}/%.skeleton.o
	$(CC) --shared $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $@ $<

# 所用 c++ 打包成动态库
liq.pb.so: $(cpp_pb_objects)
	$(CC) --shared $(LDFLAGS) $(LOADLIBES) $(LDLIBS) -o $@ $^


SUFFIXES = .cc .o
.PRECIOUS :  $(foreach name, ${service_names}, ./cpp/proto/${name}.stub.cc)
