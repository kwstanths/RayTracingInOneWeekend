#ifndef __Ray_hpp__
#define __Ray_hpp__

#include "Vector3.hpp"

class Ray {
public:
    Ray() {}
    Ray(const Point3& origin, const Vector3& direction, Real time = 0)
        : origin_(origin), direction_(direction), time_(time)
    {
    }

    Point3 origin() const { 
        return origin_;
    }
    Vector3 direction() const { 
        return direction_;
    }
    Real Time() const {
        return time_;
    }

    Point3 at(Real t) const {
        return origin_ + t * direction_;
    }

public:
    Point3 origin_;
    Vector3 direction_;
    Real time_;
};

#endif
