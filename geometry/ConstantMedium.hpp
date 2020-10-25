#ifndef __ConstantMedium_hpp__
#define __ConstantMedium_hpp__

#include "Common.hpp"
#include "Hittable.hpp"
#include "Texture.hpp"
#include "Material.hpp"

class ConstantMedium : public Hittable {
public:
    ConstantMedium(shared_ptr<Hittable> b, Real d, shared_ptr<Texture> a) : boundary_(b), neg_inv_density_(-1 / d), phase_function_(make_shared<Isotropic>(a)) {}

    ConstantMedium(shared_ptr<Hittable> b, Real d, Color c) : boundary_(b), neg_inv_density_(-1 / d), phase_function_(make_shared<Isotropic>(c)) {}

    virtual bool hit(const Ray& r, Real t_min, Real t_max, HitRecord& rec) const override;
    
    virtual bool bounding_box(Real t0, Real t1, AABB& output_box) const override {
        return boundary_->bounding_box(t0, t1, output_box);
    }

public:
    shared_ptr<Hittable> boundary_;
    shared_ptr<Material> phase_function_;
    Real neg_inv_density_;
};

#endif