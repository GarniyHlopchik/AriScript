include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create the test executable
add_executable(unit_tests test_main.cpp)

target_link_libraries(unit_tests 
    GTest::gtest_main 
    ari_lib
)

# This allows you to run 'ctest' from the build folder
include(GoogleTest)
gtest_discover_tests(unit_tests)