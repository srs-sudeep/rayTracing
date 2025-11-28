---
sidebar_position: 6
---

# Scene Presets

Scene presets provide quick access to different sphere arrangements with varied materials.

## Available Presets

### 🔴 Single Sphere

The classic ray tracing demo - one red sphere at the origin.

```cpp
Material redMaterial(Vec3(0.9f, 0.2f, 0.15f), 0.5f, 32.0f);
redMaterial.reflectivity = 0.3f;
spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f, redMaterial));
```

- **Spheres**: 1
- **Good for**: Testing materials, lighting, learning basics

### 🎱 Three Spheres

Red, blue, and green spheres with different sizes and positions.

```cpp
// Red center
Material redMat(Vec3(0.9f, 0.2f, 0.15f), 0.6f, 64.0f);
redMat.reflectivity = 0.3f;
spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f, redMat));

// Blue left
Material blueMat(Vec3(0.2f, 0.4f, 0.9f), 0.7f, 128.0f);
blueMat.reflectivity = 0.5f;
spheres.push_back(Sphere(Vec3(-2.2f, -0.3f, 0.5f), 0.7f, blueMat));

// Green right
Material greenMat(Vec3(0.2f, 0.85f, 0.3f), 0.5f, 48.0f);
greenMat.reflectivity = 0.2f;
spheres.push_back(Sphere(Vec3(2.0f, -0.5f, 1.0f), 0.5f, greenMat));
```

- **Spheres**: 3
- **Good for**: Seeing reflections between objects, comparing materials

### 🪞 Mirror Spheres

Highly reflective metallic spheres showcasing the reflection system.

```cpp
// Chrome center
Material chromeMat(Vec3(0.95f, 0.95f, 0.95f), 1.0f, 256.0f);
chromeMat.reflectivity = 0.95f;
spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f, chromeMat));

// Gold left
Material goldMat(Vec3(1.0f, 0.84f, 0.0f), 0.9f, 128.0f);
goldMat.reflectivity = 0.8f;
spheres.push_back(Sphere(Vec3(-2.0f, -0.4f, 0.3f), 0.6f, goldMat));

// Rose gold right
Material roseMat(Vec3(0.95f, 0.5f, 0.5f), 0.85f, 96.0f);
roseMat.reflectivity = 0.7f;
spheres.push_back(Sphere(Vec3(1.8f, -0.5f, 0.8f), 0.5f, roseMat));

// Small accents
Material accentMat(Vec3(0.3f, 0.3f, 0.35f), 1.0f, 256.0f);
accentMat.reflectivity = 0.9f;
spheres.push_back(Sphere(Vec3(-0.8f, -0.75f, 1.5f), 0.25f, accentMat));
spheres.push_back(Sphere(Vec3(0.6f, -0.8f, 1.8f), 0.2f, accentMat));
```

- **Spheres**: 5
- **Good for**: Recursive reflections, Fresnel effect, metallic materials

### 🌈 Rainbow

Seven colorful spheres arranged in an arc.

```cpp
Vec3 colors[] = {
    Vec3(0.95f, 0.2f, 0.2f),   // Red
    Vec3(0.95f, 0.5f, 0.1f),   // Orange
    Vec3(0.95f, 0.9f, 0.1f),   // Yellow
    Vec3(0.2f, 0.9f, 0.3f),    // Green
    Vec3(0.2f, 0.6f, 0.95f),   // Blue
    Vec3(0.5f, 0.2f, 0.9f),    // Indigo
    Vec3(0.8f, 0.3f, 0.8f)     // Violet
};

for (int i = 0; i < 7; i++) {
    Material mat(colors[i], 0.6f, 64.0f);
    mat.reflectivity = 0.35f;
    
    float x = -3.0f + i * 1.0f;
    float y = -0.5f + sin(i * 0.5f) * 0.3f;
    float z = 0.5f + cos(i * 0.4f) * 0.5f;
    
    spheres.push_back(Sphere(Vec3(x, y, z), 0.4f, mat));
}
```

- **Spheres**: 7
- **Good for**: Color variety, performance testing, fun visuals

## Preset Selection

Use the Scene toolbar above the canvas:

```jsx
const SCENE_PRESETS = [
  { id: 0, name: 'Single', icon: '🔴' },
  { id: 1, name: 'Three', icon: '🎱' },
  { id: 2, name: 'Mirror', icon: '🪞' },
  { id: 3, name: 'Rainbow', icon: '🌈' },
];
```

Click a button to instantly switch scenes.

## Implementation

```cpp
void Scene::loadPreset(ScenePreset preset) {
    spheres.clear();  // Remove existing spheres
    
    switch (preset) {
        case ScenePreset::SINGLE_SPHERE:
            // Add single sphere
            break;
        case ScenePreset::THREE_SPHERES:
            // Add three spheres
            break;
        case ScenePreset::MIRROR_SPHERES:
            // Add mirror spheres
            break;
        case ScenePreset::RAINBOW:
            // Add rainbow spheres
            break;
    }
}
```

JavaScript binding:

```javascript
wasmModule.loadScenePreset(presetId);
const count = wasmModule.getSphereCount();
```

## Material Controls

Material tab controls affect the **first sphere** only:

```cpp
void updateMainSphere(float specular, float shininess, float reflectivity) {
    if (!spheres.empty()) {
        spheres[0].material.specularIntensity = specular;
        spheres[0].material.shininess = shininess;
        spheres[0].material.reflectivity = reflectivity;
    }
}
```

Other spheres keep their preset materials.

## Performance Notes

| Preset | Spheres | Relative Render Time |
|--------|---------|---------------------|
| Single | 1 | 1× |
| Three | 3 | ~1.5× |
| Mirror | 5 | ~2× (more reflections) |
| Rainbow | 7 | ~2.5× |

More spheres = more intersection tests per ray.

