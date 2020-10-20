#include "Camera.hpp"

Camera::Camera(Point3 lookfrom, Point3 lookat, Vector3 vup, Real vfov, Real aspect_ratio, Real aperture, Real focus_dist)
{
    auto theta = degrees_to_radians(vfov);
    auto h = tan(theta / 2);
    auto viewport_height = 2.0 * h;
    auto viewport_width = aspect_ratio * viewport_height;
    auto focal_length = 1.0;

    w_ = unit_vector(lookfrom - lookat);
    u_ = unit_vector(cross(vup, w_));
    v_ = cross(w_, u_);

    origin_ = lookfrom;
    horizontal_ = focus_dist * viewport_width * u_;
    vertical_ = focus_dist * viewport_height * v_;
    lower_left_corner_ = origin_ - horizontal_ / 2 - vertical_ / 2 - focus_dist * w_;

    lens_radius_ = aperture / 2;
}

Ray Camera::get_ray(Real s, Real t) const
{
    Vector3 rd = lens_radius_ * random_in_unit_disk();
    Vector3 offset = u_ * rd.x() + v_ * rd.y();

    return Ray(
        origin_ + offset,
        lower_left_corner_ + s * horizontal_ + t * vertical_ - origin_ - offset
    );
}
