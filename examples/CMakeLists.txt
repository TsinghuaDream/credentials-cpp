cmake_minimum_required(VERSION 3.13)
project(credentials_examples CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find the credentials library
find_package(alibabacloud_credential REQUIRED)

# Example: All credential types demonstration
add_executable(credential_examples credential_examples.cpp)
target_link_libraries(credential_examples alibabacloud_credential::alibabacloud_credential)
