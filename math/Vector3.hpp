#ifndef __Vector3_hpp__
#define __Vector3_hpp__

#include <cmath>
#include <iostream>

#include "Common.hpp"

using std::sqrt;

class Vector3 {
public:
    Vector3() : e{ 0,0,0 } {}
    Vector3(Real e0, Real e1, Real e2) : e{ e0, e1, e2 } {}

    Real x() const { return e[0]; }
    Real y() const { return e[1]; }
    Real z() const { return e[2]; }

    Vector3 operator-() const {
        return Vector3(-e[0], -e[1], -e[2]);
    }
    Real operator[](int i) const { 
        return e[i]; 
    }
    Real& operator[](int i) { 
        return e[i]; 
    }

    Vector3& operator+=(const Vector3 &v) {
        e[0] += v.e[0];
        e[1] += v.e[1];
        e[2] += v.e[2];
        return *this;
    }

    Vector3& operator*=(const Real t) {
        e[0] *= t;
        e[1] *= t;
        e[2] *= t;
        return *this;
    }

    Vector3& operator/=(const Real t) {
        return *this *= 1 / t;
    }

    Real length() const {
        return sqrt(length_squared());
    }

    Real length_squared() const {
        return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
    }

    inline static Vector3 random() {
        return Vector3(random_double(), random_double(), random_double());
    }

    inline static Vector3 random(Real min, Real max) {
        return Vector3(random_double(min, max), random_double(min, max), random_double(min, max));
    }

public:
    Real e[3];
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

inline Vector3 operator*(Real t, const Vector3 &v) {
    return Vector3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

inline Vector3 operator*(const Vector3 &v, Real t) {
    return t * v;
}

inline Vector3 operator/(Vector3 v, Real t) {
    return (1 / t) * v;
}

inline Real dot(const Vector3 &u, const Vector3 &v) {
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

Vector3 refract(const Vector3& uv, const Vector3& n, Real etai_over_etat);

Vector3 random_in_unit_sphere();

Vector3 random_unit_vector();

Vector3 random_in_hemisphere(const Vector3& normal);

Vector3 random_in_unit_disk();

Vector3 random_cosine_direction();

#endif
