file(GLOB_RECURSE CPP_FILES "*.cpp")

add_executable(${PROJECT_NAME}Test ${CPP_FILES})

# include
target_include_directories(${PROJECT_NAME}Test
                           PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# googletest
target_link_libraries(${PROJECT_NAME}Test ${PROJECT_NAME} gtest gmock gtest_main)

# On Windows with shared libs, copy the DLL to the test executable directory
if(WIN32 AND BUILD_SHARED_LIBS)
  # Copy credentials DLL
  add_custom_command(TARGET ${PROJECT_NAME}Test POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    $<TARGET_FILE:${PROJECT_NAME}>
    $<TARGET_FILE_DIR:${PROJECT_NAME}Test>
    COMMENT "Copying ${PROJECT_NAME} DLL to test executable directory"
  )
  
  # Copy darabonba_core DLL if it's a shared library
  if(TARGET darabonba_core::darabonba_core)
    add_custom_command(TARGET ${PROJECT_NAME}Test POST_BUILD
      COMMAND ${CMAKE_COMMAND} -E copy_if_different
      $<TARGET_FILE:darabonba_core::darabonba_core>
      $<TARGET_FILE_DIR:${PROJECT_NAME}Test>
      COMMENT "Copying darabonba_core DLL to test executable directory"
    )
  endif()
endif()

# Register test
add_test(NAME ${PROJECT_NAME}Test COMMAND ${PROJECT_NAME}Test)

# Windows dynamic library: Set PATH environment for CTest to find DLLs
if(WIN32 AND BUILD_SHARED_LIBS)
  set_tests_properties(${PROJECT_NAME}Test PROPERTIES
    ENVIRONMENT "PATH=$<TARGET_FILE_DIR:${PROJECT_NAME}>;$ENV{PATH}"
  )
endif()
