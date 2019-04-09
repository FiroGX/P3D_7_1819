#pragma once

#include "hit.hpp"
#include "ray.hpp"
#include "math/vec3.hpp"

namespace p3d {
    class b_box {
        float _x0, _y0, _z0;
        float _x1, _y1, _z1;

    public:
        b_box();
        b_box(float, float, float, float, float, float);
        ~b_box() = default;

        float x0();
        float y0();
        float z0();
        float x1();
        float y1();
        float z1();

        bool collided(const ray &);
        bool inside(math::vec3);
    };
}