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
