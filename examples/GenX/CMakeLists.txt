cmake_minimum_required (VERSION 3.13)

# Set compiler flags
set (CMAKE_CXX_FLAGS "-g -std=c++0x")

# Find and include the Threads library
find_package (Threads REQUIRED)

# Include directories for vsomeip
include_directories (${VSOMEIP_INCLUDE_DIRS})

# Server 1 executable
add_executable(genx_server1 ../src/genx_MyServer1.cpp)
target_link_libraries(genx_server1 vsomeip3 ${CMAKE_THREAD_LIBS_INIT})

# Server 2 executable
add_executable(genx_server2 ../src/genx_MyServer2.cpp)
target_link_libraries(genx_server2 vsomeip3 ${CMAKE_THREAD_LIBS_INIT})

# Client executable
add_executable(genx_client ../src/genx_MyClientApplication.cpp)
target_link_libraries(genx_client vsomeip3 ${CMAKE_THREAD_LIBS_INIT} Threads::Threads)

