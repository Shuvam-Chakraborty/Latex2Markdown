cmake_minimum_required(VERSION 3.22)
project(Latex2Markdown)

# Enable testing
enable_testing()

# Find GTest
find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# Set the source files
set(SOURCES
    test_latexmarkdown.cpp
)

# Create the executable for the tests
add_executable(runTests ${SOURCES})

# Link GTest and pthread to the test executable
target_link_libraries(runTests ${GTEST_LIBRARIES} pthread)

# Add a test target to allow running tests with `make test`
add_test(NAME runTests COMMAND runTests)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fprofile-arcs -ftest-coverage")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

# Add link flags for coverage
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -fprofile-arcs -ftest-coverage")

