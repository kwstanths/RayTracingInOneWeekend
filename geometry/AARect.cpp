#include "AARect.hpp"

bool XYRect::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    auto t = (k_ - r.origin().z()) / r.direction().z();
    /* Check if intersection happens within the time limit given */
    if (t < t_min || t > t_max)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto y = r.origin().y() + t * r.direction().y();

    /* Check if intersection point on z=k the plane is within the rectangle limits */
    if (x < x0_ || x > x1_ || y < y0_ || y > y1_)
        return false;

    /* Set hit record */
    rec.u_ = (x - x0_) / (x1_ - x0_);
    rec.v_ = (y - y0_) / (y1_ - y0_);
    rec.t_ = t;
    auto outward_normal = Vector3(0, 0, 1);
    rec.SetFaceNormal(r, outward_normal);
    rec.mat_ = mat_;
    rec.p_ = r.at(t);
    return true;
}

bool XYRect::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    // The bounding box must have non-zero width in each dimension, so pad the Z
    // dimension a small amount
    output_box = AABB(Point3(x0_, y0_, k_ - 0.0001), Point3(x1_, y1_, k_ + 0.0001));
    return true;
}


bool XZRect::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    auto t = (k_ - r.origin().y()) / r.direction().y();
    if (t < t_min || t > t_max)
        return false;

    auto x = r.origin().x() + t * r.direction().x();
    auto z = r.origin().z() + t * r.direction().z();
    if (x < x0_ || x > x1_ || z < z0_ || z > z1_)
        return false;

    rec.u_ = (x - x0_) / (x1_ - x0_);
    rec.v_ = (z - z0_) / (z1_ - z0_);
    rec.t_ = t;
    auto outward_normal = Vector3(0, 1, 0);
    rec.SetFaceNormal(r, outward_normal);
    rec.mat_ = mat_;
    rec.p_ = r.at(t);
    return true;
}

bool XZRect::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    // The bounding box must have non-zero width in each dimension, so pad the Y
    // dimension a small amount
    output_box = AABB(Point3(x0_, k_ - 0.0001, z0_), Point3(x1_, k_ + 0.0001, z1_));
    return true;
}


bool YZRect::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    auto t = (k_ - r.origin().x()) / r.direction().x();
    if (t < t_min || t > t_max)
        return false;

    auto y = r.origin().y() + t * r.direction().y();
    auto z = r.origin().z() + t * r.direction().z();
    if (y < y0_ || y > y1_ || z < z0_ || z > z1_)
        return false;

    rec.u_ = (y - y0_) / (y1_ - y0_);
    rec.v_ = (z - z0_) / (z1_ - z0_);
    rec.t_ = t;
    auto outward_normal = Vector3(1, 0, 0);
    rec.SetFaceNormal(r, outward_normal);
    rec.mat_ = mat_;
    rec.p_ = r.at(t);
    return true;
}

bool YZRect::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    // The bounding box must have non-zero width in each dimension, so pad the X
    // dimension a small amount
    output_box = AABB(Point3(k_ - 0.0001, y0_, z0_), Point3(k_ + 0.0001, y1_, z1_));
    return true;
}
