cmake_minimum_required(VERSION 3.8)

project(liq)

set(CMAKE_VERBOSE_MAKEFILE  ON)
set(CMAKE_BUILD_TYPE debug)

include_directories(./include)
set(CMAKE_CXX_FLAGS "-std=gnu++11 -L./")

file(GLOB rpc_gen_sources "src/rpc_gen/*.cc")
add_executable(rpc_gen ${rpc_gen_sources})
target_link_libraries(rpc_gen protobuf protoc)


file(GLOB liq_sources "src/lib/*.cc")
add_library(liq SHARED ${liq_sources})
target_link_libraries(liq boost_filesystem dl protobuf shmchannel)
target_include_directories(liq PUBLIC "/home/ezfun/server/frame/src/comm/shm_channel")

file(GLOB sol_sources "src/sol/*.cc")
add_executable(sol ${sol_sources})
target_link_libraries(sol liq)

add_subdirectory(test)

