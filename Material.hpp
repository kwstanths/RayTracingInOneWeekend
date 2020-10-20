#ifndef __Material_hpp__
#define __Material_hpp__

#include "Ray.hpp"
#include "Vector3.hpp"

struct HitRecord;

class Material {
public:
    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
    ) const = 0;
};


class Lambertian : public Material {
public:
    Lambertian(const Color& a) : albedo_(a) {}

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
    ) const override {
        Vector3 scatter_direction = rec.normal_ + random_unit_vector();
        scattered = Ray(rec.p_, scatter_direction);
        attenuation = albedo_;
        return true;
    }

public:
    Color albedo_;
};


class Metal : public Material {
public:
    Metal(const Color& a, Real fuzziness) : albedo_(a), fuzziness_(fuzziness) {};

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
    ) const override {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_);
        scattered = Ray(rec.p_, reflected + fuzziness_ * random_in_unit_sphere());
        attenuation = albedo_;
        return (dot(scattered.direction(), rec.normal_) > 0);
    }

public:
    Color albedo_;
    Real fuzziness_;
};

class Dielectric : public Material {
public:
    Dielectric(Real index_of_refraction) : ir_(index_of_refraction) {};

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, Color& attenuation, Ray& scattered
    ) const override {
        attenuation = Color(1.0, 1.0, 1.0);
        double refraction_ratio = rec.front_face_ ? (1.0 / ir_) : ir_;

        Vector3 unit_direction = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-unit_direction, rec.normal_), 1.0);
        double sin_theta = sqrt(1.0 - cos_theta * cos_theta);
        bool cannot_refract = refraction_ratio * sin_theta > 1.0;
        Vector3 direction;
        if (cannot_refract || reflectance(cos_theta, refraction_ratio) > random_double()) {
            direction = reflect(unit_direction, rec.normal_);
        }
        else
            direction = refract(unit_direction, rec.normal_, refraction_ratio);

        scattered = Ray(rec.p_, direction);
        return true;
    }

public:
    Real ir_;

private:
    static double reflectance(double cosine, double ref_idx) {
        // Use Schlick's approximation for reflectance.
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0)*pow((1 - cosine), 5);
    }
};

#endif