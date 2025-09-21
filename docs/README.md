# Morphosis: 4D Julia Set Fractal Renderer - Documentation

## Overview

Morphosis is a C/C++ application that generates and renders 4D Julia set fractals using OpenGL. The application creates 3D visualizations of 4-dimensional mathematical objects through advanced computational geometry and real-time graphics rendering.

## Documentation Structure

This documentation is organized into the following modules:

- **[System Architecture](./01-system-architecture.md)** - High-level system design and data flow
- **[Mathematical Algorithms](./02-mathematical-algorithms.md)** - Core fractal generation and complex number operations
- **[Graphics Pipeline](./03-graphics-pipeline.md)** - OpenGL rendering and visualization components
- **[Build System](./04-build-system.md)** - Compilation, dependencies, and build configuration
- **[Data Structures](./05-data-structures.md)** - Core data types and memory management
- **[Issues Analysis](./06-issues-analysis.md)** - Known problems and required fixes
- **[Project Plan](./07-project-plan.md)** - 3-phase implementation strategy

## Quick Start

The application supports multiple execution modes:

```bash
# Default mode with interactive parameters
./morphosis -d

# Manual parameter mode
./morphosis <step_size> <q.x> <q.y> <q.z> <q.w>

# Matrix file mode
./morphosis -m <matrix_file.mat>

# Poem file mode  
./morphosis -p <poem_file>
```

## Key Features

- **4D Julia Set Generation**: Advanced mathematical computation of 4-dimensional fractals
- **Marching Cubes Algorithm**: Polygonization of implicit surfaces
- **Real-time OpenGL Rendering**: Interactive 3D visualization with rotation
- **OBJ Export**: Save generated meshes for external use
- **Multiple Input Modes**: Support for various parameter input methods

## Prerequisites

- C compiler (GCC/Clang)
- OpenGL development libraries
- GLFW library  
- GLEW library
- OpenSSL library
- cglm (OpenGL Mathematics library)

## Current Status

⚠️ **ATTENTION**: This is a legacy codebase with known compilation and runtime issues that need to be resolved as part of the hackathon challenge.
