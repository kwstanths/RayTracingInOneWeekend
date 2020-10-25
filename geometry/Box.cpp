#include "Box.hpp"

Box::Box(const Point3 & p0, const Point3 & p1, shared_ptr<Material> mat)
{
    box_min_ = p0;
    box_max_ = p1;

    sides_.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p1.z(), mat));
    sides_.add(make_shared<XYRect>(p0.x(), p1.x(), p0.y(), p1.y(), p0.z(), mat));

    sides_.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p1.y(), mat));
    sides_.add(make_shared<XZRect>(p0.x(), p1.x(), p0.z(), p1.z(), p0.y(), mat));

    sides_.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p1.x(), mat));
    sides_.add(make_shared<YZRect>(p0.y(), p1.y(), p0.z(), p1.z(), p0.x(), mat));
}

bool Box::hit(const Ray & r, Real t_min, Real t_max, HitRecord & rec) const
{
    return sides_.hit(r, t_min, t_max, rec);
}
