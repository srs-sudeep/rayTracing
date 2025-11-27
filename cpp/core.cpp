/**
 * RayTracerStudio - Core WebAssembly Module
 * 
 * This is the main entry point that exposes the ray tracing
 * functionality to JavaScript via Emscripten bindings.
 */

#include <emscripten/bind.h>
#include <vector>
#include <cstdint>

#include "include/Vec3.h"
#include "include/Ray.h"
#include "include/Material.h"
#include "include/Light.h"
#include "include/Camera.h"
#include "include/Sphere.h"
#include "include/Scene.h"
#include "include/Renderer.h"

// ============================================================================
// Global Instances
// ============================================================================

Scene globalScene;
Renderer globalRenderer;

// ============================================================================
// API Functions (Called from JavaScript)
// ============================================================================

std::vector<uint8_t> render(int width, int height) {
    globalRenderer.width = width;
    globalRenderer.height = height;
    return globalRenderer.render(globalScene);
}

void updateLight(float x, float y, float z) {
    globalScene.updateLight(x, y, z);
}

void updateMaterial(float specular, float shininess) {
    globalScene.updateMainSphere(specular, shininess);
}

void updateCamera(float posX, float posY, float posZ) {
    globalScene.updateCamera(posX, posY, posZ);
}

void orbitCamera(float deltaX, float deltaY) {
    globalScene.orbitCamera(deltaX, deltaY);
}

void zoomCamera(float delta) {
    globalScene.zoomCamera(delta);
}

void setShowGrid(bool show) {
    globalRenderer.showGrid = show;
}

void setGridSize(int size) {
    globalRenderer.gridSize = size;
}

// Get camera position for UI sync
float getCameraX() { return globalScene.camera.position.x; }
float getCameraY() { return globalScene.camera.position.y; }
float getCameraZ() { return globalScene.camera.position.z; }

// ============================================================================
// Emscripten Bindings
// ============================================================================

EMSCRIPTEN_BINDINGS(raytracer_module) {
    // Core render function
    emscripten::function("render", &render);
    
    // Light controls
    emscripten::function("updateLight", &updateLight);
    
    // Material controls
    emscripten::function("updateMaterial", &updateMaterial);
    
    // Camera controls
    emscripten::function("updateCamera", &updateCamera);
    emscripten::function("orbitCamera", &orbitCamera);
    emscripten::function("zoomCamera", &zoomCamera);
    emscripten::function("getCameraX", &getCameraX);
    emscripten::function("getCameraY", &getCameraY);
    emscripten::function("getCameraZ", &getCameraZ);
    
    // Grid controls
    emscripten::function("setShowGrid", &setShowGrid);
    emscripten::function("setGridSize", &setGridSize);
    
    // Vector type for return values
    emscripten::register_vector<uint8_t>("VectorUint8");
}
