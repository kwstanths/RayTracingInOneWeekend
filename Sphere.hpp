#ifndef __Sphere_hpp__
#define __Sphere_hpp__

#include "Hittable.hpp"
#include "Vector3.hpp"
#include "Material.hpp"

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, Real r, shared_ptr<Material> mat) : center_(cen), radius_(r), mat_(mat) {
    };

    virtual bool hit(
        const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;

public:
    Point3 center_;
    Real radius_;
    shared_ptr<Material> mat_;
};

class MovingSphere : public Hittable {
public:
    MovingSphere() {}
    MovingSphere(
        Point3 cen0, Point3 cen1, Real t0, Real t1, Real r, shared_ptr<Material> m)
        : center0_(cen0), center1_(cen1), time0_(t0), time1_(t1), radius_(r), mat_(m)
    {};

    virtual bool hit(
        const Ray& r, Real tmin, Real tmax, HitRecord& rec) const override;

    Point3 center(Real time) const;

public:
    Point3 center0_, center1_;
    Real time0_, time1_;
    Real radius_;
    shared_ptr<Material> mat_;
};

#endif
