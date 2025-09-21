# Build System Analysis

## Build Configuration Overview

The project provides two build systems with different configurations:

### Makefile (Primary)
- **Compiler**: clang
- **Standard**: C99 (implicit)
- **Optimization**: `-O3` (aggressive optimization)
- **Target**: macOS-specific with Frameworks

### CMakeLists.txt (Alternative)  
- **Compiler**: Default (GCC/Clang)
- **Standard**: C99 (explicit)
- **Optimization**: `-O2` (moderate optimization)
- **Target**: Cross-platform

## Dependencies Analysis

### Required Libraries

#### Graphics Libraries
```makefile
GL_LIBS = -framework OpenGL -lGLEW -lglfw -I/usr/local/include
```
- **OpenGL**: Core graphics framework (macOS framework)
- **GLEW**: OpenGL Extension Wrangler Library
- **GLFW**: Window and input management

#### System Libraries
```makefile  
OPENSSL_LIB = -lssl -lcrypto -L/usr/local/opt/openssl@1.1/lib
```
- **OpenSSL**: Used for SHA256 hashing in matrix processing
- **Math Library**: Implicit linking for mathematical functions

#### Custom Libraries
```makefile
./libft/libft.a
```
- **libft**: Custom utility library (currently missing)

### Include Paths
```makefile
FLAGS = -O3 -Wall -I$(INC_DIR) -I$(LIB_INC_DIR)
```
- `./includes/` - Project headers
- `./libft/` - Custom library headers
- `/usr/local/include` - System libraries

## Critical Build Issues Identified

### 1. Missing libft.a Library
**Problem**: Makefile references `./libft/libft.a` but file doesn't exist
**Impact**: Linking will fail
**Files Affected**: Only header files present in `libft/`

### 2. Shader File Path Mismatch
**Problem**: Code expects `vertex_old.shader` and `fragment_old.shader`
**Reality**: Files are named `vertex.shader` and `fragment.shader`
**Location**: `includes/gl_includes.h` lines 6-7
```c
#define VERTEX_SRC "./shaders/vertex_old.shader"    // ❌ Wrong
#define FRAGMENT_SRC "./shaders/fragment_old.shader" // ❌ Wrong
```

### 3. Missing OpenCL Dependencies
**Problem**: `lib_complex.h` includes OpenCL headers
**Issue**: `opencl-c-base.h` may not be available on all systems
**Impact**: Compilation failure on systems without OpenCL

### 4. Type Definition Dependencies
**Problem**: Uses OpenCL vector types (`float2`, `float3`, `float4`, `uint2`)
**Issue**: These types may not be defined without proper OpenCL headers
**Impact**: Compilation errors for undefined types

### 5. Platform-Specific Code
**Problem**: Makefile uses macOS-specific frameworks
**Issue**: Won't compile on Linux/Windows without modification
**Solution**: CMakeLists.txt provides cross-platform alternative

## Compilation Process

### Makefile Build Steps
1. **Create obj/ directory**
2. **Compile each .c file** to .o object file
3. **Link all objects** with libraries to create executable

### Expected Compilation Command
```bash
clang -O3 -Wall -I./includes/ -I./libft/ -c srcs/*.c
clang *.o ./libft/libft.a -o morphosis -framework OpenGL -lGLEW -lglfw -lssl -lcrypto
```

## Dependency Installation (macOS)

### Using Homebrew
```bash
# Graphics libraries
brew install glfw glew

# OpenSSL (if needed)
brew install openssl@1.1

# Math library (usually included with system)
# OpenGL framework (included with macOS)
```

### Using MacPorts
```bash
port install glfw glew
```

## Cross-Platform Considerations

### Linux Dependencies
```bash
# Ubuntu/Debian
sudo apt-get install libglfw3-dev libglew-dev libssl-dev

# Fedora/CentOS  
sudo yum install glfw-devel glew-devel openssl-devel
```

### Windows Dependencies
- **GLFW**: Download from official website
- **GLEW**: Download from official website  
- **OpenSSL**: Use vcpkg or compile from source
- **Build Tools**: Visual Studio or MinGW

## Build System Fixes Required

### Priority 1: Critical Issues
1. **Fix shader file paths** in `gl_includes.h`
2. **Create or fix libft.a** library
3. **Resolve OpenCL type dependencies**

### Priority 2: Compatibility Issues  
1. **Add missing math.h includes** where needed
2. **Fix bzero/ft_bzero** function calls
3. **Add proper error checking** for library initialization

### Priority 3: Enhancement
1. **Add debug build configuration**
2. **Improve cross-platform support**
3. **Add dependency checking** in build scripts

## Recommended Build Strategy

### Phase 1: Fix Critical Issues
1. Update shader paths
2. Build libft.a or remove dependency
3. Fix type definitions

### Phase 2: Platform Compatibility
1. Test on target platform
2. Adjust library paths as needed
3. Add missing includes

### Phase 3: Build System Enhancement
1. Add debug/release configurations
2. Improve error messages
3. Add dependency validation
