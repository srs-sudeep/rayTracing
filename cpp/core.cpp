#include <emscripten/bind.h>
#include <vector>
#include <cmath>
#include <algorithm>

// ============================================================================
// Vec3 - Simple 3D Vector Implementation
// ============================================================================

struct Vec3 {
    float x, y, z;

    Vec3() : x(0), y(0), z(0) {}
    Vec3(float x, float y, float z) : x(x), y(y), z(z) {}

    Vec3 operator+(const Vec3& v) const {
        return Vec3(x + v.x, y + v.y, z + v.z);
    }

    Vec3 operator-(const Vec3& v) const {
        return Vec3(x - v.x, y - v.y, z - v.z);
    }

    Vec3 operator*(float t) const {
        return Vec3(x * t, y * t, z * t);
    }

    float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    Vec3 normalize() const {
        float len = length();
        if (len > 0) {
            return Vec3(x / len, y / len, z / len);
        }
        return Vec3(0, 0, 0);
    }
};

// ============================================================================
// Ray Structure
// ============================================================================

struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalize()) {}

    Vec3 at(float t) const {
        return origin + direction * t;
    }
};

// ============================================================================
// Sphere Structure
// ============================================================================

struct Sphere {
    Vec3 center;
    float radius;
    Vec3 color;

    Sphere(const Vec3& c, float r, const Vec3& col) 
        : center(c), radius(r), color(col) {}

    // Returns t value for intersection, -1 if no hit
    float intersect(const Ray& ray) const {
        Vec3 oc = ray.origin - center;
        float a = ray.direction.dot(ray.direction);
        float b = 2.0f * oc.dot(ray.direction);
        float c = oc.dot(oc) - radius * radius;
        float discriminant = b * b - 4 * a * c;

        if (discriminant < 0) {
            return -1.0f;
        }

        float t = (-b - std::sqrt(discriminant)) / (2.0f * a);
        if (t > 0.001f) {
            return t;
        }

        t = (-b + std::sqrt(discriminant)) / (2.0f * a);
        if (t > 0.001f) {
            return t;
        }

        return -1.0f;
    }

    Vec3 getNormal(const Vec3& point) const {
        return (point - center).normalize();
    }
};

// ============================================================================
// Light Structure (Global)
// ============================================================================

struct Light {
    Vec3 position;
    float intensity;

    Light() : position(Vec3(2.0f, 2.0f, -1.0f)), intensity(1.0f) {}
};

// Global light instance
Light globalLight;

// ============================================================================
// Update Light Position (Called from JavaScript)
// ============================================================================

void updateLight(float x, float y, float z) {
    globalLight.position = Vec3(x, y, z);
}

// ============================================================================
// Scene Configuration
// ============================================================================

// Dark grey background
const Vec3 BACKGROUND_COLOR(0.1f, 0.1f, 0.12f);

// Red sphere at origin
Sphere mainSphere(Vec3(0.0f, 0.0f, 0.0f), 1.0f, Vec3(0.9f, 0.2f, 0.15f));

// Camera configuration
const Vec3 CAMERA_ORIGIN(0.0f, 0.0f, -3.0f);

// ============================================================================
// Trace Ray and Compute Color
// ============================================================================

Vec3 traceRay(const Ray& ray) {
    float t = mainSphere.intersect(ray);

    if (t > 0) {
        // Hit the sphere
        Vec3 hitPoint = ray.at(t);
        Vec3 normal = mainSphere.getNormal(hitPoint);

        // Calculate light direction
        Vec3 lightDir = (globalLight.position - hitPoint).normalize();

        // Diffuse lighting (Lambertian)
        float diffuse = std::max(0.0f, normal.dot(lightDir));

        // Ambient light
        float ambient = 0.15f;

        // Combine lighting
        float lighting = ambient + diffuse * globalLight.intensity * 0.85f;
        lighting = std::min(1.0f, lighting);

        // Apply lighting to sphere color
        return mainSphere.color * lighting;
    }

    // No hit - return background
    return BACKGROUND_COLOR;
}

// ============================================================================
// Main Render Function
// ============================================================================

std::vector<uint8_t> render(int width, int height) {
    std::vector<uint8_t> buffer(width * height * 4);

    float aspectRatio = static_cast<float>(width) / static_cast<float>(height);
    float fov = 1.0f; // Field of view factor

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            // Convert pixel coordinates to normalized device coordinates
            float u = (2.0f * x / width - 1.0f) * aspectRatio * fov;
            float v = (1.0f - 2.0f * y / height) * fov;

            // Create ray from camera through pixel
            Vec3 rayDir(u, v, 1.0f);
            Ray ray(CAMERA_ORIGIN, rayDir);

            // Trace ray and get color
            Vec3 color = traceRay(ray);

            // Convert to 8-bit RGBA
            int index = (y * width + x) * 4;
            buffer[index + 0] = static_cast<uint8_t>(std::min(255.0f, color.x * 255.0f));
            buffer[index + 1] = static_cast<uint8_t>(std::min(255.0f, color.y * 255.0f));
            buffer[index + 2] = static_cast<uint8_t>(std::min(255.0f, color.z * 255.0f));
            buffer[index + 3] = 255; // Alpha
        }
    }

    return buffer;
}

// ============================================================================
// Emscripten Bindings
// ============================================================================

EMSCRIPTEN_BINDINGS(raytracer_module) {
    emscripten::function("render", &render);
    emscripten::function("updateLight", &updateLight);

    emscripten::register_vector<uint8_t>("VectorUint8");
}

