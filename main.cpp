#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <atomic>
#include <omp.h>

#include "Common.hpp"
#include "Color.hpp"
#include "geometry/HittableList.hpp"
#include "geometry/Sphere.hpp"
#include "geometry/AARect.hpp"
#include "geometry/Box.hpp"
#include "geometry/ConstantMedium.hpp"
#include "Camera.hpp"
#include "Material.hpp"
#include "BVH.hpp"

/* Write an image to the disk */
void CreateImage(std::shared_ptr<Vector3> data, const std::string& file_name, int width, int height, int samples_per_pixel) {
    
    std::cout << "Writing image file... " << std::flush;
    std::ofstream file;
    file.open(file_name);
    
    file << "P3\n" << width << ' ' << height << "\n255\n";

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) {
            write_color(file, data.get()[j * width + i], samples_per_pixel);
        }
    }

    file.close();
    std::cout << "DONE" << std::endl;
}

Color ray_color(const Ray& r, const Color& background, const Hittable& world, int depth) {
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord hit;
    if (!world.hit(r, 0.001, infinity, hit)) {
        return background;
    }
    
    Ray scattered;
    Color attenuation;
    Color emitted = hit.mat_->emitted(hit.u_, hit.v_, hit.p_);

    if (!hit.mat_->scatter(r, hit, attenuation, scattered))
        return emitted;
    
    return emitted + attenuation * ray_color(scattered, background, world, depth - 1);
}

HittableList two_spheres() {
    HittableList objects;

    auto checker_texture = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));

    objects.add(make_shared<Sphere>(Point3(0, -10, 0), 10, make_shared<Lambertian>(checker_texture)));
    objects.add(make_shared<Sphere>(Point3(0, 10, 0), 10, make_shared<Lambertian>(checker_texture)));

    return objects;
}

HittableList random_scene() {
    HittableList world;

    auto checker_texture = make_shared<CheckerTexture>(Color(0.2, 0.3, 0.1), Color(0.9, 0.9, 0.9));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(checker_texture)));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            Point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - Point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = Color::random() * Color::random();
                    sphere_material = make_shared<Lambertian>(albedo);
                    auto center2 = center + Vector3(0, random_double(0, .5), 0);
                    world.add(make_shared<MovingSphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                }
                else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = Color::random(0.5, 1.0);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<Metal>(albedo, fuzz);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
                else {
                    // glass
                    sphere_material = make_shared<Dielectric>(1.5);
                    world.add(make_shared<Sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }

    auto material1 = make_shared<Dielectric>(1.5);
    world.add(make_shared<Sphere>(Point3(0, 1, 0), 1.0, material1));

    auto material2 = make_shared<Lambertian>(Color(0.4, 0.2, 0.1));
    world.add(make_shared<Sphere>(Point3(-4, 1, 0), 1.0, material2));

    auto material3 = make_shared<Metal>(Color(0.7, 0.6, 0.5), 0.0);
    world.add(make_shared<Sphere>(Point3(4, 1, 0), 1.0, material3));

    return world;
}

HittableList two_perlin_spheres() {
    HittableList objects;

    auto pertext = make_shared<NoiseTexture>(16);
    objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    return objects;
}

HittableList earth() {
    auto earth_texture = make_shared<ImageTexture>("earthmap.jpg");
    auto earth_surface = make_shared<Lambertian>(earth_texture);
    auto globe = make_shared<Sphere>(Point3(0, 0, 0), 2, earth_surface);

    return HittableList(globe);
}

HittableList simple_lights() {
    HittableList objects;

    auto pertext = make_shared<NoiseTexture>(4);
    objects.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, make_shared<Lambertian>(pertext)));
    objects.add(make_shared<Sphere>(Point3(0, 2, 0), 2, make_shared<Lambertian>(pertext)));

    auto difflight = make_shared<DiffuseLight>(Color(4, 4, 4));
    objects.add(make_shared<XYRect>(3, 5, 1, 3, -2, difflight));

    auto difflight_2 = make_shared<DiffuseLight>(Color(4, 0, 0));
    objects.add(make_shared<Sphere>(Point3(0, 6, 0), 1.5, difflight_2));

    return objects;
}

