---
sidebar_position: 2
---

# Getting Started

This guide will help you set up RayTracer Studio for development.

## Prerequisites

Before you begin, ensure you have the following installed:

- **Node.js** (v18 or later)
- **npm** (comes with Node.js)
- **Emscripten SDK** - For compiling C++ to WebAssembly

### Installing Emscripten

```bash
# Clone the emsdk repository
git clone https://github.com/emscripten-core/emsdk.git

# Enter the directory
cd emsdk

# Download and install the latest SDK
./emsdk install latest

# Activate the SDK
./emsdk activate latest

# Set up environment variables
source ./emsdk_env.sh
```

:::tip
Add `source /path/to/emsdk/emsdk_env.sh` to your shell profile (`.bashrc`, `.zshrc`) to automatically activate Emscripten.
:::

## Installation

### 1. Clone the Repository

```bash
git clone https://github.com/srs-sudeep/rayTracing.git
cd rayTracing
```

### 2. Install Dependencies

```bash
npm install
```

### 3. Build the WebAssembly Module

```bash
npm run build:wasm
```

This script:
1. Compiles `cpp/core.cpp` with Emscripten
2. Outputs `raytracer.js` and `raytracer.wasm` to `src/wasm/`
3. Uses optimization flags for best performance

### 4. Start Development Server

```bash
npm run dev
```

Open [http://localhost:5173](http://localhost:5173) in your browser.

## Project Structure

```
rayTracing/
├── cpp/                      # C++ source code
│   ├── include/              # Header files
│   │   ├── Vec3.h            # 3D vector math
│   │   ├── Ray.h             # Ray structure
│   │   ├── Material.h        # Material properties
│   │   ├── Light.h           # Light structure
│   │   ├── Camera.h          # Camera with orbit controls
│   │   ├── Sphere.h          # Sphere intersection
│   │   ├── Plane.h           # Ground plane with grid
│   │   ├── Scene.h           # Scene management
│   │   └── Renderer.h        # Main render loop
│   ├── core.cpp              # Emscripten bindings
│   └── build.sh              # Build script
├── src/                      # React source code
│   ├── components/           # React components
│   │   ├── canvas/           # Canvas and toolbar
│   │   ├── controls/         # Control panels
│   │   ├── layout/           # Header and modals
│   │   └── ui/               # Reusable UI elements
│   ├── hooks/                # Custom React hooks
│   ├── wasm/                 # Compiled WASM (generated)
│   ├── App.jsx               # Main application
│   └── main.jsx              # Entry point
├── public/                   # Static assets
├── docs/                     # This documentation
├── package.json              # Node.js dependencies
└── vite.config.js            # Vite configuration
```

## Available Scripts

| Command | Description |
|---------|-------------|
| `npm run dev` | Start development server |
| `npm run build` | Build for production |
| `npm run build:wasm` | Compile C++ to WebAssembly |
| `npm run preview` | Preview production build |

## Troubleshooting

### WASM Module Failed to Load

If you see "Failed to Load Module" error:

1. Make sure you've built the WASM module:
   ```bash
   npm run build:wasm
   ```

2. Check that `src/wasm/raytracer.js` and `src/wasm/raytracer.wasm` exist.

### Emscripten Not Found

If `emcc` command is not found:

```bash
# Set the EMSDK path
export EMSDK="/path/to/your/emsdk"

# Activate environment
source $EMSDK/emsdk_env.sh
```

### Slow Performance

- Reduce resolution in the View tab (256 or 512)
- Lower the Max Bounces setting
- Use fewer spheres (Single Sphere scene)

