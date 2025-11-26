# RayTracerStudio

A real-time CPU-based ray tracer running in the browser via WebAssembly. Built with C++, Emscripten, and React.

![RayTracerStudio](https://via.placeholder.com/800x400/0a0a0c/e85d4c?text=RayTracerStudio)

## Features

- **CPU Ray Tracing** - Pure C++ implementation compiled to WebAssembly
- **Real-time Interactivity** - Adjust light position and see immediate results
- **Diffuse Shading** - Lambertian lighting model
- **Modern UI** - Dark studio aesthetic with sleek controls

## Prerequisites

- **Node.js** (v18+)
- **Emscripten SDK** - [Installation Guide](https://emscripten.org/docs/getting_started/downloads.html)

## Quick Start

### 1. Install Dependencies

```bash
npm install
```

### 2. Build the WebAssembly Module

Make sure Emscripten is installed and `emcc` is in your PATH:

```bash
# Activate Emscripten (if not already in PATH)
source /path/to/emsdk/emsdk_env.sh

# Build the WASM module
npm run build:wasm
```

### 3. Start Development Server

```bash
npm run dev
```

Open [http://localhost:5173](http://localhost:5173) in your browser.

## Project Structure

```
RayTracer/
├── cpp/
│   ├── core.cpp          # Ray tracing engine
│   └── build.sh          # Emscripten build script
├── src/
│   ├── components/
│   │   ├── RaytracerCanvas.jsx
│   │   └── Controls.jsx
│   ├── wasm/
│   │   ├── raytracer.js      # Generated: WASM loader
│   │   └── raytracer.wasm    # Generated: WebAssembly binary
│   ├── App.jsx
│   ├── App.css
│   └── main.jsx
├── index.html
├── package.json
└── vite.config.js
```

## How It Works

### C++ Core

The ray tracer implements:
- Custom `Vec3` struct with vector math operations
- Ray-sphere intersection using the quadratic formula
- Lambertian (diffuse) shading

### WebAssembly Bridge

The C++ code is compiled to WebAssembly using Emscripten's `embind`:
- `render(width, height)` - Returns RGBA pixel buffer
- `updateLight(x, y, z)` - Updates global light position

### React Frontend

- Loads WASM module asynchronously
- Renders to Canvas using `putImageData`
- Interactive sliders for light control

## Scripts

| Command | Description |
|---------|-------------|
| `npm run dev` | Start Vite dev server |
| `npm run build` | Build for production |
| `npm run build:wasm` | Compile C++ to WASM |
| `npm run preview` | Preview production build |

## Tech Stack

- **C++** - Core ray tracing logic
- **Emscripten** - C++ to WebAssembly compiler
- **React 18** - UI framework
- **Vite** - Build tool with excellent WASM support

## License

MIT

