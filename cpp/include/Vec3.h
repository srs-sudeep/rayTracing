#pragma once

#include <cmath>

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

    // Reflect vector around normal
    Vec3 reflect(const Vec3& normal) const {
        return *this - normal * (2.0f * this->dot(normal));
    }

    // Clamp each component between 0 and 1
    Vec3 clamp() const {
        return Vec3(
            std::fmax(0.0f, std::fmin(1.0f, x)),
            std::fmax(0.0f, std::fmin(1.0f, y)),
            std::fmax(0.0f, std::fmin(1.0f, z))
        );
    }

    // Refract vector through surface using Snell's law
    // eta = n1/n2 (ratio of refractive indices)
    // Returns zero vector if total internal reflection occurs
    Vec3 refract(const Vec3& normal, float eta) const {
        float cosI = -this->dot(normal);
        float sin2T = eta * eta * (1.0f - cosI * cosI);
        
        // Total internal reflection
        if (sin2T > 1.0f) {
            return Vec3(0, 0, 0);
        }
        
        float cosT = std::sqrt(1.0f - sin2T);
        return *this * eta + normal * (eta * cosI - cosT);
    }
};

