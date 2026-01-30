cmake_minimum_required(VERSION 3.13)

project(alibabacloud_credential VERSION "0.1.0" LANGUAGES CXX)

# Build type options
option(BUILD_SHARED_LIBS "Build shared libraries" ON)
option(ENABLE_UNIT_TESTS "Enable unit tests" OFF)

# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< General set up >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> #
if(NOT CMAKE_CXX_STANDARD)
  set(CMAKE_CXX_STANDARD 11)
endif()
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

if(MSVC)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS -DNOMINMAX)
    # Ensure MSVC reports the correct C++ standard
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Zc:__cplusplus /utf-8")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
    if(APPLE)
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wno-deprecated-declarations")
    endif()
endif()

# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Dependencies >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> #
# Find dependencies before adding subdirectories to help with static link detection
find_package(OpenSSL REQUIRED)
find_package(CURL REQUIRED)
find_package(nlohmann_json REQUIRED QUIET)

# Handle external dependencies (e.g., darabonba_core)
add_subdirectory(external)

# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Target set up >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> #
file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")

add_library(${PROJECT_NAME} ${SOURCE_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# MSVC Runtime Library handling (MD/MDd) for CMake 3.13
if(MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE
    "$<$<CONFIG:Release>:/MD>"
    "$<$<CONFIG:MinSizeRel>:/MD>"
    "$<$<CONFIG:RelWithDebInfo>:/MD>"
    "$<$<CONFIG:Debug>:/MDd>")
endif()

target_include_directories(${PROJECT_NAME}
    PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)

target_link_libraries(${PROJECT_NAME}
    PUBLIC
    OpenSSL::SSL
    OpenSSL::Crypto
    CURL::libcurl
    darabonba_core
)

if(WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32)
elseif(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PRIVATE pthread dl)
endif()

set_target_properties(${PROJECT_NAME}
    PROPERTIES
    POSITION_INDEPENDENT_CODE ON
    SOVERSION ${PROJECT_VERSION_MAJOR}
    VERSION ${PROJECT_VERSION}
    OUTPUT_NAME "${PROJECT_NAME}"
    DEBUG_POSTFIX "_d"
    WINDOWS_EXPORT_ALL_SYMBOLS ON
)

if(ENABLE_UNIT_TESTS)
  enable_testing()
  add_subdirectory(tests)
endif()

# <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Install set up >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> #
# Standard path for CMake config files
set(INSTALL_CMAKEDIR "${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME}")

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(DIRECTORY include/
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}
    FILES_MATCHING PATTERN "*.hpp" PATTERN "*.h"
)

# Export targets to a file
install(EXPORT ${PROJECT_NAME}Targets
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${INSTALL_CMAKEDIR}
)

# Generate Config and Version files
include(CMakePackageConfigHelpers)

write_basic_package_version_file(
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY SameMajorVersion
)

# Create a Config.cmake that handles downstream dependencies
file(WRITE "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
"include(CMakeFindDependencyMacro)\n"
"find_dependency(OpenSSL)\n"
"find_dependency(CURL)\n"
"if(NOT TARGET darabonba_core)\n"
"  find_dependency(darabonba_core)\n"
"endif()\n"
"include(\"\${CMAKE_CURRENT_LIST_DIR}/${PROJECT_NAME}Targets.cmake\")\n"
)

install(FILES
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake"
    "${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake"
    DESTINATION ${INSTALL_CMAKEDIR}
)
