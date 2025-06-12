#pragma once

#include "../lib/point.h"
#include "../lib/vector.h"

namespace RT
{
    struct Trace
    {
        bool hit {};
        Point origin {};
        Point position {};
        Vector normal {};

        explicit Trace(bool hit, Point origin, Point position, Vector normal) : hit { hit },
                                                                                origin { origin },
                                                                                position { position },
                                                                                normal { normal } {}

        Trace() = default;
        Trace(const Trace&) = default;
        ~Trace() = default;
        Trace& operator=(const Trace&) = default;
    };
}

inline std::ostream& operator<<(std::ostream& os, const RT::Trace& t)
{
    os << "Trace(" << std::boolalpha << t.hit << ", " << t.origin
       << ", " << t.position << ", " << t.normal << ")";
    return os;
}
