#include "grid.hpp"

#include <limits>

math::vec3 p3d::grid::min_coordinates() {
    float min_x = std::numeric_limits<float>::max();
    float min_y = min_x;
    float min_z = min_x;

    for (scene_obj *obj : _cells) {
        b_box box = obj->box();
        if (box.x0() < min_x)
            min_x = box.x0();
        if (box.y0() < min_y)
            min_y = box.y0();
        if (box.z0() < min_z)
            min_z = box.z0();
    }

    min_x -= math::KEPSILON;
    min_y -= math::KEPSILON;
    min_z -= math::KEPSILON;
    return math::vec3(min_x, min_y, min_z);
}

math::vec3 p3d::grid::max_coordinates() {
    float max_x = std::numeric_limits<float>::min();
    float max_y = max_x;
    float max_z = max_x;

    for (scene_obj *obj : _cells) {
        b_box box = obj->box();
        if (box.x1() > max_x)
            max_x = box.x1();
        if (box.y1() > max_y)
            max_y = box.y1();
        if (box.z1() > max_z)
            max_z = box.z1();
    }

    max_x -= math::KEPSILON;
    max_y -= math::KEPSILON;
    max_z -= math::KEPSILON;
    return math::vec3(max_x, max_y, max_z);
}

void p3d::grid::setup_cells() {}
