cmake_minimum_required(VERSION 2.8.12)

project(clipboard_transport)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(
    ./include
    )


FILE(GLOB INCS ./include/*.h)

FILE(GLOB SRCS ./src/*.cpp)

find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(${PROJECT_NAME} ${INCS} ${SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui)
