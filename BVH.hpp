#ifndef __BVH_hpp__
#define __BVH_hpp__

#include "Common.hpp"

#include "geometry/Hittable.hpp"
#include "geometry/HittableList.hpp"


class BVHNode : public Hittable {
public:
    BVHNode() {};

    BVHNode(const HittableList& list, Real time0, Real time1) : BVHNode(list.objects_, 0, list.objects_.size(), time0, time1) {};

    BVHNode(const std::vector<shared_ptr<Hittable>>& src_objects, size_t start, size_t end, Real time0, Real time1);

    virtual bool hit( const Ray& r, Real tmin, Real tmax, HitRecord& rec) const override;

    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

public:
    shared_ptr<Hittable> left;
    shared_ptr<Hittable> right;
    AABB box;

private:
};

bool box_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b, int axis);
bool box_x_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
bool box_y_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);
bool box_z_compare(const shared_ptr<Hittable> a, const shared_ptr<Hittable> b);

#endif