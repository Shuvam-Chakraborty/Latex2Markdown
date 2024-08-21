#!/bin/bash

# Exit on error
set -e

# Check if required packages are installed
PACKAGES=("libgtest-dev" "cmake" "lcov")
for PACKAGE in "${PACKAGES[@]}"; do
  if ! dpkg -s "$PACKAGE" &> /dev/null; then
    echo "Error: $PACKAGE is not installed. Installing..."
    sudo apt-get install "$PACKAGE"
  fi
done

# Directory for build and coverage reports
BUILD_DIR="build"
COVERAGE_DIR="coverage"

# Create and navigate to the build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Run CMake to generate the build files
cmake ..

# Build the project
make

# Run the tests
./runTests

# Generate coverage report
# Cleanup old coverage files if any
rm -f coverage.info

# Capture coverage data
lcov --capture --directory . --output-file coverage.info

# Generate HTML report from coverage data
mkdir -p ../$COVERAGE_DIR
genhtml coverage.info --output-directory ../$COVERAGE_DIR

# Inform the user
echo "Coverage report generated in $COVERAGE_DIR directory."

# Return to the original directory
cd ..
