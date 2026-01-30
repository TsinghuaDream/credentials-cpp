cmake_minimum_required(VERSION 3.13)

include(FetchContent)

# Try find_package first, then download from Git
find_package(darabonba_core QUIET)

if(darabonba_core_FOUND)
  message(STATUS "Found darabonba_core version ${darabonba_core_VERSION}")
  set(DARABONBA_CORE_FOUND TRUE PARENT_SCOPE)
  return()
else()
  message(STATUS "darabonba_core not found, building from source")
  
  # Prepare CMAKE_ARGS for darabonba_core
  set(_darabonba_core_CMAKE_ARGS
    -DENABLE_UNIT_TESTS:BOOL=OFF)
  
  # Ensure darabonba_core uses dynamic runtime library on Windows
  if(MSVC)
    list(APPEND _darabonba_core_CMAKE_ARGS
      -DCMAKE_CXX_FLAGS_RELEASE:STRING="/MD"
      -DCMAKE_CXX_FLAGS_DEBUG:STRING="/MDd"
      -DCMAKE_C_FLAGS_RELEASE:STRING="/MD"
      -DCMAKE_C_FLAGS_DEBUG:STRING="/MDd")
  endif()

  # Download from Git
  FetchContent_Declare(
    _darabonba_core
    GIT_REPOSITORY https://github.com/TsinghuaDream/tea-cpp.git
    GIT_TAG master
    CMAKE_ARGS ${_darabonba_core_CMAKE_ARGS}
  )

  set(ENABLE_UNIT_TESTS OFF)
  FetchContent_MakeAvailable(_darabonba_core)
  
  # Configure runtime library on darabonba_core target
  if(MSVC AND TARGET darabonba_core)
    # Force /MD flag on darabonba_core to ensure it uses dynamic runtime
    target_compile_options(darabonba_core PRIVATE
      "$<$<CONFIG:Release>:/MD>"
      "$<$<CONFIG:MinSizeRel>:/MD>"
      "$<$<CONFIG:RelWithDebInfo>:/MD>"
      "$<$<CONFIG:Debug>:/MDd>")
    
    # Explicitly exclude static C++ runtime libraries (CMake 3.13+ required)
    target_link_options(darabonba_core PRIVATE
      "$<$<CONFIG:Release>:/NODEFAULTLIB:libcpmt>"
      "$<$<CONFIG:MinSizeRel>:/NODEFAULTLIB:libcpmt>"
      "$<$<CONFIG:RelWithDebInfo>:/NODEFAULTLIB:libcpmt>"
      "$<$<CONFIG:Debug>:/NODEFAULTLIB:libcpmtd>")
  endif()

  # Linux-specific: link uuid library
  if(UNIX AND NOT APPLE)
    cmake_policy(SET CMP0079 NEW)
    
    find_package(PkgConfig QUIET)
    if(PkgConfig_FOUND)
      pkg_check_modules(UUID uuid)
      if(UUID_FOUND AND TARGET darabonba_core)
        target_link_libraries(darabonba_core PRIVATE ${UUID_LIBRARIES})
        target_include_directories(darabonba_core PRIVATE ${UUID_INCLUDE_DIRS})
      else()
        if(TARGET darabonba_core)
          target_link_libraries(darabonba_core PRIVATE uuid)
        endif()
      endif()
    else()
      if(TARGET darabonba_core)
        target_link_libraries(darabonba_core PRIVATE uuid)
      endif()
    endif()
  endif()

  message(STATUS "darabonba_core source: ${_darabonba_core_SOURCE_DIR}")
  message(STATUS "darabonba_core binary: ${_darabonba_core_BINARY_DIR}")

  target_include_directories(${PROJECT_NAME} PRIVATE "${_darabonba_core_SOURCE_DIR}/include")
  
  # Make the include directory available to parent scope for tests
  set(DARABONBA_CORE_INCLUDE_DIR "${_darabonba_core_SOURCE_DIR}/include" PARENT_SCOPE)
endif()