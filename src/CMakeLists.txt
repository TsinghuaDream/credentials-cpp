# 1. Source files - Explicitly listing files is preferred, but GLOB is kept if you have many files
file(GLOB_RECURSE CPP_FILES "*.cpp")

# 2. Add library - The target name is inherited from PROJECT_NAME in the root
add_library(${PROJECT_NAME} ${CPP_FILES})
add_library(${PROJECT_NAME}::${PROJECT_NAME} ALIAS ${PROJECT_NAME})

# 3. Public header directories
# Using CMAKE_CURRENT_SOURCE_DIR to ensure paths are absolute and robust
target_include_directories(${PROJECT_NAME}
  PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

# 4. Link dependencies 
# We assume the root CMakeLists.txt has already found these packages.
# Using Targets (::) ensures that include directories and compile definitions are inherited.
# Note: For static linking, order matters! Libraries that depend on others must come first.
# CURL depends on OpenSSL, so CURL must be listed before OpenSSL.
target_link_libraries(${PROJECT_NAME}
  PUBLIC
    darabonba_core
    CURL::libcurl
    OpenSSL::SSL
    OpenSSL::Crypto
)

# 5. Conditional dependencies for static builds
if(NOT BUILD_SHARED_LIBS)
  # ZLIB (commonly needed for static curl)
  find_package(ZLIB QUIET)
  if(TARGET ZLIB::ZLIB)
    target_link_libraries(${PROJECT_NAME} PUBLIC ZLIB::ZLIB)
  endif()
  
  # On Linux, static linking may need additional system libraries
  if(UNIX AND NOT APPLE)
    target_link_libraries(${PROJECT_NAME} PUBLIC dl pthread)
  endif()
endif()

# Platform-specific libraries
if(UNIX AND NOT APPLE)
  target_link_libraries(${PROJECT_NAME} PRIVATE pthread dl)
  # Try modern PkgConfig first for uuid
  find_package(PkgConfig QUIET)
  pkg_check_modules(UUID uuid)
  if(UUID_FOUND)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${UUID_LIBRARIES})
    target_include_directories(${PROJECT_NAME} PRIVATE ${UUID_INCLUDE_DIRS})
  else()
    # Fallback to manual search
    find_path(UUID_INCLUDE_DIR uuid/uuid.h)
    find_library(UUID_LIBRARY uuid)
    if(UUID_INCLUDE_DIR AND UUID_LIBRARY)
      target_include_directories(${PROJECT_NAME} PRIVATE ${UUID_INCLUDE_DIR})
      target_link_libraries(${PROJECT_NAME} PRIVATE ${UUID_LIBRARY})
    endif()
  endif()
endif()

# macOS specific Frameworks
if(APPLE)
  target_link_libraries(${PROJECT_NAME} PUBLIC "-framework CoreFoundation" "-framework Security")
endif()

# Windows specific libraries
if(WIN32)
  target_link_libraries(${PROJECT_NAME} PRIVATE ws2_32 crypt32)
endif()

# 6. Target Properties and Compiler Definitions
set_target_properties(${PROJECT_NAME} PROPERTIES
  POSITION_INDEPENDENT_CODE ON
  WINDOWS_EXPORT_ALL_SYMBOLS ON
  SOVERSION ${PROJECT_VERSION_MAJOR}
  VERSION ${PROJECT_VERSION}
)

# Add -Werror only for our own code (not external dependencies)
if(NOT MSVC)
  target_compile_options(${PROJECT_NAME} PRIVATE -Werror)
endif()

if(WIN32)
  target_compile_definitions(${PROJECT_NAME} PRIVATE ALIBABACLOUD_CREDENTIALS_BUILD)
  # Runtime library is now handled by CMAKE_MSVC_RUNTIME_LIBRARY in root CMakeLists.txt
endif()

# RTTI control
if(NOT ENABLE_RTTI)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /GR-)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE -fno-rtti)
  endif()
endif()

# Size optimization
if(MINIMIZE_SIZE)
  if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /O1 /Os)
  else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Os)
  endif()
endif()
