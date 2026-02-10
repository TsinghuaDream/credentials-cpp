cmake_minimum_required(VERSION 3.13)
project(credentials_examples CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Try to find the installed credentials library first
find_package(alibabacloud_credentials QUIET)

if(NOT alibabacloud_credentials_FOUND)
    message(STATUS "alibabacloud_credentials not found, attempting to build from source...")
    
    # Try to add parent directory as subdirectory (when building from source tree)
    if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/../CMakeLists.txt")
        message(STATUS "Building credentials library from parent directory")
        # Temporarily disable tests/examples to avoid recursion
        set(ENABLE_UNIT_TESTS OFF CACHE BOOL "" FORCE)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_BINARY_DIR}/credentials-cpp)
    else()
        message(FATAL_ERROR 
            "Could not find alibabacloud_credentials library.\n"
            "Please either:\n"
            "  1. Install the library: cd .. && mkdir build && cd build && cmake .. && cmake --build . && sudo cmake --install .\n"
            "  2. Build from source tree: cd .. && mkdir build && cd build && cmake .. && cmake --build .\n"
            "  3. Set CMAKE_PREFIX_PATH to the installation directory")
    endif()
endif()

# Example: All credential types demonstration
add_executable(credential_examples credential_examples.cpp)
target_link_libraries(credential_examples alibabacloud_credentials::alibabacloud_credentials)
