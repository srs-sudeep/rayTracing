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

### 💎 Glass Spheres

Transparent and refractive spheres demonstrating the refraction system.

```cpp
// Large glass sphere center
Material glassMat = Material::glass();
spheres.push_back(Sphere(Vec3(0, 0, 0), 1.0f, glassMat));

// Diamond sphere left
Material diamondMat = Material::diamond();
spheres.push_back(Sphere(Vec3(-2.0f, -0.4f, 0.5f), 0.6f, diamondMat));

// Blue-tinted glass right
Material blueTint = Material::glass(Vec3(0.85f, 0.9f, 1.0f));
spheres.push_back(Sphere(Vec3(1.8f, -0.5f, 0.8f), 0.5f, blueTint));

// Solid red behind (to see distortion)
Material redMat(Vec3(0.95f, 0.15f, 0.15f), 0.6f, 64.0f);
spheres.push_back(Sphere(Vec3(0, -0.3f, -2.5f), 0.7f, redMat));

// Water sphere
Material waterMat = Material::water();
spheres.push_back(Sphere(Vec3(-0.7f, -0.7f, 1.5f), 0.3f, waterMat));
```

- **Spheres**: 5
- **Good for**: Refraction effects, Fresnel, transparent materials

### 🧊 Primitives (Shapes)

Mixed shapes showcasing spheres, boxes, and cylinders.

```cpp
// Red metallic sphere center
Material sphereMat(Vec3(0.9f, 0.3f, 0.2f), 0.8f, 128.0f);
spheres.push_back(Sphere(Vec3(0, 0, 0), 0.8f, sphereMat));

// Blue cube left
Material boxMat(Vec3(0.2f, 0.4f, 0.9f), 0.7f, 64.0f);
boxes.push_back(Box(Vec3(-2.2f, -0.35f, 0.3f), Vec3(1.3f, 1.3f, 1.3f), boxMat));

// Green cylinder right
Material cylMat(Vec3(0.2f, 0.85f, 0.4f), 0.6f, 48.0f);
cylinders.push_back(Cylinder(Vec3(2.0f, -1.0f, 0.5f), 0.5f, 1.4f, cylMat));

// Glass cube
Material glassMat = Material::glass();
boxes.push_back(Box(Vec3(1.0f, -0.6f, 1.8f), Vec3(0.8f, 0.8f, 0.8f), glassMat));

// More spheres and cylinders...
```

- **Objects**: 3 spheres, 2 boxes, 2 cylinders
- **Good for**: Showcasing all primitive types, mixed materials

## Preset Selection

Use the Scene toolbar above the canvas:

```jsx
const SCENE_PRESETS = [
  { id: 0, name: 'Single', icon: '🔴' },
  { id: 1, name: 'Three', icon: '🎱' },
  { id: 2, name: 'Mirror', icon: '🪞' },
  { id: 3, name: 'Rainbow', icon: '🌈' },
  { id: 4, name: 'Glass', icon: '💎' },
  { id: 5, name: 'Shapes', icon: '🧊' },
];
```

Click a button to instantly switch scenes.

## Implementation

```cpp
void Scene::loadPreset(ScenePreset preset) {
    spheres.clear();
    boxes.clear();
    cylinders.clear();
    
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
        case ScenePreset::GLASS_SPHERES:
            // Add transparent spheres
            break;
        case ScenePreset::PRIMITIVES:
            // Add mixed shapes
            break;
    }
}
```

JavaScript binding:

```javascript
wasmModule.loadScenePreset(presetId);

// Get object counts
const spheres = wasmModule.getSphereCount();
const boxes = wasmModule.getBoxCount();
const cylinders = wasmModule.getCylinderCount();
const total = wasmModule.getTotalObjectCount();
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

| Preset | Objects | Relative Render Time |
|--------|---------|---------------------|
| Single | 1 sphere | 1× |
| Three | 3 spheres | ~1.5× |
| Mirror | 5 spheres | ~2× (more reflections) |
| Rainbow | 7 spheres | ~2.5× |
| Glass | 5 spheres | ~3× (refraction rays) |
| Shapes | 7 mixed | ~2.5× |

More objects = more intersection tests per ray.  
Transparent objects = additional refraction rays per hit.

