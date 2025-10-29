# STM32F4 CI/CD Project

A comprehensive STM32F4 development project with automated build pipeline and semihosting support for embedded development.

## 📋 Project Overview

This project demonstrates best practices for STM32F4 microcontroller development with:
- Modern Makefile-based build system
- Semihosting support for debugging
- HAL (Hardware Abstraction Layer) driver integration
- GPIO and UART peripheral examples
- Ready-to-use CI/CD pipeline structure

## 🎯 Target Hardware

- **Microcontroller**: STM32F407VGT6
- **Core**: ARM Cortex-M4 with FPU
- **Flash**: 1024KB
- **RAM**: 192KB (128KB + 64KB CCM)
- **Development Board**: STM32F4-Discovery or compatible

## 🏗️ Project Structure

```
stm32f4_ci_cd/
├── README.md                           # This file
├── .gitignore                         # Git ignore rules
├── base_app/                          # Main application
│   ├── Makefile                       # Build configuration
│   ├── STM32F407VGTX_FLASH.ld        # Linker script
│   ├── startup_stm32f407xx.s         # Startup assembly code
│   ├── Inc/                          # Header files
│   │   ├── main.h                    # Main application header
│   │   ├── stm32f4xx_hal_conf.h      # HAL configuration
│   │   └── stm32f4xx_it.h           # Interrupt handlers header
│   ├── Src/                          # Source files
│   │   ├── main.c                    # Main application
│   │   ├── stm32f4xx_hal_msp.c       # HAL MSP implementation
│   │   ├── stm32f4xx_it.c           # Interrupt handlers
│   │   └── system_stm32f4xx.c        # System configuration
│   ├── Drivers/                      # STM32 HAL drivers
│   │   ├── CMSIS/                    # CMSIS headers
│   │   ├── STM32F4xx_HAL_Driver/     # HAL driver sources
│   │   └── BSP/                      # Board support package
│   └── build/                        # Build output directory
└── semihosting/                      # Semihosting example
    ├── Makefile                      # Semihosting-specific build
    └── ...                          # Semihosting source files
```

## 🚀 Features

### Hardware Features Demonstrated
- **GPIO Control**: LED blinking on PD12-PD15 (Discovery board LEDs)
- **UART Communication**: UART4 configured at 115200 baud
- **Timer Usage**: TIM6 for timing operations
- **System Clock**: 168MHz using HSI + PLL

### Software Features
- **HAL Integration**: Full STM32 HAL driver support
- **Semihosting**: Printf debugging through debugger
- **Modular Makefile**: Clean, maintainable build system
- **Error Handling**: Proper error handling and recovery
- **Interrupt Support**: Complete interrupt handling framework

## 🛠️ Prerequisites

### Required Tools
1. **ARM GCC Toolchain**
   ```bash
   # macOS (Homebrew)
   brew install --cask gcc-arm-embedded
   
   # Ubuntu/Debian
   sudo apt-get install gcc-arm-none-eabi
   ```

2. **OpenOCD** (for flashing and debugging)
   ```bash
   # macOS
   brew install openocd
   
   # Ubuntu/Debian
   sudo apt-get install openocd
   ```

3. **ST-LINK Tools** (optional, alternative flashing)
   ```bash
   # macOS
   brew install stlink
   ```

4. **Make** (usually pre-installed)
   ```bash
   make --version
   ```

### Hardware Requirements
- STM32F4-Discovery board or compatible
- ST-LINK/V2 programmer (integrated on Discovery board)
- USB cable for programming and power

## 🔧 Building the Project

### Quick Start
```bash
# Clone the repository
git clone <repository-url>
cd stm32f4_ci_cd/base_app

# Build the project
make

# Clean build artifacts
make clean

# Build with release optimization
make DEBUG=0
```

### Build Options
```bash
# Debug build (default)
make DEBUG=1

# Release build
make DEBUG=0

# Enable semihosting
make SEMIHOST=1

# Custom GCC path
make GCC_PATH=/path/to/arm-gcc

# Show memory usage
make size

# Detailed memory analysis
make size-detailed
```

## 📡 Flashing and Programming

### Using OpenOCD (Recommended)
```bash
# Flash the firmware
make flash

# Start debugging session
make debug
```

### Using ST-LINK Utility
```bash
# Flash using ST-LINK
make flash-stlink
```

### Manual OpenOCD Commands
```bash
# Start OpenOCD server
openocd -f interface/stlink.cfg -f target/stm32f4x.cfg

# In another terminal, connect with GDB
arm-none-eabi-gdb build/stm32f4_base_app.elf
(gdb) target remote localhost:3333
(gdb) monitor reset halt
(gdb) load
(gdb) continue
```

## 🐛 Debugging

### Semihosting Support
The project includes semihosting support for printf debugging:

