#ifndef __Hittable_hpp__
#define __Hittable_hpp__

#include "Ray.hpp"
#include "Real.hpp"
#include "Vector3.hpp"

class Material;

struct HitRecord {
    Point3 p_;
    Vector3 normal_;
    shared_ptr<Material> mat_;
    Real t_;
    bool front_face_;

    inline void SetFaceNormal(const Ray& r, const Vector3& outward_normal) {
        front_face_ = dot(r.direction(), outward_normal) < 0;
        normal_ = front_face_ ? outward_normal : -outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const = 0;
};


#endif