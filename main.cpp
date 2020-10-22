#include <iostream>
#include <fstream>
#include <vector>
#include <limits>
#include <atomic>
#include <omp.h>

#include "Common.hpp"
#include "Color.hpp"
#include "HittableList.hpp"
#include "Sphere.hpp"
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

Color ray_color(const Ray& r, const Hittable& world, int depth) {
    if (depth <= 0)
        return Color(0, 0, 0);

    HitRecord hit;
    if (world.hit(r, 0.001, infinity, hit)) {
        Ray scattered;
        Color attenuation;
        if (hit.mat_->scatter(r, hit, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1);
        return Color(0, 0, 0);
    }
    
    Vector3 unit_direction = unit_vector(r.direction());
    auto t = 0.5*(unit_direction.y() + 1.0);
    return (1.0 - t)*Color(1.0, 1.0, 1.0) + t * Color(0.5, 0.7, 1.0);
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

int main() {

    /* Image parameters */
    const Real aspect_ratio = 16.0 / 9.0;
    const size_t image_width = 256;
    const size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
    const int samples_per_pixel = 100;
    /* Create the image data */
    std::shared_ptr<Vector3> image_data(new Vector3[image_width * image_height], std::default_delete<Vector3[]>());

    /* Computation parameters */
    const int max_depth = 50;
    const int threads = 4;

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

    switch (0) {
    case 1:
        world = random_scene();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        aperture = 0.1;
        break;
    case 2:
        world = two_spheres();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    case 3:
        world = two_perlin_spheres();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    default:
    case 4:
        world = earth();
        lookfrom = Point3(13, 2, 3);
        lookat = Point3(0, 0, 0);
        vfov = 20.0;
        break;
    }
    /* Create a BVH out of the world crated */
    BVHNode tree(world, time_start, time_end);

    Camera camera(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, time_start, time_end);

    std::atomic<int> lines_remaining = { (int)image_height };
#pragma omp parallel for num_threads(threads) shared(image_data) schedule(dynamic, 2)
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, tree, max_depth);
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