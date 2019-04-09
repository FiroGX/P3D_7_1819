#pragma once

#include "scene_obj.hpp"

#include <vector>

namespace p3d {
    class grid {
        std::vector<scene_obj *> _cells;
        b_box _b_box;
        int _nx, _ny, _nz;

        math::vec3 min_coordinates();
        math::vec3 max_coordinates();

    public:
        grid() = default;
        ~grid() = default;

        void setup_cells();
    };
}