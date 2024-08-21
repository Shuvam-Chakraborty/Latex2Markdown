#!/bin/bash

# Remove old version of the doc folder
rm -rf doc

# Run Doxygen to generate documentation
doxygen Doxyfile

# Check if Firefox is installed
if ! command -v firefox &> /dev/null; then
    echo "Firefox is not installed. Installing Firefox..."

    # Install Firefox based on the package manager (assuming a Debian-based system like Ubuntu)
    sudo apt-get update
    sudo apt-get install -y firefox

    echo "Firefox installed successfully."
else
    echo "Firefox is already installed."
fi

# Open the generated documentation in Firefox
firefox doc/html/index.html

