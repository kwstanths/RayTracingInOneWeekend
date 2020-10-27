#ifndef __Hittable_hpp__
#define __Hittable_hpp__

#include "math/Ray.hpp"
#include "math/Real.hpp"
#include "math/Vector3.hpp"
#include "geometry/AABB.hpp"

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
    /* Caclulate the intersection with object, within the given time margin, store the result to rec */
    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const = 0;

    /* Return the bounding box of the object, within the given time margin */
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const = 0;
    
    /* Caclulate the probability the a ray starting from o towards v, hits the object */
    virtual double pdf_value(const Point3& o, const Vector3& v) const {
        return 0.0;
    }

    /* Calculate a random point on the Hittable object */
    virtual Vector3 random(const Vector3& o) const {
        return Vector3(1, 0, 0);
    }
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


class FlipFace : public Hittable {
public:
    FlipFace(shared_ptr<Hittable> p) : ptr_(p) {}

    virtual bool hit(
        const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override {

        if (!ptr_->hit(r, t_min, t_max, rec))
            return false;

        rec.front_face_ = !rec.front_face_;
        return true;
    }

    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override {
        return ptr_->bounding_box(t0, t1, output_box);
    }

public:
    shared_ptr<Hittable> ptr_;
};

#endif