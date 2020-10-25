#ifndef __Hittable_hpp__
#define __Hittable_hpp__

#include "Ray.hpp"
#include "Real.hpp"
#include "Vector3.hpp"
#include "AABB.hpp"

class Material;

struct HitRecord {
    /* Intersection point */
    Point3 p_;
    /* Normal on intersection */
    Vector3 normal_;
    /* Material of intersection point */
    shared_ptr<Material> mat_;
    /* Value of t in the ray parametric representation */
    Real t_;
    /* u coordinate of intersection point (texturing) */
    Real u_;
    /* v coordinate of intersection point (texturing) */
    Real v_;
    /* Is a front face or a back face? */
    bool front_face_;

    inline void SetFaceNormal(const Ray& r, const Vector3& outward_normal) {
        front_face_ = dot(r.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal : -outward_normal;
    }
};


class Hittable {
public:
    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const = 0;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const = 0;
};


class Translate : public Hittable {
public:
    Translate(shared_ptr<Hittable> p, const Vector3& displacement): ptr_(p), offset_(displacement) {}

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

public:
    shared_ptr<Hittable> ptr_;
    Vector3 offset_;
};


class RotateY : public Hittable {
public:
    RotateY(shared_ptr<Hittable> p, const Real angle);

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

public:
    shared_ptr<Hittable> ptr_;
    double sin_theta_;
    double cos_theta_;
    bool hasbox_;
    AABB bbox_;
};

#endif