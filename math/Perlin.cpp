#include "Perlin.hpp"

Perlin::Perlin()
{
    randvec = new Vector3[point_count];
    for (int i = 0; i < point_count; ++i) {
        randvec[i] = unit_vector(Vector3::random(-1, 1));
    }

    perm_x = perlin_generate_perm();
    perm_y = perlin_generate_perm();
    perm_z = perlin_generate_perm();
}

Real Perlin::noise(const Point3 & p) const
{
    auto u = p.x() - floor(p.x());
    auto v = p.y() - floor(p.y());
    auto w = p.z() - floor(p.z());
    auto i = static_cast<int>(floor(p.x()));
    auto j = static_cast<int>(floor(p.y()));
    auto k = static_cast<int>(floor(p.z()));
    Vector3 c[2][2][2];

    for (int di = 0; di < 2; di++)
        for (int dj = 0; dj < 2; dj++)
            for (int dk = 0; dk < 2; dk++)
                c[di][dj][dk] = randvec[
                    perm_x[(i + di) & 255] ^
                        perm_y[(j + dj) & 255] ^
                        perm_z[(k + dk) & 255]
                ];

    return perlin_interp(c, u, v, w);
}

Real Perlin::turb(const Point3 & p, int depth) const
{
    auto accum = 0.0;
    auto temp_p = p;
    auto weight = 1.0;

    for (int i = 0; i < depth; i++) {
        accum += weight * noise(temp_p);
        weight *= 0.5;
        temp_p *= 2;
    }

    return fabs(accum);
}

int * Perlin::perlin_generate_perm()
{
    auto p = new int[point_count];

    for (int i = 0; i < Perlin::point_count; i++)
        p[i] = i;

    permute(p, point_count);

    return p;
}

void Perlin::permute(int * p, int n)
{
    for (int i = n - 1; i > 0; i--) {
        int target = random_int(0, i);
        int tmp = p[i];
        p[i] = p[target];
        p[target] = tmp;
    }
}

Real Perlin::trilinear_interp(Real c[2][2][2], Real u, Real v, Real w)
{
    auto accum = 0.0;
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++)
                accum += (i*u + (1 - i)*(1 - u))*
                (j*v + (1 - j)*(1 - v))*
                (k*w + (1 - k)*(1 - w))*c[i][j][k];

    return accum;
}

Real Perlin::perlin_interp(Vector3 c[2][2][2], Real u, Real v, Real w)
{
    auto uu = u * u*(3 - 2 * u);
    auto vv = v * v*(3 - 2 * v);
    auto ww = w * w*(3 - 2 * w);
    auto accum = 0.0;

    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            for (int k = 0; k < 2; k++) {
                Vector3 weight_v(u - i, v - j, w - k);
                accum += (i*uu + (1 - i)*(1 - uu))
                    * (j*vv + (1 - j)*(1 - vv))
                    * (k*ww + (1 - k)*(1 - ww))
                    * dot(c[i][j][k], weight_v);
            }

    return accum;
}
