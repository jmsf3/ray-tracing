#pragma once

#include <cstdint>
#include "lib/point.h"
#include "lib/ray.h"
#include "lib/vector.h"

class Camera
{
private:
    Point camera_center {};
    Point image_center {};

    Vector up {};
    Vector u, v, w {};

    float distance_to_image {};
    uint32_t height, width {};
    Point lower_left_pixel {};

public:
    explicit Camera(Point camera_center, Point image_center, Vector up,
                    uint32_t height, uint32_t width);

    Camera() = default;
    Camera(const Camera &) = default;
    ~Camera() = default;
    Camera &operator=(const Camera &) = default;

    Ray cast_ray(const uint32_t& px, const uint32_t& py) const;
};