HittableList cornell_box() {
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(15, 15, 15));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRect>(213, 343, 227, 332, 554, light));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));
    objects.add(box1);

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));
    objects.add(box2);

    return objects;
}

HittableList cornell_smoke() {
    HittableList objects;

    auto red = make_shared<Lambertian>(Color(.65, .05, .05));
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    auto green = make_shared<Lambertian>(Color(.12, .45, .15));
    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));

    objects.add(make_shared<YZRect>(0, 555, 0, 555, 555, green));
    objects.add(make_shared<YZRect>(0, 555, 0, 555, 0, red));
    objects.add(make_shared<XZRect>(113, 443, 127, 432, 554, light));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 0, white));
    objects.add(make_shared<XZRect>(0, 555, 0, 555, 555, white));
    objects.add(make_shared<XYRect>(0, 555, 0, 555, 555, white));

    shared_ptr<Hittable> box1 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 330, 165), white);
    box1 = make_shared<RotateY>(box1, 15);
    box1 = make_shared<Translate>(box1, Vector3(265, 0, 295));

    shared_ptr<Hittable> box2 = make_shared<Box>(Point3(0, 0, 0), Point3(165, 165, 165), white);
    box2 = make_shared<RotateY>(box2, -18);
    box2 = make_shared<Translate>(box2, Vector3(130, 0, 65));

    objects.add(make_shared<ConstantMedium>(box1, 0.01, Color(0, 0, 0)));
    objects.add(make_shared<ConstantMedium>(box2, 0.01, Color(1, 1, 1)));

    //HittableList scene;
    //scene.add(make_shared<BVHNode>(objects));
    return objects;
}

HittableList final_scene() {
    HittableList boxes1;
    auto ground = make_shared<Lambertian>(Color(0.48, 0.83, 0.53));

    const int boxes_per_side = 20;
    for (int i = 0; i < boxes_per_side; i++) {
        for (int j = 0; j < boxes_per_side; j++) {
            auto w = 100.0;
            auto x0 = -1000.0 + i * w;
            auto z0 = -1000.0 + j * w;
            auto y0 = 0.0;
            auto x1 = x0 + w;
            auto y1 = random_double(1, 101);
            auto z1 = z0 + w;

            boxes1.add(make_shared<Box>(Point3(x0, y0, z0), Point3(x1, y1, z1), ground));
        }
    }

    HittableList objects;

    objects.add(make_shared<BVHNode>(boxes1, 0, 1));

    auto light = make_shared<DiffuseLight>(Color(7, 7, 7));
    objects.add(make_shared<XZRect>(123, 423, 147, 412, 554, light));

    auto center1 = Point3(400, 400, 200);
    auto center2 = center1 + Vector3(30, 0, 0);
    auto moving_sphere_material = make_shared<Lambertian>(Color(0.7, 0.3, 0.1));
    objects.add(make_shared<MovingSphere>(center1, center2, 0, 1, 50, moving_sphere_material));

    objects.add(make_shared<Sphere>(Point3(260, 150, 45), 50, make_shared<Dielectric>(1.5)));
    objects.add(make_shared<Sphere>(
        Point3(0, 150, 145), 50, make_shared<Metal>(Color(0.8, 0.8, 0.9), 1.0)
        ));

    auto boundary = make_shared<Sphere>(Point3(360, 150, 145), 70, make_shared<Dielectric>(1.5));
    objects.add(boundary);
    objects.add(make_shared<ConstantMedium>(boundary, 0.2, Color(0.2, 0.4, 0.9)));
    boundary = make_shared<Sphere>(Point3(0, 0, 0), 5000, make_shared<Dielectric>(1.5));
    objects.add(make_shared<ConstantMedium>(boundary, .0001, Color(1, 1, 1)));

    auto emat = make_shared<Lambertian>(make_shared<ImageTexture>("earthmap.jpg"));
    objects.add(make_shared<Sphere>(Point3(400, 200, 400), 100, emat));
    auto pertext = make_shared<NoiseTexture>(0.1);
    objects.add(make_shared<Sphere>(Point3(220, 280, 300), 80, make_shared<Lambertian>(pertext)));

    HittableList boxes2;
    auto white = make_shared<Lambertian>(Color(.73, .73, .73));
    int ns = 1000;
    for (int j = 0; j < ns; j++) {
        boxes2.add(make_shared<Sphere>(Point3::random(0, 165), 10, white));
    }

    objects.add(make_shared<Translate>(
        make_shared<RotateY>(
            make_shared<BVHNode>(boxes2, 0.0, 1.0), 15),
        Vector3(-100, 270, 395)
        )
    );

    return objects;
}

