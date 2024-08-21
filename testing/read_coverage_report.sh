#!/bin/bash

# Directory where the coverage report is stored
COVERAGE_DIR="coverage"

# Check if the coverage directory exists
if [ ! -d "$COVERAGE_DIR" ]; then
  echo "Coverage directory not found: $COVERAGE_DIR"
  exit 1
fi

# Path to the index.html file in the coverage directory
REPORT_FILE="$COVERAGE_DIR/index.html"

# Check if the index.html file exists
if [ ! -f "$REPORT_FILE" ]; then
  echo "Coverage report not found: $REPORT_FILE"
  exit 1
fi

# Check if xdg-open (or respective packages) is installed
if command -v xdg-open &> /dev/null; then
  BROWSER_CMD="xdg-open"
elif command -v open &> /dev/null; then
  BROWSER_CMD="open"
elif command -v start &> /dev/null; then
  BROWSER_CMD="start"
else
  echo "Error: No default browser opener found. Please install xdg-open (Linux), open (macOS), or start (Windows)."
  exit 1
fi

# Open the coverage report in the default web browser
echo "Opening coverage report: $REPORT_FILE"
$BROWSER_CMD "$REPORT_FILE"

# Alternatively, you can use the following to open in a specific browser
# firefox "$REPORT_FILE"
# google-chrome "$REPORT_FILE"
