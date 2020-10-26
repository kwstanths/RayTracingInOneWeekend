#ifndef __PDF_hpp__
#define __PDF_hpp__

#include "Vector3.hpp"
#include "ONB.hpp"

#include "geometry/Hittable.hpp"

class PDF {
public:
    virtual ~PDF() {}

    virtual Real value(const Vector3& direction) const = 0;
    virtual Vector3 generate() const = 0;
};


class CosinePDF : public PDF {
public:
    CosinePDF(const Vector3& w) { 
        uvw.build_from_w(w); 
    }

    virtual Real value(const Vector3& direction) const override {
        auto cosine = dot(unit_vector(direction), uvw.w());
        return (cosine <= 0) ? 0 : cosine / pi;
    }

    virtual Vector3 generate() const override {
        return uvw.local(random_cosine_direction());
    }

public:
    ONB uvw;
};


class HittablePDF : public PDF {
public:
    HittablePDF(shared_ptr<Hittable> p, const Point3& origin) :ptr_(p), o_(origin) {};

    virtual Real value(const Vector3& direction) const override {
        return ptr_->pdf_value(o_, direction);
    }

    virtual Vector3 generate() const override {
        return ptr_->random(o_);
    }

public:
    Point3 o_;
    shared_ptr<Hittable> ptr_;
};


class MixturePDF : public PDF {
public:
    MixturePDF(shared_ptr<PDF> p1, shared_ptr<PDF> p2) {
        p_[0] = p1;
        p_[1] = p2;
    };

    virtual Real value(const Vector3& direction) const override {
        return 0.5 * p_[0]->value(direction) + 0.5 *p_[1]->value(direction);
    }

    virtual Vector3 generate() const override {
        if (random_double() < 0.5)
            return p_[0]->generate();
        else
            return p_[1]->generate();
    }

public:
    shared_ptr<PDF> p_[2];
};

#endif