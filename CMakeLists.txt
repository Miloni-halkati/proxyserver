cmake_minimum_required(VERSION 3.10)
project(ProxyServer)

set(CMAKE_CXX_STANDARD 17)

find_package(Poco REQUIRED Net Util Crypto Foundation)

include_directories(${Poco_INCLUDE_DIRS} include)

file(GLOB SOURCES "src/*.cpp")
add_executable(proxy ${SOURCES})

target_link_libraries(proxy ${Poco_LIBRARIES})
