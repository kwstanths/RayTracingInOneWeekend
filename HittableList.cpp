#include "HittableList.hpp"

bool HittableList::hit(const Ray & r, Real tmin, Real tmax, HitRecord & rec) const
{
    HitRecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = tmax;

    for (const auto& object : objects_) {
        if (object->hit(r, tmin, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t_;
            rec = temp_rec;
        }
    }

    return hit_anything;
}

bool HittableList::bounding_box(double t0, double t1, AABB & output_box) const
{
    if (objects_.empty()) return false;

    AABB temp_box;
    bool first_box = true;

    for (const auto& object : objects_) {
        if (!object->bounding_box(t0, t1, temp_box)) return false;
        output_box = first_box ? temp_box : AABB::surrounding_box(output_box, temp_box);
        first_box = false;
    }

    return true;
}
