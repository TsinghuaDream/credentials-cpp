cmake_minimum_required(VERSION 3.13 FATAL_ERROR)

# Force GoogleTest to use shared CRT (MD) on Windows
if(MSVC)
  set(gtest_force_shared_crt ON CACHE BOOL "Force GoogleTest to use shared CRT" FORCE)
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreadedDLL" CACHE STRING "MSVC runtime library" FORCE)
  
  if(POLICY CMP0091)
    cmake_policy(SET CMP0091 NEW)
  endif()
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG        v1.14.0
)
FetchContent_GetProperties(googletest)

if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  
  # Prevent GoogleTest from using PThreads
  set(gtest_disable_pthreads ON CACHE BOOL "" FORCE)

  # Add the targets: gtest, gtest_main, gmock, gmock_main
  add_subdirectory(
    ${googletest_SOURCE_DIR}
    ${googletest_BINARY_DIR}
    EXCLUDE_FROM_ALL
  )

  # Configure GoogleTest targets for MSVC runtime consistency
  if(MSVC)
    foreach(_tgt gtest gtest_main gmock gmock_main)
      if(TARGET ${_tgt})
        target_compile_definitions(${_tgt}
          PRIVATE
            "_SILENCE_TR1_NAMESPACE_DEPRECATION_WARNING"
        )
        
        # Force /MD flag using generator expressions (CMake 3.13+)
        target_compile_options(${_tgt} PRIVATE
          "$<$<CONFIG:Release>:/MD>"
          "$<$<CONFIG:MinSizeRel>:/MD>"
          "$<$<CONFIG:RelWithDebInfo>:/MD>"
          "$<$<CONFIG:Debug>:/MDd>")
        
        # Explicitly exclude static C++ runtime libraries (CMake 3.13+)
        target_link_options(${_tgt} PRIVATE
          /NODEFAULTLIB:libcmt
          /NODEFAULTLIB:libcpmt
          "$<$<CONFIG:Debug>:/NODEFAULTLIB:libcmtd>"
          "$<$<CONFIG:Debug>:/NODEFAULTLIB:libcpmtd>")
      endif()
    endforeach()
  endif()
endif()
