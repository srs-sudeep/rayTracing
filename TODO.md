# RayTracerStudio - Feature Roadmap

A progressive enhancement plan for the ray tracer, organized by commits.

---

## Phase 1: Core Rendering Improvements

### ✅ Commit 0: Initial Setup
- [x] Basic ray-sphere intersection
- [x] Diffuse (Lambertian) shading
- [x] Interactive light controls
- [x] WebAssembly integration

### ✅ Commit 1: Specular Highlights + Modular Refactor
- [x] Implement Blinn-Phong specular model
- [x] Add shininess/roughness parameter
- [x] UI slider for specular intensity
- [x] UI slider for shininess
- [x] Modular C++ architecture (Vec3, Ray, Material, Light, Camera, Sphere, Scene, Renderer)
- [x] Modular React components (hooks, ui, controls, canvas, layout)
- [x] Tabbed control panel (Light, Material, Camera, View)
- [x] Canvas grid overlay
- [x] Mouse orbit camera controls
- [x] Scroll wheel zoom
- [x] Resolution selector
- [x] Material presets (Matte, Plastic, Glossy, Metal)
- [x] Performance stats (render time, MP/s, FPS)

### 🔲 Commit 2: Ground Plane
- [ ] Add infinite plane intersection
- [ ] Checkerboard procedural texture
- [ ] Plane receives shadows from sphere

### 🔲 Commit 3: Reflections
- [ ] Recursive ray tracing (max depth)
- [ ] Reflectivity parameter per material
- [ ] UI control for reflection intensity
- [ ] Sphere reflects ground, ground reflects sphere

### 🔲 Commit 4: Multiple Spheres
- [ ] Dynamic sphere array in C++
- [ ] Add/remove spheres from UI
- [ ] Different colors per sphere
- [ ] Proper closest-hit detection

---

## Phase 2: Enhanced Lighting

### 🔲 Commit 5: Camera Controls
- [ ] Camera position sliders (X, Y, Z)
- [ ] Look-at target point
- [ ] Field of view slider
- [ ] Orbit controls (optional: mouse drag)

### 🔲 Commit 6: Multiple & Colored Lights
- [ ] Support array of lights
- [ ] RGB color per light
- [ ] Light intensity control
- [ ] UI to add/configure lights

### 🔲 Commit 7: Anti-Aliasing
- [ ] Supersampling (2x2, 4x4 options)
- [ ] Jittered sampling for smoothness
- [ ] UI toggle for AA level
- [ ] Performance impact display

### 🔲 Commit 8: Soft Shadows
- [ ] Area light representation
- [ ] Multiple shadow ray samples
- [ ] Shadow softness parameter
- [ ] Penumbra effect

---

## Phase 3: Advanced Materials

### 🔲 Commit 9: Refractions
- [ ] Snell's law implementation
- [ ] Index of refraction parameter
- [ ] Transparent/glass materials
- [ ] Fresnel effect for realistic edges

### 🔲 Commit 10: Material System
- [ ] Material struct (diffuse, specular, reflection, refraction)
- [ ] Preset materials (metal, glass, plastic, mirror)
- [ ] Material editor panel in UI
- [ ] Color pickers for material colors

---

## Phase 4: Scene & Objects

### 🔲 Commit 11: More Primitives
- [ ] Box/Cube intersection
- [ ] Cylinder intersection
- [ ] Object type selector in UI

### 🔲 Commit 12: Scene Presets
- [ ] "Classic" - single sphere
- [ ] "Reflections" - mirror spheres on plane
- [ ] "Glass" - transparent sphere with caustics
- [ ] "Cornell Box" - colored walls
- [ ] Preset dropdown in UI

---

## Phase 5: Polish & Export

### 🔲 Commit 13: Image Quality
- [ ] Gamma correction (2.2)
- [ ] Tone mapping (Reinhard or ACES)
- [ ] Exposure control
- [ ] Resolution presets (256, 512, 1024, 2048)

### 🔲 Commit 14: Export & UI Polish
- [ ] Download as PNG button
- [ ] Copy render stats
- [ ] Keyboard shortcuts
- [ ] Loading states during render

### 🔲 Commit 15: Performance
- [ ] Web Workers for multi-threading
- [ ] Progressive rendering (low-res first)
- [ ] Tile-based rendering with progress
- [ ] Cancel ongoing render

---

## Future Ideas (Backlog)

- [ ] Path tracing / Global illumination
- [ ] Depth of field (bokeh)
- [ ] Motion blur
- [ ] Texture mapping (images on surfaces)
- [ ] Normal mapping
- [ ] BVH acceleration structure
- [ ] Triangle mesh support
- [ ] OBJ file loading
- [ ] Environment maps (HDRI)
- [ ] Volumetric effects (fog, god rays)

---

## Progress Tracking

| Phase | Commits | Status |
|-------|---------|--------|
| Phase 1: Core | 0-4 | 🟡 In Progress (1/4 complete) |
| Phase 2: Lighting | 5-8 | ⚪ Not Started |
| Phase 3: Materials | 9-10 | ⚪ Not Started |
| Phase 4: Scene | 11-12 | ⚪ Not Started |
| Phase 5: Polish | 13-15 | ⚪ Not Started |

---

*Last updated: November 2024*

