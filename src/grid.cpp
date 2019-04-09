#include "grid.hpp"

#include <cmath>
#include <limits>

math::vec3 p3d::grid::min_coordinates(const std::vector<scene_obj *> &objs) {
    float min_x = std::numeric_limits<float>::max();
    float min_y = min_x;
    float min_z = min_x;

    for (scene_obj *obj : objs) {
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

math::vec3 p3d::grid::max_coordinates(const std::vector<scene_obj *> &objs) {
    float max_x = std::numeric_limits<float>::min();
    float max_y = max_x;
    float max_z = max_x;

    for (scene_obj *obj : objs) {
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

void p3d::grid::setup_cells(const std::vector<scene_obj *> &objs) {
    // find the minimum and maximum coordinates of the grid
    math::vec3 p0 = min_coordinates(objs);
    math::vec3 p1 = max_coordinates(objs);

    // store them on the bounding box
    _b_box = b_box(p0.x(), p0.y(), p0.z(),
        p1.x(), p1.y(), p1.z());

    // compute the number of cells in the x, y, z directions
    int num_objs = objs.size();
    float wx = p1.x() - p0.x(); // grid extent in x
    float wy = p1.y() - p0.y(); // grid extent in y
    float wz = p1.z() - p0.z(); // grid extent in z
    float mult = 2.0f;          // about 8 times more cells than objects
    float s = std::pow(wx * wy * wz / num_objs, 1 / 3);
    _nx = mult * wx / s + 1;
    _ny = mult * wy / s + 1;
    _nz = mult * wz / s + 1;


}
