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
target_link_libraries(liq boost_filesystem dl protobuf)

file(GLOB test_source "test/test.cc")
add_executable(test ${test_source})
target_link_libraries(test liq)

file(GLOB sources "test/*.proto")
foreach(source ${sources})
    string(REGEX REPLACE ".*/(.*).proto" "\\1"
        service ${source})
    message ("add library " ${service})
    set(pb_dir ${CMAKE_CURRENT_SOURCE_DIR}/test/pb/)
    add_custom_command(
        OUTPUT ${pb_dir}/${service}.pb.cc 
            ${pb_dir}/${service}.stub.cc
            ${pb_dir}/${service}.skeleton.cc
            ${pb_dir}/${service}.h
            ${pb_dir}/${service}.pb.h
        COMMAND mkdir -p ${pb_dir}
        COMMAND protoc --cpp_out=${pb_dir} -I${CMAKE_CURRENT_SOURCE_DIR}/test ${source}
        COMMAND protoc --plugin=protoc-gen-crpc=${CMAKE_CURRENT_BINARY_DIR}/rpc_gen --crpc_out=${pb_dir} -I${CMAKE_CURRENT_SOURCE_DIR}/test ${source}
        DEPENDS rpc_gen ${source}
        )
    add_library(${service} SHARED test/${service}.cc)
    add_library(${service}.pb SHARED ${pb_dir}/${service}.pb.cc)
    add_library(${service}.stub SHARED ${pb_dir}/${service}.stub.cc)
    add_library(${service}.skeleton SHARED ${pb_dir}/${service}.skeleton.cc)
endforeach(source)
