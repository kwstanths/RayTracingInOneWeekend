#include "Hittable.hpp"

bool Translate::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    Ray moved_r(r.origin() - offset_, r.direction(), r.Time());
    if (!ptr_->hit(moved_r, t_min, t_max, rec))
        return false;

    rec.p_ += offset_;
    rec.SetFaceNormal(moved_r, rec.normal_);

    return true;
}

bool Translate::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    if (!ptr_->bounding_box(t0, t1, output_box))
        return false;

    output_box = AABB(
        output_box.min() + offset_,
        output_box.max() + offset_);

    return true;
}

RotateY::RotateY(shared_ptr<Hittable> p, const Real angle) : ptr_(p) {
    auto radians = degrees_to_radians(angle);
    sin_theta_ = sin(radians);
    cos_theta_ = cos(radians);
    hasbox_ = ptr_->bounding_box(0, 1, bbox_);

    Point3 min(infinity, infinity, infinity);
    Point3 max(-infinity, -infinity, -infinity);

    for (int i = 0; i < 2; i++) {
        for (int j = 0; j < 2; j++) {
            for (int k = 0; k < 2; k++) {
                /* Get all the combinations between the min and max of the points of the bounding box */
                auto x = i * bbox_.max().x() + (1 - i)*bbox_.min().x();
                auto y = j * bbox_.max().y() + (1 - j)*bbox_.min().y();
                auto z = k * bbox_.max().z() + (1 - k)*bbox_.min().z();

                auto newx = cos_theta_ * x + sin_theta_ * z;
                auto newz = -sin_theta_ * x + cos_theta_ * z;

                /* The rotated point of the bounding box */
                Vector3 temp(newx, y, newz);

                /* Calculate min and max */
                for (int c = 0; c < 3; c++) {
                    min[c] = fmin(min[c], temp[c]);
                    max[c] = fmax(max[c], temp[c]);
                }
            }
        }
    }

    bbox_ = AABB(min, max);
}

bool RotateY::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    auto origin = r.origin();
    auto direction = r.direction();

    origin[0] = cos_theta_ * r.origin()[0] - sin_theta_ * r.origin()[2];
    origin[2] = sin_theta_ * r.origin()[0] + cos_theta_ * r.origin()[2];

    direction[0] = cos_theta_ * r.direction()[0] - sin_theta_ * r.direction()[2];
    direction[2] = sin_theta_ * r.direction()[0] + cos_theta_ * r.direction()[2];

    Ray rotated_r(origin, direction, r.Time());

    if (!ptr_->hit(rotated_r, t_min, t_max, rec))
        return false;

    auto p = rec.p_;
    auto normal = rec.normal_;

    p[0] = cos_theta_ * rec.p_[0] + sin_theta_ * rec.p_[2];
    p[2] = -sin_theta_ * rec.p_[0] + cos_theta_ * rec.p_[2];

    normal[0] = cos_theta_ * rec.normal_[0] + sin_theta_ * rec.normal_[2];
    normal[2] = -sin_theta_ * rec.normal_[0] + cos_theta_ * rec.normal_[2];

    rec.p_ = p;
    rec.SetFaceNormal(rotated_r, normal);

    return true;
}

bool RotateY::bounding_box(Real t0, Real t1, AABB & output_box) const
{
    output_box = bbox_;
    return hasbox_;
}
