#ifndef __AARect_hpp__
#define __AARect_hpp__

#include "Common.hpp"
#include "Hittable.hpp"
#include "Material.hpp"

class XYRect : public Hittable {
public:
    XYRect() {};
    XYRect(Real x0, Real x1, Real y0, Real y1, Real k, shared_ptr<Material> mat) : x0_(x0), x1_(x1), y0_(y0), y1_(y1), k_(k), mat_(mat) {};

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

public:
    shared_ptr<Material> mat_;
    Real x0_, x1_, y0_, y1_, k_;
};


class XZRect : public Hittable {
public:
    XZRect() {};
    XZRect(Real x0, Real x1, Real z0, Real z1, Real k, shared_ptr<Material> mat) : x0_(x0), x1_(x1), z0_(z0), z1_(z1), k_(k), mat_(mat) {};

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

public:
    shared_ptr<Material> mat_;
    Real x0_, x1_, z0_, z1_, k_;
};


class YZRect : public Hittable {
public:
    YZRect() {};
    YZRect(Real y0, Real y1, Real z0, Real z1, Real k, shared_ptr<Material> mat) : y0_(y0), y1_(y1), z0_(z0), z1_(z1), k_(k), mat_(mat) {};

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override;

public:
    shared_ptr<Material> mat_;
    Real y0_, y1_, z0_, z1_, k_;
};

#endif