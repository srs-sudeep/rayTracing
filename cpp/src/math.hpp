#pragma once

#include <cmath>

// ============================================================================
// Vec3 - 3D Vector Implementation
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

    Vec3 operator*(const Vec3& v) const {
        return Vec3(x * v.x, y * v.y, z * v.z);
    }

    float dot(const Vec3& v) const {
        return x * v.x + y * v.y + z * v.z;
    }

    Vec3 cross(const Vec3& v) const {
        return Vec3(
            y * v.z - z * v.y,
            z * v.x - x * v.z,
            x * v.y - y * v.x
        );
    }

    float length() const {
        return std::sqrt(x * x + y * y + z * z);
    }

    float lengthSquared() const {
        return x * x + y * y + z * z;
    }

    Vec3 normalize() const {
        float len = length();
        if (len > 0) {
            return Vec3(x / len, y / len, z / len);
        }
        return Vec3(0, 0, 0);
    }

    Vec3 reflect(const Vec3& normal) const {
        return *this - normal * (2.0f * this->dot(normal));
    }

    static Vec3 lerp(const Vec3& a, const Vec3& b, float t) {
        return a * (1.0f - t) + b * t;
    }
};

// ============================================================================
// Ray - Ray with origin and direction
// ============================================================================

struct Ray {
    Vec3 origin;
    Vec3 direction;

    Ray() : origin(), direction(0, 0, 1) {}
    Ray(const Vec3& o, const Vec3& d) : origin(o), direction(d.normalize()) {}

    Vec3 at(float t) const {
        return origin + direction * t;
    }
};

