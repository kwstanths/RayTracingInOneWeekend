#ifndef __Common_hpp__
#define __Common_hpp__

#include <cmath>
#include <limits>
#include <memory>
#include <random>

#include "math/Real.hpp"

using std::shared_ptr;
using std::make_shared;

const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

inline Real degrees_to_radians(Real degrees)
{
    return degrees * pi / 180.0;
}

inline Real random_double()
{
    static std::uniform_real_distribution<Real> distribution(0.0, 1.0);
    static std::mt19937 generator;
    return distribution(generator);
}

inline Real random_double(Real min, Real max)
{
    return min + (max - min)*random_double();
}

inline int random_int(int min, int max) {
    return static_cast<int>(random_double(min, max + 1));
}

inline Real clamp(Real x, Real min, Real max)
{
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

#endif