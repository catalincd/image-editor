cmake_minimum_required(VERSION 3.5)
project(YourProjectName)

find_package(OpenCV REQUIRED)
find_package(Crow CONFIG REQUIRED)

include_directories(${OpenCV_INCLUDE_DIRS})

# Server

file(GLOB server_src
    "src/server/server.cpp"
    "src/utils/packets/*.h"
    "src/utils/packets/*.cpp"
    "src/utils/*.h"
    "src/utils/*.cpp"
    "src/server/*/*.h"
    "src/server/*/*.cpp"
)

add_executable(server ${server_src})

target_link_libraries(server ${OpenCV_LIBS} Crow::Crow)

# Client
file(GLOB client_src
    "src/client/client.cpp"
    "src/utils/packets/*.h"
    "src/utils/packets/*.cpp"
    "src/utils/*.h"
    "src/utils/*.cpp"
    "src/client/*/*.h"
    "src/client/*/*.cpp"
)

add_executable(client ${client_src})


