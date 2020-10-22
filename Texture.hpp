#ifndef __Texture_hpp__
#define __Texture_hpp__

#include "Common.hpp"
#include "Vector3.hpp"
#include "Perlin.hpp"


class Texture {
public:
    virtual Color value(Real u, Real v, const Point3& p) const = 0;
};


class SolidColor : public Texture {
public:
    SolidColor() {}
    SolidColor(Color c) : color_value_(c) {}

    SolidColor(Real red, Real green, Real blue) : SolidColor(Color(red, green, blue)) {}

    virtual Color value(Real u, Real v, const Vector3& p) const override {
        return color_value_;
    }

private:
    Color color_value_;
};


class CheckerTexture : public Texture {
public:
    CheckerTexture() {};

    CheckerTexture(shared_ptr<Texture> t0, shared_ptr<Texture> t1)
        : even_color_(t0), odd_color_(t1) {}

    CheckerTexture(Color c1, Color c2)
        : even_color_(make_shared<SolidColor>(c1)), odd_color_(make_shared<SolidColor>(c2)) {}

    virtual Color value(double u, double v, const Point3& p) const override {
        auto sines = sin(10 * p.x())*sin(10 * p.y())*sin(10 * p.z());
        if (sines < 0)
            return odd_color_->value(u, v, p);
        else
            return even_color_->value(u, v, p);
    }

public:
    shared_ptr<Texture> odd_color_;
    shared_ptr<Texture> even_color_;
};


class NoiseTexture : public Texture {
public:
    NoiseTexture() {};
    NoiseTexture(Real scale) : scale_(scale) {};

    virtual Color value(Real u, Real v, const Point3& p) const override {
        return Color(1, 1, 1) * 0.5 * (1 + sin(scale_*p.z() + 10 * noise_.turb(p)));
    }

public:
    Perlin noise_;
    Real scale_;
};


class ImageTexture :public Texture {
public:
    const static int bytes_per_pixel = 3;

    ImageTexture(): data_(nullptr), width_(0), height_(0), bytes_per_scanline_(0) {}

    ImageTexture(const char* filename);

    ~ImageTexture() {
        delete data_;
    }

    virtual Color value(Real u, Real v, const Vector3& p) const override;

private:
    unsigned char *data_;
    int width_, height_;
    int bytes_per_scanline_;
};

#endif