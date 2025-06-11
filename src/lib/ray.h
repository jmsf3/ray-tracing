#pragma once

#include <ostream>
#include "lib/point.h"
#include "lib/vector.h"

struct Ray
{
    Point origin {};
    Vector direction {};

    explicit Ray(Point origin, Vector direction) : origin { origin }, direction { direction } {}

    Ray() = default;
    Ray(const Ray&) = default;
    ~Ray() = default;
    Ray& operator=(const Ray&) = default;

    Point at(const float& t) const
    {
        return origin + t * direction;
    }
};

inline std::ostream& operator<<(std::ostream& os, const Ray& r)
{
    os << "Ray(" << r.origin << ", " << r.direction << ")";
    return os;
}
