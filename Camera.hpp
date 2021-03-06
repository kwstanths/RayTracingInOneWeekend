#ifndef __Camera_hpp__
#define __Camera_hpp__

#include "math/Vector3.hpp"
#include "math/Ray.hpp"

class Camera {
public:
    Camera(Point3 lookfrom, Point3 lookat, Vector3 vup, Real vfov, Real aspect_ratio, Real aperture, Real focus_dist, Real time_start, Real time_end);

    Ray get_ray(Real s, Real t) const;

private:
    Point3 origin_;
    Point3 lower_left_corner_;
    Vector3 horizontal_;
    Vector3 vertical_;
    Vector3 u_, v_, w_;
    Real lens_radius_;
    Real time_start_, time_end_;
};

#endif