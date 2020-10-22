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
            GetSphereUV((rec.p_ - center_) / radius_, rec.u_, rec.v_);
            rec.mat_ = mat_;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t_ = temp;
            rec.p_ = r.at(rec.t_);
            Vector3 outward_normal = (rec.p_ - center_) / radius_;
            rec.SetFaceNormal(r, outward_normal);
            GetSphereUV((rec.p_ - center_) / radius_, rec.u_, rec.v_);
            rec.mat_ = mat_;
            return true;
        }
    }

    return false;
}

bool Sphere::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    output_box = AABB(
        center_ - Vector3(radius_, radius_, radius_),
        center_ + Vector3(radius_, radius_, radius_));
    return true;
}

bool MovingSphere::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    /* Center of sphere at time t */
    Vector3 center_t = center(r.time_);

    Vector3 oc = r.origin() - center_t;
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
            Vector3 outward_normal = (rec.p_ - center_t) / radius_;
            rec.SetFaceNormal(r, outward_normal);
            GetSphereUV((rec.p_ - center_t) / radius_, rec.u_, rec.v_);
            rec.mat_ = mat_;
            return true;
        }

        temp = (-half_b + root) / a;
        if (temp < t_max && temp > t_min) {
            rec.t_ = temp;
            rec.p_ = r.at(rec.t_);
            Vector3 outward_normal = (rec.p_ - center_t) / radius_;
            rec.SetFaceNormal(r, outward_normal);
            GetSphereUV((rec.p_ - center_t) / radius_, rec.u_, rec.v_);
            rec.mat_ = mat_;
            return true;
        }
    }

    return false;
}

bool MovingSphere::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    AABB box0(
        center(t0) - Vector3(radius_, radius_, radius_),
        center(t0) + Vector3(radius_, radius_, radius_));
    AABB box1(
        center(t1) - Vector3(radius_, radius_, radius_),
        center(t1) + Vector3(radius_, radius_, radius_));
    output_box = AABB::surrounding_box(box0, box1);
    return true;
}

Point3 MovingSphere::center(Real time) const
{
    return center0_ + ((time - time0_) / (time1_ - time0_))*(center1_ - center0_);
}

void GetSphereUV(const Vector3 & p, Real & u, Real & v)
{
    auto phi = atan2(p.z(), p.x());
    auto theta = asin(p.y());
    u = 1 - (phi + pi) / (2 * pi);
    v = (theta + pi / 2) / pi;
}
