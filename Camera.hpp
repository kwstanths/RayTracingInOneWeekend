#ifndef __Camera_hpp__
#define __Camera_hpp__

#include "Vector3.hpp"
#include "Ray.hpp"

class Camera {
public:
    Camera(Point3 lookfrom, Point3 lookat, Vector3 vup, Real vfov, Real aspect_ratio, Real aperture, Real focus_dist);

    Ray get_ray(Real s, Real t) const;

private:
    Point3 origin_;
    Point3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
    Vector3 u_, v_, w_;
    Real lens_radius_;
};

#endif