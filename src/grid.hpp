#pragma once

#include "scene_obj.hpp"

#include <vector>

namespace p3d {
    class grid {
        std::vector<std::vector<scene_obj *>> _cells;
        b_box _b_box;
        int _nx, _ny, _nz;
        int last = 2;

        math::vec3 min_coordinates(const std::vector<scene_obj *> &);
        math::vec3 max_coordinates(const std::vector<scene_obj *> &);
        hit calculate_hit(const std::vector<scene_obj *> &, const ray &);

    public:
        grid() = default;
        ~grid() = default;

        void setup_cells(const std::vector<scene_obj *> &);
        hit traverse(const ray &);
    };
}
