cmake_minimum_required(VERSION 3.20)

SET(OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/")

SET(FOLDER_MAIN "main")

SET(MAIN_PROJECT MAIN)
project(${MAIN_PROJECT} LANGUAGES C CXX)


include(cmake/dependencies.cmake)
include(cmake/CompilerWarnings.cmake)
include(cmake/pch.cmake)
include(cmake/cppcheck.cmake)


add_subdirectory("src")
# properties
set_target_properties(${MAIN_PROJECT} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${OUTPUT_DIRECTORY}")
set_target_properties(${MAIN_PROJECT} PROPERTIES CXX_STANDARD 23)
set_target_properties(${MAIN_PROJECT} PROPERTIES CXX_STANDARD_REQUIRED ON)
set_target_properties(${MAIN_PROJECT} PROPERTIES FOLDER ${FOLDER_MAIN})
#add_dependencies(${MAIN_PROJECT} cppcheck)
#include
target_include_directories(${MAIN_PROJECT} PRIVATE "${PROJECT_SOURCE_DIR}/external")
# compiler settings etc
apply_compiler_warnings(${MAIN_PROJECT})
use_pch(${MAIN_PROJECT})
