cmake_minimum_required(VERSION 3.10)
project(f1_telem_API)

set(CMAKE_CXX_STANDARD 14)
find_package(Poco REQUIRED Foundation Util Net JSON)

if (CMAKE_BUILD_TYPE MATCHES Debug)
    add_definitions(-DDEBUG=1)
endif()

add_executable(f1_telem_API main.cpp f1structs/f1_storage.cpp f1structs/f1_storage.h shared_header.h udp_reciver.cpp udp_reciver.h)
target_link_libraries(f1_telem_API Poco::Net Poco::Foundation Poco::Util Poco::JSON)