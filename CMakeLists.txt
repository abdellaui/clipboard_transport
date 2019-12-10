cmake_minimum_required(VERSION 2.8.12)

project(clipboard_transport)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(
    ./include
    )


set( INCS
./include/ClipboardManager.h
)

set( SRCS
./src/ClipboardManager.cpp
./src/main.cpp
)



find_package(Qt5 COMPONENTS Core Gui REQUIRED)

add_executable(${PROJECT_NAME} ${INCS} ${SRCS})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Gui)
