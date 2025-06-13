#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#include <iomanip>

#include "src/geometry/geometry.h"
#include "src/lib/ray.h"
#include "src/lib/point.h"
#include "src/lib/vector.h"
#include "src/scene/camera.h"

namespace fs = std::filesystem;

// Function to return the color of the ray
Vector color(const Ray& r, const Geometry::Sphere& sphere, const Geometry::Plane& plane)
{
    if (sphere.hit(r).hit)
        return Vector(1.0f, 0.0f, 0.0f);  // red

    if (plane.hit(r).hit)
        return Vector(0.0f, 1.0f, 0.0f);  // green

    Vector unit_direction = r.direction.normalized();
    float t = 0.5f * (unit_direction.y + 1.0f);
    return Vector(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector(0.5f, 0.7f, 1.0f) * t;
}

int main()
{
    uint32_t pixel_height = 720;
    uint32_t pixel_width = 1280;
    Geometry::Plane plane(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));

    // Create the output folder if it doesn't exist
    std::string output_dir = "output_images";
    fs::create_directories(output_dir);

    int image_counter = 1;
    char keep_going = 'Y';

    while (keep_going == 'Y' || keep_going == 'y')
    {
        float sx, sy, sz;
        std::cout << "Enter sphere position (x y z): ";
        std::cin >> sx >> sy >> sz;
        Geometry::Sphere sphere(Point(sx, sy, sz), 1.0f);

        float cx, cy, cz;
        std::cout << "Enter camera position (x y z): ";
        std::cin >> cx >> cy >> cz;
        Point center(cx, cy, cz);

        float tx, ty, tz;
        std::cout << "Enter target position (x y z): ";
        std::cin >> tx >> ty >> tz;
        Point target(tx, ty, tz);

        Vector up(0.0f, 1.0f, 0.0f);
        float vertical_fov = 90.0f * M_PI / 180.0f;
        Camera camera(center, target, up, vertical_fov, pixel_height, pixel_width);

        // Image filename
        std::ostringstream filename;
        filename << output_dir << "/image" << image_counter << ".ppm";

        std::ofstream image_file(filename.str());
        image_file << "P3\n" << pixel_width << " " << pixel_height << "\n255\n";

        for (int j = pixel_height - 1; j >= 0; j--)
        {
            for (int i = 0; i < pixel_width; i++)
            {
                Vector c = color(camera.cast_ray(i, j), sphere, plane);
                int ir = static_cast<int>(255.99f * (c.x < 0.0f ? 0.0f : (c.x > 1.0f ? 1.0f : c.x)));
                int ig = static_cast<int>(255.99f * (c.y < 0.0f ? 0.0f : (c.y > 1.0f ? 1.0f : c.y)));
                int ib = static_cast<int>(255.99f * (c.z < 0.0f ? 0.0f : (c.z > 1.0f ? 1.0f : c.z)));
                image_file << ir << " " << ig << " " << ib << "\n";
            }
        }

        image_file.close();
        std::cout << "Image saved as " << filename.str() << "\n";

        std::cout << "Do you want to generate another? (Y/N): ";
        std::cin >> keep_going;
        image_counter++;
    }

    return 0;
}
