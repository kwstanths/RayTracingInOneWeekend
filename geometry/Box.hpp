#ifndef __Box_hpp__
#define __Box_hpp__

#include "Common.hpp"

#include "AARect.hpp"
#include "HittableList.hpp"

class Box : public Hittable {
public:
    Box() {};
    Box(const Point3& p0, const Point3& p1, shared_ptr<Material> mat);

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override {
        output_box = AABB(box_min_, box_max_);
        return true;
    }

public:
    Point3 box_min_;
    Point3 box_max_;
    HittableList sides_;
};

#endif