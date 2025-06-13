#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "src/geometry/geometry.h"
#include "src/lib/ray.h"
#include "src/lib/point.h"
#include "src/lib/vector.h"
#include "src/scene/camera.h"

template <typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

Geometry::Sphere sphere(Point(0.0f, 1.0f, -10.0f), 1.0f);
Geometry::Plane plane(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));

Vector color(const Ray& ray)
{
    if (sphere.hit(ray).hit)
    {
        return Vector(1.0f, 0.0f, 0.0f);
    }

    if (plane.hit(ray).hit)
    {
        return Vector(0.0f, 1.0f, 0.0f);
    }

    Vector unit_direction = ray.direction.normalized();
    float t = 0.5f * (unit_direction.y + 1.0f);

    return Vector(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector(0.5f, 0.7f, 1.0f) * t;
}

void render_scene(const Camera& camera, const std::string& filename, uint32_t image_width, uint32_t image_height)
{
    std::ofstream image(filename);
    if (!image)
    {
        std::cerr << "Error creating " << filename << "\n";
        return;
    }

    image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            Vector pixel_color = color(camera.cast_ray(i, j));

            int red   = static_cast<int>(255.99f * clamp(pixel_color.x, 0.0f, 1.0f));
            int green = static_cast<int>(255.99f * clamp(pixel_color.y, 0.0f, 1.0f));
            int blue  = static_cast<int>(255.99f * clamp(pixel_color.z, 0.0f, 1.0f));

            image << red << " " << green << " " << blue << "\n";
        }
    }

    image.close();
    std::cout << "Image saved to " << filename << "\n";
}

int main()
{
    Point camera_position { 0.0f, 5.0f, 0.0f };
    Point look_at { 0.0f, 1.0f, -10.0f };
    Vector up_vector { 0.0f, 1.0f, 0.0f };

    float vertical_fov = 90.0f * M_PI / 180.0f;

    uint32_t image_height = 720;
    uint32_t image_width = 1280;

    Camera camera { camera_position, look_at, up_vector, vertical_fov, image_height, image_width };

    render_scene(camera, "output.ppm", image_width, image_height);

    return 0;
}
