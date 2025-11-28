---
sidebar_position: 1
slug: /
---

# Introduction

Welcome to **RayTracer Studio** - a real-time CPU ray tracer that runs entirely in your browser!

## What is RayTracer Studio?

RayTracer Studio is an educational and interactive ray tracing application that demonstrates fundamental computer graphics concepts. The rendering engine is written in **C++** and compiled to **WebAssembly**, allowing near-native performance in any modern web browser.

## Key Features

- 🎨 **Real-time Rendering** - Interactive ray tracing with immediate visual feedback
- 🪞 **Recursive Reflections** - Accurate mirror reflections with Fresnel effect
- 💡 **Blinn-Phong Shading** - Realistic specular highlights
- 🌑 **Shadow Rays** - Hard shadows from point lights
- 📷 **Interactive Camera** - Orbit, zoom, and adjust field of view
- 🎭 **Scene Presets** - Multiple sphere arrangements to explore
- 📐 **3D Ground Plane** - Blender-style infinite grid

## Technology Stack

| Layer | Technology | Purpose |
|-------|------------|---------|
| **Core Engine** | C++ | Ray tracing mathematics and rendering |
| **Compilation** | Emscripten | C++ to WebAssembly compiler |
| **Runtime** | WebAssembly | Near-native browser performance |
| **UI Framework** | React | Interactive user interface |
| **Build Tool** | Vite | Fast development and bundling |

## How It Works

```
┌─────────────────────────────────────────────────────────────┐
│                         Browser                              │
│  ┌─────────────────┐         ┌─────────────────────────┐    │
│  │   React UI      │ ──────► │   WebAssembly Module    │    │
│  │  (Controls)     │ state   │   (C++ Ray Tracer)      │    │
│  └─────────────────┘         └───────────┬─────────────┘    │
│          ▲                               │                   │
│          │                               │ pixel buffer      │
│          │         ┌─────────────────────▼─────┐            │
│          └─────────│      HTML Canvas          │            │
│                    │   (Rendered Image)        │            │
│                    └───────────────────────────┘            │
└─────────────────────────────────────────────────────────────┘
```

## Quick Links

- [Getting Started](./getting-started) - Set up and run the project
- [Architecture](./architecture) - Understand the system design
- [C++ Engine](./cpp-engine/overview) - Deep dive into the ray tracer
- [React Frontend](./react-frontend/overview) - Explore the UI components
