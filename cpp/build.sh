#!/bin/bash

# ============================================================================
# RayTracerStudio - Emscripten Build Script
# ============================================================================

set -e

if [ -n "$EMSDK" ]; then
    echo "Sourcing Emscripten environment from \$EMSDK..."
    source "$EMSDK/emsdk_env.sh"
elif ! command -v emcc &> /dev/null; then
    echo "emcc command not found. Please ensure Emscripten is installed and activated."
    echo "If you have emsdk installed, you might need to run:"
    echo "  export EMSDK="/path/to/your/emsdk""
    echo "  source \$EMSDK/emsdk_env.sh"
    echo "before running this script, or add it to your shell profile."
    exit 1
fi

echo "🔧 Building RayTracerStudio WebAssembly Module..."

# Get the directory where this script is located
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
PROJECT_ROOT="$(dirname "$SCRIPT_DIR")"

# Input and output paths
INPUT_FILE="$SCRIPT_DIR/core.cpp"
OUTPUT_DIR="$PROJECT_ROOT/src/wasm"
OUTPUT_NAME="raytracer"

# Create output directory if it doesn't exist
mkdir -p "$OUTPUT_DIR"

# Compile with Emscripten
emcc "$INPUT_FILE" \
    -O3 \
    -s WASM=1 \
    -s MODULARIZE=1 \
    -s EXPORT_ES6=1 \
    -s ALLOW_MEMORY_GROWTH=1 \
    -s ENVIRONMENT='web' \
    -s EXPORT_NAME='createRaytracerModule' \
    --bind \
    -o "$OUTPUT_DIR/$OUTPUT_NAME.js"

echo "✅ Build complete!"
echo "   Output: $OUTPUT_DIR/$OUTPUT_NAME.js"
echo "   Output: $OUTPUT_DIR/$OUTPUT_NAME.wasm"

