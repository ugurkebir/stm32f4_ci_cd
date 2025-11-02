# STM32F4 Unit Tests

This directory contains comprehensive unit tests for the STM32F4 base application using the Unity test framework.

## 📋 Overview

The unit test suite provides:
- **Mock HAL Implementation**: Complete STM32F4xx HAL function mocks
- **Unity Test Framework**: Lightweight C testing framework
- **Comprehensive Coverage**: Tests for all major application components
- **CI/CD Integration**: Automated testing support

## 🏗️ Test Structure

```
tests/
├── unity.h                    # Unity test framework header
├── unity.c                    # Unity test framework implementation
├── stm32f4xx_hal_mocks.h      # HAL function mocks header
├── stm32f4xx_hal_mocks.c      # HAL function mocks implementation
├── main_testable.h            # Testable main application header
├── main_testable.c            # Testable main application functions
├── test_main.c                # Unit tests for main.c
└── README.md                  # This file
```

## 🎯 Test Categories

### 1. System Initialization Tests
- HAL initialization (success/failure scenarios)
- System clock configuration
- Complete initialization sequence
- Error handling during initialization

### 2. GPIO Tests
- GPIO initialization
- Pin toggle functionality  
- Pin write/read operations
- Multiple GPIO operations

### 3. UART Tests
- UART4 initialization
- Configuration parameter validation
- Baud rate settings
- Error handling

### 4. Timer Tests
- TIM6 initialization
- Timer configuration parameters
- Master configuration setup

### 5. Main Loop Tests
- Single iteration testing
- Multiple iteration testing
- Delay functionality validation

### 6. Integration Tests
- Complete system operation
- Peripheral failure scenarios
- End-to-end functionality

## 🚀 Running Tests

### Prerequisites
```bash
# Install GCC (if not already installed)
# macOS
xcode-select --install

# Ubuntu/Debian
sudo apt-get install build-essential

# Optional: Install testing tools
sudo apt-get install valgrind lcov cppcheck
```

### Basic Test Execution
```bash
# Run all tests
make -f test.mk test

# Clean and run tests
make -f test.mk clean test

# Show available commands
make -f test.mk help
```

### Advanced Testing Options
```bash
# Run tests with verbose output
make -f test.mk test-verbose

# Run tests with memory leak detection
make -f test.mk test-memcheck

# Run with coverage analysis
make -f test.mk coverage

# Generate HTML coverage report
make -f test.mk coverage-html

# Run static code analysis
make -f test.mk static-analysis

# Run complete CI test suite
make -f test.mk ci
```

## 🧪 Test Examples

### Example Test Output
```
test_main.c:55:test_hal_init_success:PASS
test_main.c:72:test_hal_init_failure:PASS
test_main.c:85:test_system_clock_config:PASS
test_main.c:108:test_main_init_sequence_success:PASS
test_main.c:129:test_main_init_hal_failure:PASS

-----------------------
25 Tests 0 Failures 0 Ignored
OK
```

### Example Coverage Report
```
File 'main_testable.c'
Lines executed:95.24% of 42
Branches executed:87.50% of 8
Taken at least once:75.00% of 8
Calls executed:100.00% of 15
```

## 🔍 Mock System

### HAL Function Mocks
The mock system provides controlled testing environment:

```c
// Reset all mocks before each test
mock_reset();

// Configure mock behavior
mock_set_hal_init_return(HAL_ERROR);
mock_set_uart_init_return(HAL_OK);

// Execute test code
HAL_StatusTypeDef result = HAL_Init();

// Verify mock interactions
TEST_ASSERT_EQUAL(HAL_ERROR, result);
TEST_ASSERT_TRUE(mock_control.hal_init_called);
```

### Available Mock Controls
- **Return Value Control**: Set success/failure for HAL functions
- **Call Tracking**: Monitor function call counts and parameters
- **State Simulation**: Simulate GPIO pin states and register values
- **Parameter Validation**: Verify correct parameters passed to HAL functions

