
file(GLOB sources "*.proto")
foreach(source ${sources})
    string(REGEX REPLACE ".*/(.*).proto" "\\1"
        service ${source})
    message ("add library " ${service} ${source})
    add_custom_command(
        OUTPUT pb/${service}.pb.cc 
            pb/${service}.stub.cc
            pb/${service}.skeleton.cc
            pb/${service}.h
            pb/${service}.pb.h
        COMMAND mkdir -p pb
        COMMAND protoc --cpp_out=./pb -I${CMAKE_CURRENT_SOURCE_DIR}/ ${source}
        COMMAND protoc --plugin=protoc-gen-crpc=${PROJECT_BINARY_DIR}/rpc_gen --crpc_out=./pb -I${CMAKE_CURRENT_SOURCE_DIR}/ ${source}
        DEPENDS rpc_gen ${source}
        )
    include_directories(${CMAKE_CURRENT_BINARY_DIR})
    add_library(${service} SHARED ${service}.cc pb/test_s.h)
    SET_TARGET_PROPERTIES(${service} PROPERTIES PREFIX "")

    add_library(${service}.pb SHARED pb/${service}.pb.cc)
    SET_TARGET_PROPERTIES(${service}.pb PROPERTIES PREFIX "")

    add_library(${service}.stub SHARED pb/${service}.stub.cc)
    SET_TARGET_PROPERTIES(${service}.stub PROPERTIES PREFIX "")

    add_library(${service}.skeleton SHARED pb/${service}.skeleton.cc)
    SET_TARGET_PROPERTIES(${service}.skeleton PROPERTIES PREFIX "")
endforeach(source)
