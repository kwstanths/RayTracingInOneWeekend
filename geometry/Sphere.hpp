#ifndef __Sphere_hpp__
#define __Sphere_hpp__

#include "math/Vector3.hpp"
#include "Hittable.hpp"
#include "Material.hpp"

/* 
    Get the uv coordiantes for a point on the surface of a unit sphere
*/
void GetSphereUV(const Vector3& p, Real& u, Real& v);

class Sphere : public Hittable {
public:
    Sphere() {}
    Sphere(Point3 cen, Real r, shared_ptr<Material> mat) : center_(cen), radius_(r), mat_(mat) {
    };

    virtual bool hit(
        const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;

    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

    virtual double pdf_value(const Point3& o, const Vector3& v) const override;

    virtual Vector3 random(const Vector3& o) const override;

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

    virtual bool hit(const Ray& r, Real tmin, Real tmax, HitRecord& rec) const override;
    
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

    Point3 center(Real time) const;

public:
    Point3 center0_, center1_;
    Real time0_, time1_;
    Real radius_;
    shared_ptr<Material> mat_;
};

#endif
