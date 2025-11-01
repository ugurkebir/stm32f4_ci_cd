# ==== Test Makefile for STM32F4 Unit Tests ====
# This Makefile builds and runs unit tests for the STM32F4 project

TARGET = test_main
BUILD_DIR = build
TEST_DIR = tests

# ==== Compiler Settings ====
CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -g -O0 -DUNIT_TEST
CFLAGS += -Werror=implicit-function-declaration

# ==== Include Paths ====
INCLUDES = \
  -I$(TEST_DIR) \
  -IInc

# ==== Source Files ====
# Unity framework sources
UNITY_SOURCES = \
  $(TEST_DIR)/unity.c

# Mock sources
MOCK_SOURCES = \
  $(TEST_DIR)/stm32f4xx_hal_mocks.c

# Testable application sources
TESTABLE_SOURCES = \
  $(TEST_DIR)/main_testable.c

# Test sources
TEST_SOURCES = \
  $(TEST_DIR)/test_main.c

# All sources
SOURCES = $(UNITY_SOURCES) $(MOCK_SOURCES) $(TESTABLE_SOURCES) $(TEST_SOURCES)

# ==== Object Files ====
OBJECTS = $(addprefix $(BUILD_DIR)/,$(notdir $(SOURCES:.c=.o)))
vpath %.c $(sort $(dir $(SOURCES)))

# ==== Build Rules ====
all: $(BUILD_DIR)/$(TARGET)

# Build the test executable
$(BUILD_DIR)/$(TARGET): $(OBJECTS) | $(BUILD_DIR)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@
	@echo "Build complete: $@"

# Compile C files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) $< -o $@

# Create build directory
$(BUILD_DIR):
	mkdir -p $@

# ==== Test Execution ====
test: $(BUILD_DIR)/$(TARGET)
	@echo "===================="
	@echo "Running Unit Tests"
	@echo "===================="
	./$(BUILD_DIR)/$(TARGET)
	@echo "===================="

# Run tests with verbose output
test-verbose: $(BUILD_DIR)/$(TARGET)
	@echo "===================="
	@echo "Running Unit Tests (Verbose)"
	@echo "===================="
	./$(BUILD_DIR)/$(TARGET) -v
	@echo "===================="

# Run tests with memory check (if valgrind is available)
test-memcheck: $(BUILD_DIR)/$(TARGET)
	@if command -v valgrind >/dev/null 2>&1; then \
		echo "===================="; \
		echo "Running Memory Check"; \
		echo "===================="; \
		valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(BUILD_DIR)/$(TARGET); \
		echo "===================="; \
	else \
		echo "Valgrind not available, running normal test"; \
		$(MAKE) test; \
	fi

# ==== Coverage Analysis ====
# Build with coverage flags
coverage-build: CFLAGS += --coverage
coverage-build: LDFLAGS += --coverage
coverage-build: clean $(BUILD_DIR)/$(TARGET)

# Run tests and generate coverage report
coverage: coverage-build
	@echo "===================="
	@echo "Running Coverage Analysis"
	@echo "===================="
	./$(BUILD_DIR)/$(TARGET)
	@if command -v gcov >/dev/null 2>&1; then \
		echo "Generating coverage report..."; \
		gcov $(TEST_SOURCES) $(TESTABLE_SOURCES) -o $(BUILD_DIR); \
		echo "Coverage files generated in current directory"; \
	else \
		echo "gcov not available for coverage analysis"; \
	fi
	@echo "===================="

# Generate HTML coverage report (requires lcov)
coverage-html: coverage
	@if command -v lcov >/dev/null 2>&1 && command -v genhtml >/dev/null 2>&1; then \
		echo "Generating HTML coverage report..."; \
		lcov --capture --directory $(BUILD_DIR) --output-file coverage.info; \
		genhtml coverage.info --output-directory coverage-html; \
		echo "HTML coverage report generated in coverage-html/"; \
	else \
		echo "lcov/genhtml not available for HTML coverage report"; \
	fi

# ==== Debugging ====
# Build with debug symbols and run in GDB
debug: $(BUILD_DIR)/$(TARGET)
	gdb ./$(BUILD_DIR)/$(TARGET)

# ==== Static Analysis ====
# Run static analysis (requires cppcheck)
static-analysis:
	@if command -v cppcheck >/dev/null 2>&1; then \
		echo "====================";\
		echo "Running Static Analysis"; \
		echo "===================="; \
		cppcheck --enable=all --std=c99 --platform=unix32 --suppress=missingIncludeSystem $(TEST_SOURCES) $(TESTABLE_SOURCES); \
		echo "===================="; \
	else \
		echo "cppcheck not available for static analysis"; \
	fi

# ==== Continuous Integration ====
# Run all tests suitable for CI
ci: clean test coverage static-analysis
	@echo "===================="
	@echo "CI Tests Complete"
	@echo "===================="

# ==== Clean ====
clean:
	-rm -rf $(BUILD_DIR)
	-rm -f *.gcov *.gcda *.gcno coverage.info
	-rm -rf coverage-html

# Clean everything including generated files
distclean: clean
	-rm -f $(TARGET)

# ==== Information ====
# Show build information
info:
	@echo "===================="
	@echo "Build Information"
	@echo "===================="
	@echo "Target: $(TARGET)"
	@echo "Build Directory: $(BUILD_DIR)"
	@echo "Test Directory: $(TEST_DIR)"
	@echo "Compiler: $(CC)"
	@echo "CFLAGS: $(CFLAGS)"
	@echo "Includes: $(INCLUDES)"
	@echo ""
	@echo "Source Files:"
	@for src in $(SOURCES); do echo "  $$src"; done
	@echo ""
	@echo "Available Targets:"
	@echo "  all          - Build test executable"
	@echo "  test         - Run unit tests"
	@echo "  test-verbose - Run tests with verbose output"
	@echo "  test-memcheck- Run tests with memory check"
	@echo "  coverage     - Run coverage analysis"
	@echo "  coverage-html- Generate HTML coverage report"
	@echo "  debug        - Run tests in debugger"
	@echo "  static-analysis - Run static code analysis"
	@echo "  ci           - Run all CI tests"
	@echo "  clean        - Clean build artifacts"
	@echo "  distclean    - Clean everything"
	@echo "  info         - Show this information"
	@echo "===================="

# ==== Help ====
help: info

# ==== Dependencies ====
# Automatic dependency generation
DEPS = $(OBJECTS:.o=.d)
-include $(DEPS)

$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	$(CC) -c $(CFLAGS) $(INCLUDES) -MMD -MP $< -o $@

# ==== Phony Targets ====
.PHONY: all test test-verbose test-memcheck coverage coverage-build coverage-html debug static-analysis ci clean distclean info help

# Default target
.DEFAULT_GOAL := test