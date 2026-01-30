cmake_minimum_required(VERSION 3.13)

# Get darabonba_core source directory
include(FetchContent)
FetchContent_GetProperties(_darabonba_core)
if(NOT _darabonba_core_POPULATED)
  FetchContent_Populate(_darabonba_core)
endif()

set(TEST_SOURCE_FILES
  test_basic.cpp
  test_config_model.cpp
  test_providers.cpp
  test_environment_default.cpp
  test_url_provider.cpp
  test_profile_provider.cpp
  test_client.cpp
  test_auth_util.cpp
  test_cli_profile_provider.cpp
  test_refreshable_provider.cpp
  test_edge_cases.cpp
  test_integration_scenarios.cpp
  test_ecs_ram_role_provider.cpp
  test_env_priority.cpp
)

add_executable(tests_AlibabaCloud_credential ${TEST_SOURCE_FILES})

# Ensure test executable uses dynamic runtime library on Windows
if(MSVC)
  # Force /MD flag using generator expressions (CMake 3.13+)
  target_compile_options(tests_AlibabaCloud_credential PRIVATE
    "$<$<CONFIG:Release>:/MD>"
    "$<$<CONFIG:MinSizeRel>:/MD>"
    "$<$<CONFIG:RelWithDebInfo>:/MD>"
    "$<$<CONFIG:Debug>:/MDd>")
  
  # Explicitly exclude static C++ runtime libraries (CMake 3.13+)
  target_link_options(tests_AlibabaCloud_credential PRIVATE
    /NODEFAULTLIB:libcmt
    /NODEFAULTLIB:libcpmt
    "$<$<CONFIG:Debug>:/NODEFAULTLIB:libcmtd>"
    "$<$<CONFIG:Debug>:/NODEFAULTLIB:libcpmtd>")
endif()

target_include_directories(tests_AlibabaCloud_credential
  PRIVATE
    ${CMAKE_SOURCE_DIR}/include
    ${_darabonba_core_SOURCE_DIR}/include
)

target_link_libraries(tests_AlibabaCloud_credential
  PRIVATE
    alibabacloud_credential
    gtest
    gtest_main
    darabonba_core
    OpenSSL::SSL
    OpenSSL::Crypto
)

# Platform-specific test link libraries
if(WIN32)
  target_link_libraries(tests_AlibabaCloud_credential PRIVATE ws2_32)
  
  # Copy alibabacloud_credential.dll to the executable directory
  add_custom_command(TARGET tests_AlibabaCloud_credential POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:alibabacloud_credential>
      $<TARGET_FILE_DIR:tests_AlibabaCloud_credential>
    COMMENT "Copying alibabacloud_credential.dll to test output directory"
  )
  
  # Copy darabonba_core.dll to the executable directory
  add_custom_command(TARGET tests_AlibabaCloud_credential POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:darabonba_core>
      $<TARGET_FILE_DIR:tests_AlibabaCloud_credential>
    COMMENT "Copying darabonba_core.dll to test output directory"
  )
  
  # Copy vcpkg DLLs (libcurl, OpenSSL, etc.) to output directory
  if(DEFINED VCPKG_INSTALLED_DIR)
    set(_vcpkg_bin_dir "${VCPKG_INSTALLED_DIR}/${VCPKG_TARGET_TRIPLET}/bin")
  elseif(DEFINED ENV{VCPKG_INSTALLATION_ROOT})
    set(_vcpkg_bin_dir "$ENV{VCPKG_INSTALLATION_ROOT}/installed/${VCPKG_TARGET_TRIPLET}/bin")
  elseif(DEFINED CMAKE_TOOLCHAIN_FILE)
    get_filename_component(_vcpkg_root "${CMAKE_TOOLCHAIN_FILE}" DIRECTORY)
    get_filename_component(_vcpkg_root "${_vcpkg_root}" DIRECTORY)
    set(_vcpkg_bin_dir "${_vcpkg_root}/installed/${VCPKG_TARGET_TRIPLET}/bin")
  endif()
  
  if(DEFINED _vcpkg_bin_dir AND EXISTS "${_vcpkg_bin_dir}")
    message(STATUS "Copying DLLs from vcpkg: ${_vcpkg_bin_dir}")
    file(GLOB _vcpkg_dlls 
      "${_vcpkg_bin_dir}/libcurl*.dll"
      "${_vcpkg_bin_dir}/libcrypto*.dll"
      "${_vcpkg_bin_dir}/libssl*.dll"
      "${_vcpkg_bin_dir}/zlib*.dll"
      "${_vcpkg_bin_dir}/nghttp2*.dll"
      "${_vcpkg_bin_dir}/brotli*.dll"
      "${_vcpkg_bin_dir}/zstd*.dll"
    )
    foreach(_dll ${_vcpkg_dlls})
      add_custom_command(TARGET tests_AlibabaCloud_credential POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
          "${_dll}"
          $<TARGET_FILE_DIR:tests_AlibabaCloud_credential>
        COMMENT "Copying ${_dll} to test output directory"
      )
    endforeach()
  else()
    message(WARNING "vcpkg bin directory not found, DLLs may need to be copied manually")
  endif()
elseif(UNIX AND NOT APPLE)
  target_link_libraries(tests_AlibabaCloud_credential PRIVATE pthread dl)
endif()

add_test(NAME google_test
  COMMAND $<TARGET_FILE:tests_AlibabaCloud_credential>
)
