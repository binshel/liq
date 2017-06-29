cmake_minimum_required(VERSION 2.8)

project(liq)

set(CMAKE_VERBOSE_MAKEFILE  ON)
set(CMAKE_BUILD_TYPE debug)

include_directories(./include)
set(CMAKE_CXX_FLAGS "-std=gnu++11")

file(GLOB rpc_gen_sources "src/rpc_gen/*.cc")
add_executable(rpc_gen ${rpc_gen_sources})
target_link_libraries(rpc_gen protobuf protoc)

file(GLOB liq_sources "src/lib/*.cc")
add_library(liq SHARED ${liq_sources})
target_link_libraries(liq boost_filesystem config++ dl)

file(GLOB test_source "test/*.cc")
add_executable(test ${test_source})
target_link_libraries(test liq)

file(GLOB sources "test/*.proto")
foreach(source ${sources})
    string(REGEX REPLACE ".*/(.*).proto" "\\1"
        service ${source})
    message ("add library " ${service})
    add_custom_command(
        OUTOUT test/.rpc/${service}.pb.cc test/.rpc/${service}.stub.cc test/.rpc/${service}.skeleton.cc
        COMMAND mkdir -p test/.rpc
        COMMAND protoc --cpp_out=test/.rpc/ ${source}
        COMMAND protoc --plugin=protoc-gen-crpc=EXECUTABLE_OUTPUT_PAHT/crpc_gen --crpc_out=test/.rpc/ ${source}
        )
    add_library(${service}.pb SHARED test/.rpc/${service}.pb.cc)
    add_library(${service}.stub SHARED test/.rpc/${service}.stub.cc)
    add_library(${service}.skeleton SHARED test/.rpc/${service}.skeleton.cc)
endforeach(source)
