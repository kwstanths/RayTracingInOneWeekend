#ifndef __Perlin_hpp__
#define __Perlin_hpp__

#include "Common.hpp"
#include "Vector3.hpp"

class Perlin {
public:
    Perlin();

    ~Perlin() {
        delete[] randvec;
        delete[] perm_x;
        delete[] perm_y;
        delete[] perm_z;
    }

    Real noise(const Point3& p) const;

    Real turb(const Point3& p, int depth = 7) const;

private:
    static const int point_count = 256;
    Vector3 * randvec;
    int * perm_x;
    int * perm_y;
    int * perm_z;

    static int* perlin_generate_perm();

    static void permute(int* p, int n);

    static Real trilinear_interp(Real c[2][2][2], Real u, Real v, Real w);
    static Real perlin_interp(Vector3 c[2][2][2], Real u, Real v, Real w);
};

#endif