cmake_minimum_required(VERSION 3.15)

project(oatpp-showcase-backend)

set(CMAKE_CXX_STANDARD 17)

# Find oatpp package (accept any version 1.3.0+)
find_package(oatpp REQUIRED)

add_executable(oatpp-showcase-exe
    src/App.cpp
    src/AppComponent.hpp
    src/controller/ApiController.hpp
    src/dto/DTOs.hpp
)

target_link_libraries(oatpp-showcase-exe
    PUBLIC oatpp::oatpp
)

target_include_directories(oatpp-showcase-exe PUBLIC src)

# Print oatpp version for debugging
message(STATUS "Using oatpp version: ${oatpp_VERSION}")