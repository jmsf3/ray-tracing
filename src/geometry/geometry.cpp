#include <cmath>
#include "geometry.h"

namespace Geometry
{
    RT::Trace Sphere::hit(const Ray& ray) const
    {
        bool hit { false };
        Point origin { ray.origin };
        Point position {};
        Vector normal {};

        // To simplify the problem, we find the
        // intersection in a coordinate system
        // where the sphere is centered at the origin.
        Vector o = ray.origin - center;
        Vector d = ray.direction;
        float r = radius;

        float a = dot(d, d);
        float b = 2.0f * dot(o, d);
        float c = dot(o, o) - r * r;
        float discriminant = b * b - 4.0f * a * c;

        if (discriminant < 0.0f)
        {
            return RT::Trace { hit, origin, position, normal };
        }

        float t {};
        float t1 = (-b - std::sqrt(discriminant)) / (2.0f * a);
        float t2 = (-b + std::sqrt(discriminant)) / (2.0f * a);

        if (t1 > 0.0f)
        {
            t = t1;
        }
        else if (t2 > 0.0f)
        {
            t = t2;
        }
        else
        {
            return RT::Trace { hit, origin, position, normal };
        }

        hit = true;
        position = ray.at(t);
        normal = (position - center).normalized();

        return RT::Trace { hit, origin, position, normal };
    }

    RT::Trace Plane::hit(const Ray& ray) const
    {
        bool hit { false };
        Point origin { ray.origin };
        Point position {};
        Vector normal {};

        Point o = ray.origin;
        Vector d = ray.direction;
        Point p = this->point;
        Vector n = this->normal;

        // When comparing floating-point numbers,
        // it's better to check if the difference
        // between them is smaller than a value epsilon
        // rather than checking for exact equality.
        constexpr float epsilon = 1e-6f;

        if (std::abs(dot(n, d)) < epsilon)
        {
            return RT::Trace { hit, origin, position, normal };
        }

        float t = dot(n, p - o) / dot(n, d);

        if (t < 0.0f)
        {
            return RT::Trace { hit, origin, position, normal };
        }

        hit = true;
        position = ray.at(t);
        normal = n.normalized();

        return RT::Trace { hit, origin, position, normal };
    }

    RT::Trace Triangle::hit(const Ray& ray) const
    {
        // TODO: Implement the hit function for Triangle

        bool hit { false };
        Point origin { ray.origin };
        Point position {};
        Vector normal {};

        return RT::Trace { hit, origin, position, normal };
    }
}