## 📊 Test Metrics

### Test Coverage Goals
- **Line Coverage**: > 90%
- **Branch Coverage**: > 85%
- **Function Coverage**: 100%

### Performance Targets
- **Test Execution Time**: < 1 second for full suite
- **Memory Usage**: < 10MB during test execution
- **No Memory Leaks**: Zero leaks detected by Valgrind

## 🔧 Adding New Tests

### 1. Test Function Template
```c
void test_new_functionality(void)
{
    /* Arrange */
    mock_reset();
    // Set up test conditions
    
    /* Act */
    // Execute the code under test
    
    /* Assert */
    TEST_ASSERT_EQUAL(expected, actual);
    TEST_ASSERT_TRUE(condition);
}
```

### 2. Add to Test Runner
```c
int main(void)
{
    UNITY_BEGIN();
    
    // Existing tests...
    RUN_TEST(test_new_functionality);
    
    return UNITY_END();
}
```

### 3. Update Mock System (if needed)
```c
// In stm32f4xx_hal_mocks.h
extern bool new_function_called;

// In stm32f4xx_hal_mocks.c
HAL_StatusTypeDef HAL_NewFunction(void) {
    mock_control.new_function_called = true;
    return mock_control.new_function_return_value;
}
```

## 🚀 Continuous Integration

### GitHub Actions Integration
```yaml
name: Unit Tests
on: [push, pull_request]
jobs:
  test:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: Install Dependencies
      run: sudo apt-get install build-essential lcov
    - name: Run Tests
      run: |
        cd base_app
        make -f test.mk ci
    - name: Upload Coverage
      uses: codecov/codecov-action@v3
      with:
        file: ./coverage.info
```

### Test Automation
The test suite supports:
- **Automated Test Discovery**: Tests are automatically detected
- **Parallel Execution**: Tests can run independently
- **Result Reporting**: JUnit XML output for CI systems
- **Coverage Tracking**: Automatic coverage report generation

## 📚 Testing Best Practices

### 1. Test Organization
- **One Concept per Test**: Each test should verify one specific behavior
- **Descriptive Names**: Test names should clearly describe what is being tested
- **Arrange-Act-Assert**: Follow the AAA pattern consistently

### 2. Mock Usage
- **Reset State**: Always reset mocks before each test
- **Verify Interactions**: Check that expected function calls occurred
- **Isolate Units**: Use mocks to isolate the code under test

### 3. Test Data
- **Boundary Values**: Test edge cases and boundary conditions
- **Invalid Inputs**: Verify error handling with invalid data
- **Typical Values**: Test normal operation scenarios

## 🐛 Debugging Tests

### Common Issues and Solutions

**Test Fails Unexpectedly**
```bash
# Run single test with debugger
make -f test.mk debug
(gdb) break test_function_name
(gdb) run
```

**Memory Leaks Detected**
```bash
# Run with detailed memory checking
make -f test.mk test-memcheck
# Check the valgrind output for leak details
```

**Compilation Errors**
```bash
# Check compiler flags and includes
make -f test.mk info
# Verify all source files are included
```

**Mock Not Working**
```c
// Ensure mock is reset before test
void setUp(void) {
    mock_reset();
}

// Verify mock function is called
TEST_ASSERT_TRUE(mock_control.function_called);
```

## 📈 Future Enhancements

### Planned Improvements
1. **Parameterized Tests**: Support for data-driven testing
2. **Test Fixtures**: Reusable test setup and teardown
3. **Performance Tests**: Timing and performance validation
4. **Integration Tests**: Hardware-in-the-loop testing support

### Contributing
1. **Add Tests**: Write tests for new functionality
2. **Improve Coverage**: Increase test coverage for existing code
3. **Enhance Mocks**: Add more sophisticated mock behaviors
4. **Documentation**: Improve test documentation and examples

---

For more information about the Unity testing framework, visit: https://github.com/ThrowTheSwitch/Unity