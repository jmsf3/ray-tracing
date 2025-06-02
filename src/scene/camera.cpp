#include <cassert>
#include "scene/camera.h"

Camera::Camera(Point camera_center, Point image_center, Vector up,
               uint32_t height, uint32_t width) : camera_center { camera_center },
                                                  image_center { image_center },
                                                  up { up },
                                                  height { height },
                                                  width { width }
{
    assert(height != 0 && width != 0);
    distance_to_image = (image_center - camera_center).norm();

    w = -(image_center - camera_center).normalized();
    v = (up - w * (dot(w, up) / pow(w.norm(), 2))).normalized();
    u = cross(v, w).normalized();

    lower_left_pixel = image_center - u * ((width - 1) / 2.0f) - v * ((height - 1) / 2.0f);
}

Ray Camera::cast_ray(const uint32_t& px, const uint32_t& py) const
{
    Point pixel = lower_left_pixel + u * px + v * py;
    Vector direction = (pixel - camera_center).normalized();
    return Ray(camera_center, direction);
}
