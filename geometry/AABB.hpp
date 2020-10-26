#ifndef __AABB_hpp__
#define __AABB_hpp__

#include "Common.hpp"
#include "math/Vector3.hpp"
#include "math/Ray.hpp"

class AABB {
public:
    AABB() {}
    
    AABB(const Point3& a, const Point3& b) { 
        min_ = a;
        max_ = b; 
    }

    Point3 min() const { 
        return min_; 
    }
    Point3 max() const { 
        return max_; 
    }

    bool hit(const Ray& r, Real tmin, Real tmax) const;

    static AABB surrounding_box(AABB box0, AABB box1);

    Point3 min_;
    Point3 max_;
};

#endif
