#ifndef __HittableList_hpp__
#define __HittableList_hpp__

#include "Hittable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:
    HittableList() {
    }
    HittableList(shared_ptr<Hittable> object) { 
        add(object); 
    }

    void clear() { 
        objects_.clear(); 
    }
    void add(shared_ptr<Hittable> object) { 
        objects_.push_back(object); 
    }

    virtual bool hit(const Ray& r, Real tmin, Real tmax, HitRecord& rec) const override;

    virtual bool bounding_box(double t0, double t1, AABB& output_box) const override;

public:
    std::vector<shared_ptr<Hittable>> objects_;
};

#endif