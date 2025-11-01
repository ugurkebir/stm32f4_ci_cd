#!/bin/bash

################################################################################
# STM32F4 Unit Test Runner Script
# 
# This script provides an easy way to run unit tests for the STM32F4 project
# with various options and configurations.
################################################################################

# Colors for output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Script directory
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_DIR="$SCRIPT_DIR"

# Default values
VERBOSE=false
COVERAGE=false
MEMCHECK=false
CLEAN=false
BUILD_ONLY=false
HELP=false

# Function to print colored output
print_status() {
    echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
    echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
    echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
    echo -e "${RED}[ERROR]${NC} $1"
}

# Function to show usage
show_usage() {
    echo "STM32F4 Unit Test Runner"
    echo ""
    echo "Usage: $0 [OPTIONS]"
    echo ""
    echo "Options:"
    echo "  -v, --verbose     Enable verbose test output"
    echo "  -c, --coverage    Run with coverage analysis"
    echo "  -m, --memcheck    Run with memory leak detection (requires valgrind)"
    echo "  -C, --clean       Clean build artifacts before running"
    echo "  -b, --build-only  Build tests but don't run them"
    echo "  -h, --help        Show this help message"
    echo ""
    echo "Examples:"
    echo "  $0                Run basic unit tests"
    echo "  $0 -v             Run tests with verbose output"
    echo "  $0 -c             Run tests with coverage analysis"
    echo "  $0 -m             Run tests with memory checking"
    echo "  $0 -C -c          Clean build and run with coverage"
    echo "  $0 --build-only   Build tests without running"
    echo ""
}

# Parse command line arguments
while [[ $# -gt 0 ]]; do
    case $1 in
        -v|--verbose)
            VERBOSE=true
            shift
            ;;
        -c|--coverage)
            COVERAGE=true
            shift
            ;;
        -m|--memcheck)
            MEMCHECK=true
            shift
            ;;
        -C|--clean)
            CLEAN=true
            shift
            ;;
        -b|--build-only)
            BUILD_ONLY=true
            shift
            ;;
        -h|--help)
            HELP=true
            shift
            ;;
        *)
            print_error "Unknown option: $1"
            show_usage
            exit 1
            ;;
    esac
done

# Show help if requested
if [ "$HELP" = true ]; then
    show_usage
    exit 0
fi

# Change to project directory
cd "$PROJECT_DIR" || {
    print_error "Cannot change to project directory: $PROJECT_DIR"
    exit 1
}

# Print banner
echo "=============================================="
echo "         STM32F4 Unit Test Runner"
echo "=============================================="
echo "Project Directory: $PROJECT_DIR"
echo "Test Configuration:"
echo "  Verbose: $VERBOSE"
echo "  Coverage: $COVERAGE"
echo "  Memory Check: $MEMCHECK"
echo "  Clean Build: $CLEAN"
echo "  Build Only: $BUILD_ONLY"
echo "=============================================="

# Check if test.mk exists
if [ ! -f "test.mk" ]; then
    print_error "test.mk not found in $PROJECT_DIR"
    print_error "Make sure you're running this script from the correct directory"
    exit 1
fi

# Clean if requested
if [ "$CLEAN" = true ]; then
    print_status "Cleaning build artifacts..."
    make -f test.mk clean
    if [ $? -ne 0 ]; then
        print_error "Failed to clean build artifacts"
        exit 1
    fi
    print_success "Build artifacts cleaned"
fi

# Build tests
print_status "Building unit tests..."
make -f test.mk all
if [ $? -ne 0 ]; then
    print_error "Failed to build unit tests"
    exit 1
fi
print_success "Unit tests built successfully"

# Exit if build-only is requested
if [ "$BUILD_ONLY" = true ]; then
    print_success "Build completed. Exiting as requested."
    exit 0
fi

# Determine which test target to run
TEST_TARGET="test"
if [ "$VERBOSE" = true ]; then
    TEST_TARGET="test-verbose"
elif [ "$MEMCHECK" = true ]; then
    # Check if valgrind is available
    if ! command -v valgrind &> /dev/null; then
        print_warning "Valgrind not found. Running normal tests instead."
        TEST_TARGET="test"
    else
        TEST_TARGET="test-memcheck"
    fi
elif [ "$COVERAGE" = true ]; then
    TEST_TARGET="coverage"
fi

# Run tests
print_status "Running unit tests with target: $TEST_TARGET"
make -f test.mk "$TEST_TARGET"
TEST_RESULT=$?

# Check test results
if [ $TEST_RESULT -eq 0 ]; then
    print_success "All unit tests passed!"
    
    # Generate HTML coverage report if coverage was enabled
    if [ "$COVERAGE" = true ]; then
        if command -v lcov &> /dev/null && command -v genhtml &> /dev/null; then
            print_status "Generating HTML coverage report..."
            make -f test.mk coverage-html
            if [ $? -eq 0 ]; then
                print_success "HTML coverage report generated in coverage-html/"
                if command -v open &> /dev/null; then
                    print_status "Opening coverage report in browser..."
                    open coverage-html/index.html
                elif command -v xdg-open &> /dev/null; then
                    print_status "Opening coverage report in browser..."
                    xdg-open coverage-html/index.html
                fi
            fi
        else
            print_warning "lcov/genhtml not available for HTML coverage report"
        fi
    fi
    
else
    print_error "Unit tests failed with exit code: $TEST_RESULT"
    print_error "Check the test output above for details"
fi

# Show summary
echo "=============================================="
echo "           Test Run Summary"
echo "=============================================="
echo "Target: $TEST_TARGET"
echo "Result: $([ $TEST_RESULT -eq 0 ] && echo "PASS" || echo "FAIL")"
echo "Exit Code: $TEST_RESULT"

if [ "$COVERAGE" = true ] && [ -f "coverage.info" ]; then
    echo "Coverage: Report generated"
fi

if [ "$MEMCHECK" = true ] && command -v valgrind &> /dev/null; then
    echo "Memory Check: Completed (check output above)"
fi

echo "=============================================="

exit $TEST_RESULT