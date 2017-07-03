
file(GLOB sources "*.proto")
foreach(source ${sources})
    string(REGEX REPLACE ".*/(.*).proto" "\\1"
        service ${source})
    message ("add library " ${service})
    set(pb_dir ${CMAKE_CURRENT_SOURCE_DIR}/pb/)
    add_custom_command(
        OUTPUT ${pb_dir}/${service}.pb.cc 
            ${pb_dir}/${service}.stub.cc
            ${pb_dir}/${service}.skeleton.cc
            ${pb_dir}/${service}.h
            ${pb_dir}/${service}.pb.h
        COMMAND mkdir -p ${pb_dir}
        COMMAND protoc --cpp_out=${pb_dir} -I${CMAKE_CURRENT_SOURCE_DIR}/ ${source}
        COMMAND protoc --plugin=protoc-gen-crpc=${CMAKE_CURRENT_BINARY_DIR}/rpc_gen --crpc_out=${pb_dir} -I${CMAKE_CURRENT_SOURCE_DIR}/ ${source}
        DEPENDS rpc_gen ${source}
        )
    add_library(${service} SHARED ${service}.cc)
    add_library(${service}.pb SHARED ${pb_dir}/${service}.pb.cc)
    add_library(${service}.stub SHARED ${pb_dir}/${service}.stub.cc)
    add_library(${service}.skeleton SHARED ${pb_dir}/${service}.skeleton.cc)
endforeach(source)
