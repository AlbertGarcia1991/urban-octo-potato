# The following lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)


option(BUILD_TARGET "Choose BLE build target: SERVER or CLIENT" "SERVER")
if(BUILD_TARGET STREQUAL "SERVER")
    add_compile_definitions(BUILD_SERVER=1)
elseif(BUILD_TARGET STREQUAL "CLIENT")
    add_compile_definitions(BUILD_CLIENT=1)
endif()

set(
    EXTRA_COMPONENT_DIRS
    ${CMAKE_SOURCE_DIR}/src/main
    ${CMAKE_SOURCE_DIR}/src/os
    ${CMAKE_SOURCE_DIR}/src/managers/cli
    ${CMAKE_SOURCE_DIR}/src/managers/uart
    ${CMAKE_SOURCE_DIR}/src/managers/nvs
    ${CMAKE_SOURCE_DIR}/src/managers/ble
    ${CMAKE_SOURCE_DIR}/src/utils
)
    
include($ENV{IDF_PATH}/tools/cmake/project.cmake)

# "Trim" the build. Include the minimal set of components, main, and anything it depends on.
idf_build_set_property(MINIMAL_BUILD ON)

message(STATUS "Building the project (BUILD_SERVER=${BUILD_TARGET}): ${PROJECT_NAME} v${PROJECT_VERSION}")
    
# Project name and version
project(GymHand VERSION 0.0.1)