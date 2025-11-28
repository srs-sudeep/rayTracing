---
sidebar_position: 7
---

# API Reference

Complete reference for all WASM-exposed functions.

## Render

### `render(width, height)`

Renders the scene and returns a pixel buffer.

```typescript
function render(width: number, height: number): VectorUint8
```

**Parameters:**
- `width` - Render width in pixels
- `height` - Render height in pixels

**Returns:** `VectorUint8` - RGBA pixel buffer (must call `.delete()` after use)

**Example:**
```javascript
const pixels = wasmModule.render(512, 512);
// Use pixels...
pixels.delete();  // Free memory!
```

---

## Scene

### `loadScenePreset(preset)`

Loads a predefined scene configuration.

```typescript
function loadScenePreset(preset: number): void
```

**Preset values:**
- `0` - Single Sphere
- `1` - Three Spheres
- `2` - Mirror Spheres
- `3` - Rainbow

### `getSphereCount()`

Returns the number of spheres in the current scene.

```typescript
function getSphereCount(): number
```

---

## Light

RayTracer Studio supports **multiple colored lights** (up to 4).

### `addLight(x, y, z, r, g, b, intensity)`

Adds a new light to the scene.

```typescript
function addLight(
  x: number, y: number, z: number,  // Position
  r: number, g: number, b: number,  // Color (0-1)
  intensity: number                  // Brightness (0-2)
): number  // Returns the new light's index
```

### `removeLight(index)`

Removes a light by its index.

```typescript
function removeLight(index: number): void
```

:::warning
Cannot remove the last light. At least one light must remain.
:::

### `setLightPosition(index, x, y, z)`

Updates a specific light's position.

```typescript
function setLightPosition(
  index: number,
  x: number, y: number, z: number
): void
```

### `setLightColor(index, r, g, b)`

Updates a specific light's color.

```typescript
function setLightColor(
  index: number,
  r: number, g: number, b: number  // 0.0 - 1.0
): void
```

### `setLightIntensity(index, intensity)`

Updates a specific light's brightness.

```typescript
function setLightIntensity(
  index: number,
  intensity: number  // 0.0 - 2.0
): void
```

### `getLightCount()`

Returns the number of lights in the scene.

```typescript
function getLightCount(): number
```

### Light Property Getters

```typescript
function getLightX(index: number): number
function getLightY(index: number): number
function getLightZ(index: number): number
function getLightR(index: number): number
function getLightG(index: number): number
function getLightB(index: number): number
function getLightIntensity(index: number): number
```

### `resetLights()`

Resets to a single default white light.

```typescript
function resetLights(): void
```

### `updateLight(x, y, z)` *(Legacy)*

Updates the first light's position only.

```typescript
function updateLight(x: number, y: number, z: number): void
```

---

## Material

### `updateMaterial(specular, shininess, reflectivity)`

Updates the main sphere's material properties.

```typescript
function updateMaterial(
  specular: number,      // 0.0 - 1.0
  shininess: number,     // 2 - 256
  reflectivity: number   // 0.0 - 1.0
): void
```

### `updateSphereColor(r, g, b)`

Sets the main sphere's base color.

```typescript
function updateSphereColor(
  r: number,  // 0.0 - 1.0
  g: number,  // 0.0 - 1.0
  b: number   // 0.0 - 1.0
): void
```

### `updateGroundReflectivity(reflectivity)`

Sets the ground plane's reflectivity.

```typescript
function updateGroundReflectivity(reflectivity: number): void
```

---

## Camera

### `updateCamera(x, y, z)`

Sets the camera position.

```typescript
function updateCamera(x: number, y: number, z: number): void
```

### `orbitCamera(deltaX, deltaY)`

Orbits the camera around its target.

```typescript
function orbitCamera(deltaX: number, deltaY: number): void
```

**Parameters:**
- `deltaX` - Horizontal rotation (pixels of mouse movement)
- `deltaY` - Vertical rotation (pixels of mouse movement)

### `zoomCamera(delta)`

Moves camera closer or farther from target.

```typescript
function zoomCamera(delta: number): void
```

### `setCameraFov(fov)`

Sets the camera field of view.

```typescript
function setCameraFov(fov: number): void  // 10 - 120 degrees
```

### `setCameraTarget(x, y, z)`

Sets the camera look-at target.

```typescript
function setCameraTarget(x: number, y: number, z: number): void
```

### Camera Getters

```typescript
function getCameraX(): number
function getCameraY(): number
function getCameraZ(): number
function getCameraFov(): number
function getCameraTargetX(): number
function getCameraTargetY(): number
function getCameraTargetZ(): number
```

---

## View

### `setShowGrid(show)`

Toggles the grid pattern on the ground plane.

```typescript
function setShowGrid(show: boolean): void
```

### `setGridScale(scale)`

Sets the grid cell size.

```typescript
function setGridScale(scale: number): void  // 0.5 - 4.0
```

### `setShowGroundPlane(show)`

Toggles ground plane visibility.

```typescript
function setShowGroundPlane(show: boolean): void
```

### `setMaxReflectionDepth(depth)`

Sets the maximum reflection recursion depth.

```typescript
function setMaxReflectionDepth(depth: number): void  // 1 - 10
```

---

## VectorUint8

The return type of `render()`. A wrapper around `std::vector<uint8_t>`.

### `size()`

Returns the number of bytes in the buffer.

```typescript
function size(): number
```

### `get(index)`

Gets a byte at the specified index.

```typescript
function get(index: number): number
```

### `delete()`

Frees the C++ memory. **Must be called** to prevent memory leaks.

```typescript
function delete(): void
```

**Complete usage:**
```javascript
const buffer = wasmModule.render(512, 512);

const size = buffer.size();  // 512 * 512 * 4 = 1,048,576
const pixelData = new Uint8ClampedArray(size);

for (let i = 0; i < size; i++) {
  pixelData[i] = buffer.get(i);
}

buffer.delete();  // Important!

const imageData = new ImageData(pixelData, 512, 512);
ctx.putImageData(imageData, 0, 0);
```

