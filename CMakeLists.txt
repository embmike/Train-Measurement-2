################################
# Project
################################
cmake_minimum_required(VERSION 3.0.0)
project(filter_app_2 VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 17)


################################
# App
################################

add_executable(${PROJECT_NAME}
    src/main.cpp
    src/device.cpp
    src/functional_iter.cpp
)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)


################################
# GTest
################################
#find_package(GTest REQUIRED)
#include(GoogleTest)

#include(FetchContent)
#FetchContent_Declare(
#  googletest
#  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
#)

# For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)

#enable_testing()


################################
# Unit test
################################
#add_executable(tests
  # Test objects
#  src/functional_iter.cpp
#  src/device.cpp

  # Test cases
#  test/ut_functional_iter.cpp
#  test/ut_device.cpp
 # test/ut_matrix.cpp
#)

#target_link_libraries(tests
#  gtest_main
#)

#include(CTest)
#include(GoogleTest)
#gtest_discover_tests(tests)

#gtest_add_tests(TARGET tests TEST_PREFIX old:)
#add_test(NAME monolithic COMMAND tests)


