message(STATUS "Install libraries")

# Force GoogleTest to use shared CRT on Windows before adding subdirectories
if(MSVC AND ENABLE_UNIT_TESTS)
    set(gtest_force_shared_crt ON CACHE BOOL "Force GoogleTest to use shared CRT" FORCE)
endif()

add_subdirectory(darabonba_core)

if(ENABLE_UNIT_TESTS)
  # Suppress deprecation warnings from GoogleTest v1.12.1 (last C++11 compatible version)
  set(CMAKE_WARN_DEPRECATED OFF CACHE BOOL "" FORCE)
  add_subdirectory(googletest)
  set(CMAKE_WARN_DEPRECATED ON CACHE BOOL "" FORCE)
endif()
