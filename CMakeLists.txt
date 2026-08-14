cmake_minimum_required(VERSION 3.20)

project(Pulse VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(Pulse
    src/main.cpp
    src/platform/windows/Overlay.cpp
)

if (WIN32)
    target_link_libraries(Pulse PRIVATE user32)
endif()