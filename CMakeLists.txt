cmake_minimum_required(VERSION 3.10)

#Define variable Project_Name
set(Project_Name Emulator)

# Set the project name and specify that it uses C
project(${Project_Name} C)

include(CTest)

# Set the C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

 # For Debug configuration this will be evaluated to
 #   '${CMAKE_BINARY_DIR}/${OUTPUT_DEBUG}'
 # For Release configuration this will be evaluated to
 #   '${CMAKE_BINARY_DIR}/${OUTPUT_REL}'
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../bin/$<$<CONFIG:DEBUG>:${OUTPUT_DEBUG}>$<$<CONFIG:RELEASE>:${OUTPUT_REL}>")

# Include directories
include_directories(include)

# Source files
file(GLOB_RECURSE SOURCES "src/*.c")

# Create the main executable
add_executable(${Project_Name} ${SOURCES})

# Set C Standard
target_compile_features(${Project_Name} PRIVATE c_std_11)

# Enable Compiler Warnings per OS
if(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    #Add Preprocessor Definitions
    target_compile_definitions(${Project_Name} PRIVATE WINDOWS)
    target_compile_options(${Project_Name} PRIVATE /W4)
elseif (CMAKE_SYSTEM_NAME STREQUAL "Linux")
    #Add Preprocessor Definitions
    arget_compile_definitions(${Project_Name} PRIVATE LINUX)
    target_compile_options(${Project_Name} PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Testing

# Test Files
file(GLOB_RECURSE TESTS "tests/*.xme")

#Add Test
add_test(   NAME Test_01 COMMAND ${Project_Name} ${TESTS})