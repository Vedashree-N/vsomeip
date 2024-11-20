# Copyright (C) 2015-2017 Bayerische Motoren Werke Aktiengesellschaft (BMW AG)
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.

cmake_minimum_required(VERSION 3.13)
project(service-response)

find_package(Threads REQUIRED)

set(VSOMEIP_NAME "vsomeip3")

# create_target("executable")
function(create_target executable)
    add_library(vsomeip_service_response_${executable} INTERFACE)
    target_sources(vsomeip_service_response_${executable} INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/sample-ids.hpp"
    )
    target_compile_features(vsomeip_service_response_${executable} INTERFACE cxx_std_17)

    target_include_directories(vsomeip_service_response_${executable} INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}
    )
endfunction()

# link_target("executable")
function(link_target executable source_file)
    add_executable(service_response_${executable})
    target_sources(service_response_${executable} PRIVATE ${source_file})
    target_link_libraries(service_response_${executable} PRIVATE vsomeip_service_response_${executable} vsomeip3 Threads::Threads)
endfunction()

include_directories(${VSOMEIP_INCLUDE_DIRS})

# Create and link targets
create_target("request")
create_target("response")

if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Android")
    find_package(${VSOMEIP_NAME})
    if (NOT ${VSOMEIP_NAME}_FOUND)
        message("${VSOMEIP_NAME} was not found. Please specify vsomeip_DIR")
    endif()

    link_target("request" "request-sample.cpp")
    link_target("response" "response-sample.cpp")
endif()

