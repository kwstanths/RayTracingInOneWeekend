#ifndef __Vector3_hpp__
#define __Vector3_hpp__

#include <cmath>
#include <iostream>

#include "Common.hpp"

using std::sqrt;

class Vector3 {
public:
    Vector3() : e{ 0,0,0 } {}
    Vector3(double e0, double e1, double e2) : e{ e0, e1, e2 } {}

    double x() const { return e[0]; }
    double y() const { return e[1]; }
    double z() const { return e[2]; }

    Vector3 operator-() const {
        return Vector3(-e[0], -e[1], -e[2]);
    }
    double operator[](int i) const { 
        return e[i]; 
    }
    double& operator[](int i) { 
        return e[i]; 
    }

    Vector3& operator+=(const Vector3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3& operator*=(const double t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3& operator/=(const double t) {
        return *this *= 1 / t;
    }

    double length() const {
        return sqrt(length_squared());
    }

    double length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline static Vector3 random() {
        return Vector3(random_double(), random_double(), random_double());
    }

    inline static Vector3 random(double min, double max) {
        return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

public:
    double e[3];
};


using Point3 = Vector3;
using Color = Vector3;


inline std::ostream& operator<<(std::ostream &out, const Vector3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

inline Vector3 operator+(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

inline Vector3 operator-(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

inline Vector3 operator*(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

inline Vector3 operator*(double t, const Vector3 &v) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, double t) {
    return t * v;
}

inline Vector3 operator/(Vector3 v, double t) {
    return (1 / t) * v;
}

inline double dot(const Vector3 &u, const Vector3 &v) {
    return u.e[0] * v.e[0]
        + u.e[1] * v.e[1]
        + u.e[2] * v.e[2];
}

inline Vector3 cross(const Vector3 &u, const Vector3 &v) {
    return Vector3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
        u.e[2] * v.e[0] - u.e[0] * v.e[2],
        u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

inline Vector3 unit_vector(Vector3 v) {
    return v / v.length();
}

inline Vector3 reflect(const Vector3& v, const Vector3& n) {
    return v - 2 * dot(v, n)*n;
}

Vector3 refract(const Vector3& uv, const Vector3& n, double etai_over_etat);

Vector3 random_in_unit_sphere();

Vector3 random_unit_vector();

Vector3 random_in_hemisphere(const Vector3& normal);

Vector3 random_in_unit_disk();

#endif
