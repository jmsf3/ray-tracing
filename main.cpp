#include <iostream>
#include <fstream>
#include <filesystem>
#include <string>
#include <sstream>
#define _USE_MATH_DEFINES
#include <cmath>
#include "src/geometry/geometry.h"
#include "src/lib/ray.h"
#include "src/lib/point.h"
#include "src/lib/vector.h"
#include "src/scene/camera.h"

namespace fs = std::filesystem;

template <typename T>
T clamp(T value, T min, T max) {
    return (value < min) ? min : (value > max) ? max : value;
}

Vector color(const Ray& ray, const Geometry::Sphere& sphere, const Geometry::Plane& plane)
{
    if (sphere.hit(ray).hit)
        return Vector(1.0f, 0.0f, 0.0f);

    if (plane.hit(ray).hit)
        return Vector(0.0f, 1.0f, 0.0f);

    Vector unit_direction = ray.direction.normalized();
    float t = 0.5f * (unit_direction.y + 1.0f);
    return Vector(1.0f, 1.0f, 1.0f) * (1.0f - t) + Vector(0.5f, 0.7f, 1.0f) * t;
}

void render_scene(const Camera& camera, const std::string& filename, const Geometry::Sphere& sphere, const Geometry::Plane& plane)
{
    std::ofstream image(filename);
    if (!image)
    {
        std::cerr << "Error creating " << filename << "\n";
        return;
    }

    uint32_t image_width = camera.get_pixel_width();
    uint32_t image_height = camera.get_pixel_height();

    image << "P3\n" << image_width << " " << image_height << "\n255\n";

    for (int j = image_height - 1; j >= 0; --j)
    {
        for (int i = 0; i < image_width; ++i)
        {
            Vector pixel_color = color(camera.cast_ray(i, j), sphere, plane);

            int red   = static_cast<int>(255.99f * clamp(pixel_color.x, 0.0f, 1.0f));
            int green = static_cast<int>(255.99f * clamp(pixel_color.y, 0.0f, 1.0f));
            int blue  = static_cast<int>(255.99f * clamp(pixel_color.z, 0.0f, 1.0f));

            image << red << " " << green << " " << blue << "\n";
        }
    }

    image.close();
    std::cout << "Image saved to " << filename << "\n";
}

void ensure_output_folder(const std::string& folder) {
    if (!fs::exists(folder)) {
        fs::create_directory(folder);
        std::cout << "Created output folder: " << folder << "\n";
    }

    std::ofstream gitignore(".gitignore", std::ios::app);
    std::ifstream check(".gitignore");
    std::string line;
    bool already_ignored = false;

    while (getline(check, line)) {
        if (line == folder + "/") {
            already_ignored = true;
            break;
        }
    }

    if (!already_ignored) {
        gitignore << folder << "/" << "\n";
        std::cout << "Added '" << folder << "/' to .gitignore\n";
    }
}

int main()
{
    const std::string output_folder = "imagens_output";
    ensure_output_folder(output_folder);

    uint32_t image_height = 720;
    uint32_t image_width = 1280;
    Geometry::Plane plane(Point(0.0f, 0.0f, 0.0f), Vector(0.0f, 1.0f, 0.0f));

    int image_counter = 1;
    char choice = 'S';

    while (choice == 'S' || choice == 's') {
        float sx, sy, sz;
        std::cout << "Digite uma esfera (x y z): ";
        std::cin >> sx >> sy >> sz;

        float cx, cy, cz;
        std::cout << "Digite a posição da câmera (x y z): ";
        std::cin >> cx >> cy >> cz;

        float tx, ty, tz;
        std::cout << "Digite a posição do target (x y z): ";
        std::cin >> tx >> ty >> tz;

        Geometry::Sphere sphere(Point(sx, sy, sz), 1.0f);
        Point camera_position(cx, cy, cz);
        Point look_at(tx, ty, tz);
        Vector up_vector(0.0f, 1.0f, 0.0f);
        float vertical_fov = 90.0f * M_PI / 180.0f;

        Camera camera(camera_position, look_at, up_vector, vertical_fov, image_height, image_width);

        std::stringstream filename;
        filename << output_folder << "/imagem" << image_counter << ".ppm";
        render_scene(camera, filename.str(), sphere, plane);
        image_counter++;

        std::cout << "Quer gerar outra? (S/N): ";
        std::cin >> choice;
    }

    return 0;
}
