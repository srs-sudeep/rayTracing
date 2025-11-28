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

int addLight(float x, float y, float z, float r, float g, float b, float intensity) {
    return globalScene.addLight(x, y, z, r, g, b, intensity);
}

void removeLight(int index) {
    globalScene.removeLight(index);
}

void setLightPosition(int index, float x, float y, float z) {
    globalScene.setLightPosition(index, x, y, z);
}

void setLightColor(int index, float r, float g, float b) {
    globalScene.setLightColor(index, r, g, b);
}

void setLightIntensity(int index, float intensity) {
    globalScene.setLightIntensity(index, intensity);
}

int getLightCount() {
    return globalScene.getLightCount();
}

float getLightX(int index) { return globalScene.getLightX(index); }
float getLightY(int index) { return globalScene.getLightY(index); }
float getLightZ(int index) { return globalScene.getLightZ(index); }
float getLightR(int index) { return globalScene.getLightR(index); }
float getLightG(int index) { return globalScene.getLightG(index); }
float getLightB(int index) { return globalScene.getLightB(index); }
float getLightIntensity(int index) { return globalScene.getLightIntensity(index); }

void resetLights() {
    globalScene.resetLights();
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

void setCameraFov(float fov) {
    globalScene.setCameraFov(fov);
}

void setCameraTarget(float x, float y, float z) {
    globalScene.setCameraTarget(x, y, z);
}

float getCameraFov() { return globalScene.getCameraFov(); }
float getCameraTargetX() { return globalScene.getCameraTargetX(); }
float getCameraTargetY() { return globalScene.getCameraTargetY(); }
float getCameraTargetZ() { return globalScene.getCameraTargetZ(); }

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
    emscripten::function("addLight", &addLight);
    emscripten::function("removeLight", &removeLight);
    emscripten::function("setLightPosition", &setLightPosition);
    emscripten::function("setLightColor", &setLightColor);
    emscripten::function("setLightIntensity", &setLightIntensity);
    emscripten::function("getLightCount", &getLightCount);
    emscripten::function("getLightX", &getLightX);
    emscripten::function("getLightY", &getLightY);
    emscripten::function("getLightZ", &getLightZ);
    emscripten::function("getLightR", &getLightR);
    emscripten::function("getLightG", &getLightG);
    emscripten::function("getLightB", &getLightB);
    emscripten::function("getLightIntensity", &getLightIntensity);
    emscripten::function("resetLights", &resetLights);
    
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
    emscripten::function("setCameraFov", &setCameraFov);
    emscripten::function("setCameraTarget", &setCameraTarget);
    emscripten::function("getCameraFov", &getCameraFov);
    emscripten::function("getCameraTargetX", &getCameraTargetX);
    emscripten::function("getCameraTargetY", &getCameraTargetY);
    emscripten::function("getCameraTargetZ", &getCameraTargetZ);
    
    // View
    emscripten::function("setShowGrid", &setShowGrid);
    emscripten::function("setGridScale", &setGridScale);
    emscripten::function("setShowGroundPlane", &setShowGroundPlane);
    emscripten::function("setMaxReflectionDepth", &setMaxReflectionDepth);
    
    // Vector type
    emscripten::register_vector<uint8_t>("VectorUint8");
}
