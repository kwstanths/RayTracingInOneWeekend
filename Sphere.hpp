#ifndef __Sphere_hpp__
#define __Sphere_hpp__

#include "Hittable.hpp"
#include "Vector3.hpp"
#include "Material.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, Real r, shared_ptr<Material> mat) : center_(cen), radius_(r), mat_(mat) {
    };

    virtual bool hit(
        const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;

public:
    Point3 center_;
    Real radius_;
    shared_ptr<Material> mat_;
};

#endif
