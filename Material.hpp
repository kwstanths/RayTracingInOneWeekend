#ifndef __Material_hpp__
#define __Material_hpp__

#include "math/Ray.hpp"
#include "math/Vector3.hpp"
#include "math/ONB.hpp"
#include "math/PDF.hpp"
#include "Texture.hpp"

struct HitRecord;

struct ScatterRecord {
    Ray specular_ray_;
    bool is_specular_;
    Color attenuation_;
    shared_ptr<PDF> pdf_;
};

class Material {
public:
    virtual Color emitted(const Ray& r_in, const HitRecord& rec, Real u, Real v, const Point3& p) const {
        return Color(0, 0, 0);
    }
    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const {
        return false;
    }

    virtual Real scattering_pdf(
        const Ray& r_in, const HitRecord& rec, const Ray& scattered
    ) const {
        return 0;
    }

};


class Lambertian : public Material {
public:
    Lambertian(const Color& a) : albedo_(make_shared<SolidColor>(a)) {};
    Lambertian(shared_ptr<Texture> a) : albedo_(a) {};

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular_ = false;
        srec.attenuation_ = albedo_->value(rec.u_, rec.v_, rec.p_);
        srec.pdf_ = make_shared<CosinePDF>(rec.normal_);
        return true;
    }

    virtual Real scattering_pdf(
        const Ray& r_in, const HitRecord& rec, const Ray& scattered
    ) const override {
        auto cosine = dot(rec.normal_, unit_vector(scattered.direction()));
        return cosine < 0 ? 0 : cosine / pi;
    }

public:
    shared_ptr<Texture> albedo_;
};


class Metal : public Material {
public:
    Metal(const Color& a, Real fuzziness) : albedo_(a), fuzziness_(fuzziness) {};

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        Vector3 reflected = reflect(unit_vector(r_in.direction()), rec.normal_);
        srec.specular_ray_ = Ray(rec.p_, reflected + fuzziness_ * random_in_unit_sphere());
        srec.attenuation_ = albedo_;
        srec.is_specular_ = true;
        srec.pdf_ = 0;
        return true;
    }

public:
    Color albedo_;
    Real fuzziness_;
};


class Dielectric : public Material {
public:
    Dielectric(Real index_of_refraction) : ir_(index_of_refraction) {};

    virtual bool scatter(
        const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
    ) const override {
        srec.is_specular_ = true;
        srec.pdf_ = nullptr;
        srec.attenuation_ = Color(1.0, 1.0, 1.0);

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

        srec.specular_ray_ = Ray(rec.p_, direction, r_in.Time());
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


class DiffuseLight : public Material {
public:
    DiffuseLight(shared_ptr<Texture> a) : emit_(a) {};
    DiffuseLight(Color c) : emit_(make_shared<SolidColor>(c)) {};

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec
        ) const override 
    {
        return false;
    }

    virtual Color emitted(const Ray& r_in, const HitRecord& rec, Real u, Real v, const Point3& p) const {
        /* If a front face is hitm emit light, otherwise, don't */
        if (rec.front_face_)
            return emit_->value(u, v, p);
        else
            return Color(0, 0, 0);
    }

public:
    shared_ptr<Texture> emit_;
};


class Isotropic : public Material {
public:
    Isotropic(Color c) : albedo_(make_shared<SolidColor>(c)) {}
    Isotropic(shared_ptr<Texture> a) : albedo_(a) {}

    virtual bool scatter(const Ray& r_in, const HitRecord& rec, ScatterRecord& srec) const override {
        /* scatter in a random direction */
        srec.is_specular_ = false;
        srec.attenuation_ = albedo_->value(rec.u_, rec.v_, rec.p_);
        srec.pdf_ = 0;
        return true;
    }

public:
    shared_ptr<Texture> albedo_;
};

#endif