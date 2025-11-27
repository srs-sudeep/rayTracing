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
#include "include/Plane.h"
#include "include/Scene.h"
#include "include/Renderer.h"

// ============================================================================
// Global Instances
// ============================================================================

Scene globalScene;
Renderer globalRenderer;

// ============================================================================
// Render API
// ============================================================================

std::vector<uint8_t> render(int width, int height) {
    globalRenderer.width = width;
    globalRenderer.height = height;
    return globalRenderer.render(globalScene);
}

// ============================================================================
// Scene API
// ============================================================================

void loadScenePreset(int preset) {
    globalScene.loadPreset(static_cast<ScenePreset>(preset));
}

int getSphereCount() {
    return globalScene.getSphereCount();
}

// ============================================================================
// Light API
// ============================================================================

void updateLight(float x, float y, float z) {
    globalScene.updateLight(x, y, z);
}

// ============================================================================
// Material API
// ============================================================================

void updateMaterial(float specular, float shininess, float reflectivity) {
    globalScene.updateMainSphere(specular, shininess, reflectivity);
}

void updateSphereColor(float r, float g, float b) {
    globalScene.updateSphereColor(r, g, b);
}

void updateGroundReflectivity(float reflectivity) {
    globalScene.updateGroundReflectivity(reflectivity);
}

// ============================================================================
// Camera API
// ============================================================================

void updateCamera(float posX, float posY, float posZ) {
    globalScene.updateCamera(posX, posY, posZ);
}

void orbitCamera(float deltaX, float deltaY) {
    globalScene.orbitCamera(deltaX, deltaY);
}

void zoomCamera(float delta) {
    globalScene.zoomCamera(delta);
}

float getCameraX() { return globalScene.camera.position.x; }
float getCameraY() { return globalScene.camera.position.y; }
float getCameraZ() { return globalScene.camera.position.z; }

// ============================================================================
// View API
// ============================================================================

void setShowGrid(bool show) {
    globalScene.setShowGrid(show);
}

void setGridScale(float scale) {
    globalScene.setGridScale(scale);
}

void setShowGroundPlane(bool show) {
    globalScene.setShowGroundPlane(show);
}

void setMaxReflectionDepth(int depth) {
    globalScene.setMaxReflectionDepth(depth);
}

// ============================================================================
// Emscripten Bindings
// ============================================================================

EMSCRIPTEN_BINDINGS(raytracer_module) {
    // Render
    emscripten::function("render", &render);
    
    // Scene
    emscripten::function("loadScenePreset", &loadScenePreset);
    emscripten::function("getSphereCount", &getSphereCount);
    
    // Light
    emscripten::function("updateLight", &updateLight);
    
    // Material
    emscripten::function("updateMaterial", &updateMaterial);
    emscripten::function("updateSphereColor", &updateSphereColor);
    emscripten::function("updateGroundReflectivity", &updateGroundReflectivity);
    
    // Camera
    emscripten::function("updateCamera", &updateCamera);
    emscripten::function("orbitCamera", &orbitCamera);
    emscripten::function("zoomCamera", &zoomCamera);
    emscripten::function("getCameraX", &getCameraX);
    emscripten::function("getCameraY", &getCameraY);
    emscripten::function("getCameraZ", &getCameraZ);
    
    // View
    emscripten::function("setShowGrid", &setShowGrid);
    emscripten::function("setGridScale", &setGridScale);
    emscripten::function("setShowGroundPlane", &setShowGroundPlane);
    emscripten::function("setMaxReflectionDepth", &setMaxReflectionDepth);
    
    // Vector type
    emscripten::register_vector<uint8_t>("VectorUint8");
}
