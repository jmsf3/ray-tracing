#include <iostream>
#include "src/geometry/geometry.h"
#include "src/lib/ray.h"
#include "src/lib/point.h"
#include "src/lib/vector.h"
#include "src/scene/camera.h"

Geometry::Sphere sphere(Point(0.0f, 1.0f, -10.0f), 1.0f);
Geometry::Plane plane(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));

Vector color (const Ray& r)
{
    if (sphere.hit(r).hit)
    {
        return Vector(1.0f, 0.0f, 0.0f);
    }

    if (plane.hit(r).hit)
    {
        return Vector(0.0f, 1.0f, 0.0f);
    }

    Vector unit_direction = r.direction.normalized();
    float t = 0.5f * (unit_direction.y + 1.0f);

    return Vector(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector(0.5f, 0.7f, 1.0f) * t;
}

int main()
{
    Point center { 0.0f, 5.0f, 0.0f };
    Point target { 0.0f, 1.0f, -10.0f };
    Vector up { 0.0f, 1.0f, 0.0f };

    float vertical_fov = 90.0f * M_PI / 180.0f;
    uint32_t pixel_height = 720;
    uint32_t pixel_width = 1280;

    Camera camera { center, target, up, vertical_fov, pixel_height , pixel_width };
    std::cout << "P3\n" << pixel_width << " " << pixel_height << "\n255\n";

    for (int j = pixel_height - 1; j >= 0; j--)
    {
        for (int i = 0; i < pixel_width; i++)
        {
            Vector c = color(camera.cast_ray(i, j));

            int ir = static_cast<int>(255 * c.x);
            int ig = static_cast<int>(255 * c.y);
            int ib = static_cast<int>(255 * c.z);

            std::cout << ir << " " << ig << " " << ib << "\n";
        }
    }
}