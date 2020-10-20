#include "Sphere.hpp"

bool Sphere::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    Vector3 oc = r.origin() - center_;
    auto a = r.direction().length_squared();
    auto half_b = dot(oc, r.direction());
    auto c = oc.length_squared() - radius_ * radius_;
    auto discriminant = half_b * half_b - a * c;

    if (discriminant > 0) {
        auto root = sqrt(discriminant);

        auto temp = (-half_b - root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t_ = temp;
            rec.p_ = r.at(rec.t_);
            Vector3 outward_normal = (rec.p_ - center_) / radius_;
            rec.SetFaceNormal(r, outward_normal);
            rec.mat_ = mat_;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t_ = temp;
            rec.p_ = r.at(rec.t_);
            Vector3 outward_normal = (rec.p_ - center_) / radius_;
            rec.SetFaceNormal(r, outward_normal);
            rec.mat_ = mat_;
            return true;
        }
    }

    return false;

}
