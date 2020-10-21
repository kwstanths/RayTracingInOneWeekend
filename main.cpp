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

HittableList random_scene() {
    HittableList world;

    auto ground_material = make_shared<Lambertian>(Color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(Point3(0, -1000, 0), 1000, ground_material));

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

int main() {

    /* Image parameters */
    const Real aspect_ratio = 16.0 / 9.0;
    const size_t image_width = 800;
    const size_t image_height = static_cast<size_t>(image_width / aspect_ratio);
    const int samples_per_pixel = 300;

    /* Computation parameters */
    const int max_depth = 50;
    const int threads = 4;
    
    /* Create the image data */
    std::shared_ptr<Vector3> image_data(new Vector3[image_width * image_height], std::default_delete<Vector3[]>());

    HittableList world = random_scene();

    Point3 lookfrom(13, 2, 13);
    Point3 lookat(0, 0, 0);
    Vector3 vup(0, 1, 0);
    auto dist_to_focus = 10.0;
    auto aperture = 0.1;

    Camera camera(lookfrom, lookat, vup, 20, aspect_ratio, aperture, dist_to_focus, 0.0, 0.0);

    std::atomic<int> lines_remaining = { (int)image_height };
#pragma omp parallel for num_threads(threads) shared(image_data) schedule(dynamic, 2)
    for (int j = image_height - 1; j >= 0; --j) {
        for (int i = 0; i < image_width; ++i) {
            Color pixel_color(0, 0, 0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width - 1);
                auto v = (j + random_double()) / (image_height - 1);
                Ray r = camera.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth);
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