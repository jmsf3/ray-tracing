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

    w = image_center - camera_center;
    w = w.normalized();
    w = -w;

    v = up - (dot(w, up) / dot(w, w)) * w;
    v = v.normalized();

    u = cross(v, w);
    lower_left_pixel = image_center - (width / 2.0f) * u - (height / 2.0f) * v;
}

Ray Camera::cast_ray(const uint32_t& px, const uint32_t& py) const
{
    Point pixel = lower_left_pixel + px * u + py * v;
    Vector direction = (pixel - camera_center).normalized();
    return Ray { camera_center, direction };
}