```c
#include <stdio.h>

int main(void) {
    // ... initialization code ...
    
    printf("Hello from STM32F4!\n");
    printf("System Clock: %lu Hz\n", SystemCoreClock);
    
    // ... rest of application ...
}
```

Enable semihosting in your build:
```bash
make SEMIHOST=1 flash debug
```

### GDB Debugging
```bash
# Start debugging session
make debug

# Common GDB commands
(gdb) break main          # Set breakpoint at main
(gdb) continue           # Continue execution
(gdb) step              # Step into function
(gdb) next              # Step over function
(gdb) print variable    # Print variable value
(gdb) backtrace         # Show call stack
```

## 🔍 Application Details

### Main Application Flow
1. **System Initialization**: Clock configuration, HAL initialization
2. **Peripheral Setup**: GPIO, UART, Timer configuration
3. **Main Loop**: LED blinking with 500ms delay

### Clock Configuration
- **Source**: HSI (16MHz internal oscillator)
- **PLL**: 168MHz system clock
- **AHB**: 168MHz
- **APB1**: 42MHz
- **APB2**: 84MHz

### GPIO Configuration
- **PD12-PD15**: Output pins for LEDs (Discovery board)
- **PA0**: UART4 TX (if needed)
- **PA1**: UART4 RX (if needed)

## 📊 Memory Usage

Typical memory usage for the base application:
```
   text    data     bss     dec     hex filename
  12345     123    4567   17035    428b build/stm32f4_base_app.elf
```

- **text**: Program code (Flash)
- **data**: Initialized variables (Flash → RAM)
- **bss**: Uninitialized variables (RAM)

## 🚀 CI/CD Integration

### GitHub Actions (Example)
```yaml
name: STM32F4 Build
on: [push, pull_request]
jobs:
  build:
    runs-on: ubuntu-latest
    steps:
    - uses: actions/checkout@v3
    - name: Install ARM GCC
      run: sudo apt-get install gcc-arm-none-eabi
    - name: Build Project
      run: |
        cd base_app
        make
        make size
```

### Makefile Targets for CI
```bash
make all           # Build everything
make clean         # Clean build artifacts
make size          # Show memory usage
make test          # Run unit tests (if implemented)
```

## 🔧 Customization

### Adding New Source Files
1. Place `.c` files in `Src/` directory
2. Place `.h` files in `Inc/` directory
3. Makefile automatically detects new files

### Adding HAL Modules
1. Enable modules in `Inc/stm32f4xx_hal_conf.h`
2. HAL sources are automatically included

### Modifying Build Settings
Edit `Makefile` variables:
```makefile
# Change optimization level
OPT = -O3

# Add custom defines
C_DEFS += -DCUSTOM_DEFINE

# Add include paths
C_INCLUDES += -Icustom/include/path
```

## 📚 Resources

### Documentation
- [STM32F407xx Reference Manual](https://www.st.com/resource/en/reference_manual/dm00031020.pdf)
- [STM32F4xx HAL Driver Documentation](https://www.st.com/resource/en/user_manual/dm00105879.pdf)
- [ARM Cortex-M4 Technical Reference](https://developer.arm.com/documentation/100166/0001)

### Development Tools
- [STM32CubeMX](https://www.st.com/en/development-tools/stm32cubemx.html) - Configuration tool
- [STM32CubeIDE](https://www.st.com/en/development-tools/stm32cubeide.html) - Integrated development environment
- [OpenOCD](http://openocd.org/) - On-chip debugger

## 🤝 Contributing

1. Fork the repository
2. Create a feature branch: `git checkout -b feature/amazing-feature`
3. Commit changes: `git commit -m 'Add amazing feature'`
4. Push to branch: `git push origin feature/amazing-feature`
5. Open a Pull Request

## 📄 License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## 🆘 Troubleshooting

### Common Issues

**Build fails with "command not found"**
```bash
# Check if ARM GCC is in PATH
arm-none-eabi-gcc --version

# If not found, install or add to PATH
export PATH=/path/to/gcc-arm-none-eabi/bin:$PATH
```

**Flash fails with "No ST-LINK detected"**
```bash
# Check ST-LINK connection
lsusb | grep STMicro

# Try different USB port or cable
# Update ST-LINK firmware using STM32CubeProg
```

**Semihosting not working**
```bash
# Make sure you're running under debugger
make SEMIHOST=1 debug

# In GDB, enable semihosting
(gdb) monitor arm semihosting enable
```

**Memory usage too high**
```bash
# Build with release optimization
make DEBUG=0

# Check memory usage
make size-detailed
```

### Getting Help
- Check the [Issues](../../issues) page for known problems
- Create a new issue with detailed description and build output
- Include system information: OS, toolchain versions, hardware

---

**Happy coding with STM32F4!** 🎉
