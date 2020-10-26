#include "Vector3.hpp"

Vector3 refract(const Vector3 & uv, const Vector3 & n, Real etai_over_etat)
{
    auto cos_theta = dot(-uv, n);
    Vector3 r_out_perp = etai_over_etat * (uv + cos_theta * n);
    Vector3 r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
    return r_out_perp + r_out_parallel;
}

Vector3 random_in_unit_sphere()
{
    while (true) {
        auto p = Vector3::random(-1, 1);
        if (p.length_squared() >= 1) continue;
        return p;
    }
    return Vector3();
}

Vector3 random_unit_vector()
{
    auto a = random_double(0, 2 * pi);
    auto z = random_double(-1, 1);
    auto r = sqrt(1 - z * z);
    return Vector3(r*cos(a), r*sin(a), z);
}

Vector3 random_in_hemisphere(const Vector3 & normal) {
    Vector3 in_unit_sphere = random_in_unit_sphere();
    if (dot(in_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return in_unit_sphere;
    else
        return -in_unit_sphere;
}

Vector3 random_in_unit_disk()
{
    while (true) {
        auto p = Vector3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) continue;
        return p;
    }
}

Vector3 random_cosine_direction()
{
    auto r1 = random_double();
    auto r2 = random_double();
    auto z = sqrt(1 - r2);

    auto phi = 2 * pi*r1;
    auto x = cos(phi)*sqrt(r2);
    auto y = sin(phi)*sqrt(r2);

    return Vector3(x, y, z);
}