int main() {

    /* Default image parameters */
    Real aspect_ratio = 16.0 / 9.0;
    size_t image_width = 256;
    int samples_per_pixel = 100;
    
    /* Computation parameters */
    const int max_depth = 50;
    const int threads = 5;

    /* Scene and camera parameters */
    HittableList world;
    Point3 lookfrom;
    Point3 lookat;
    auto aperture = 0.1;
    auto vfov = 40.0;
    size_t time_start = 0;
    size_t time_end = 0;
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    Color background(0, 0, 0);

    switch (0) {
    case 1:
        world = random_scene();
        lookfrom = Point3(13, 2, 3);
        background = Color(0.70, 0.80, 1.00);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break;
    case 2:
        world = two_spheres();
        background = Color(0.70, 0.80, 1.00);
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        background = Color(0.70, 0.80, 1.00);
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 4:
        world = earth();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 5:
        world = simple_lights();
        samples_per_pixel = 400;
        background = Color(0, 0, 0);
        lookfrom = Point3(26, 3, 6);
        lookat = Point3(0, 2, 0);
        vfov = 20.0;
        break;
    case 6:
        world = cornell_box();
        aspect_ratio = 1.0;
        image_width = 256;
        samples_per_pixel = 100;
        background = Color(0, 0, 0);
        lookfrom = Point3(278, 278, -800);
        lookat = Point3(278, 278, 0);
        vfov = 40.0;
        break;
    case 7:
        world = cornell_smoke();
        aspect_ratio = 1.0;
        image_width = 256;
        samples_per_pixel = 100;
        lookfrom = Point3(278, 278, -800);
        lookat = Point3(278, 278, 0);
        vfov = 40.0;
        break;
    default:
    case 8:
        world = final_scene();
        aspect_ratio = 1.0;
        image_width = 256;
        samples_per_pixel = 10000;
        background = Color(0, 0, 0);
        lookfrom = Point3(478, 278, -600);
        lookat = Point3(278, 278, 0);
        vfov = 40.0;
        time_start = 0;
        time_end = 1;
        break;
    }

    /* Create a camera */
    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time_start, time_end);

    /* Create the image data */
    size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
    std::shared_ptr<Vector3> image_data(new Vector3[image_width * image_height], std::default_delete<Vector3[]>());

    std::atomic<int> lines_remaining = { (int)image_height };
#pragma omp parallel for num_threads(threads) shared(image_data) schedule(dynamic, 2)
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, background, world, max_depth);
            }
            
            image_data.get()[j * image_width + i] = pixel_color;
        }
        std::atomic_fetch_sub(&lines_remaining, 1);
        /* First thread, report progress */
        if (omp_get_thread_num() == 0) std::cerr << "\rImage lines remaining: " << lines_remaining.load() << ' ' << std::flush;
    }
    std::cerr << std::endl;

    CreateImage(image_data, "test.ppm", image_width, image_height, samples_per_pixel);

    system("pause");
}