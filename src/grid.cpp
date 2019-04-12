#include "grid.hpp"

#include <iostream>
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
    int num_cells = _nx * _ny * _nz;

    // initialize the cells
    _cells = std::vector<std::vector<scene_obj *>>(num_cells, std::vector<scene_obj *>());

    for (scene_obj *obj : objs) {
        b_box obj_box = obj->box();
        int ixmin = math::clamp((obj_box.x0() - p0.x()) * _nx / (p1.x() - p0.x()), 0, _nx - 1);
        int iymin = math::clamp((obj_box.y0() - p0.y()) * _ny / (p1.y() - p0.y()), 0, _ny - 1);
        int izmin = math::clamp((obj_box.z0() - p0.z()) * _nz / (p1.z() - p0.z()), 0, _nz - 1);
        int ixmax = math::clamp((obj_box.x1() - p0.x()) * _nx / (p1.x() - p0.x()), 0, _nx - 1);
        int iymax = math::clamp((obj_box.y1() - p0.y()) * _ny / (p1.y() - p0.y()), 0, _ny - 1);
        int izmax = math::clamp((obj_box.z1() - p0.z()) * _nz / (p1.z() - p0.z()), 0, _nz - 1);

        // add objects to the cells
        for (int iz = izmin; iz < izmax; iz++) {
            for (int iy = iymin; iy < iymax; iy++) {
                for (int ix = ixmin; ix < ixmax; ix++) {
                    int index = ix + _nx * iy + _nx * _ny * iz;
                    _cells[index].push_back(obj);
                }
            }
        }
    }
}

p3d::hit p3d::grid::calculate_hit(const std::vector<scene_obj*> &objs, const ray &ray) {
    hit closest;
    for (scene_obj *obj : objs) {
        hit hit = obj->calculate_intersection(ray);
        if (hit.collided() && hit.distance() < closest.distance())
            closest = hit;
    }
    return closest;
}

p3d::hit p3d::grid::traverse(const ray &ray) {
    // if the ray doesn't intersect the grid, ignore
    hit col = _b_box.collision(ray);
    if (!col.collided())
        return col;

    float t0 = col.distance();
    math::vec3 t_max = col.point();
    math::vec3 t_min = col.normal();

    // calculate what is the initial cell of the ray intersection
    int ix, iy, iz;
    if (_b_box.inside(ray.o())) {
        if (last != 0) {
            std::cout << "inside" << std::endl;
            last = 0;
        }
        ix = math::clamp((ray.o().x() - _b_box.x0()) * _nx / (_b_box.x1() - _b_box.x0()), 0, _nx - 1);
        iy = math::clamp((ray.o().y() - _b_box.y0()) * _ny / (_b_box.y1() - _b_box.y0()), 0, _ny - 1);
        iz = math::clamp((ray.o().z() - _b_box.z0()) * _nz / (_b_box.z1() - _b_box.z0()), 0, _nz - 1);
    } else {
        if (last != 1) {
            std::cout << "outside" << std::endl;
            last = 1;
        }
        math::vec3 p = ray.o() + t0 * ray.d();
        ix = math::clamp((p.x() - _b_box.x0()) * _nx / (_b_box.x1() - _b_box.x0()), 0, _nx - 1);
        iy = math::clamp((p.y() - _b_box.y0()) * _ny / (_b_box.y1() - _b_box.y0()), 0, _ny - 1);
        iz = math::clamp((p.z() - _b_box.z0()) * _nz / (_b_box.z1() - _b_box.z0()), 0, _nz - 1);
    }

    // code setup for grid traversal
    float dtx = (t_max.x() - t_min.x()) / _nx;
    float dty = (t_max.y() - t_min.y()) / _ny;
    float dtz = (t_max.z() - t_min.z()) / _nz;

    float tx_next, ty_next, tz_next;
    int	ix_step, iy_step, iz_step;
    int ix_stop, iy_stop, iz_stop;

    tx_next = t_min.x() + (ix + 1) * dtx;
    ix_step = 1;
    ix_stop = _nx;

    ty_next = t_min.y() + (iy + 1) * dty;
    iy_step = 1;
    iy_stop = _ny;

    tz_next = t_min.z() + (iz + 1) * dtz;
    iz_step = 1;
    iz_stop = _nz;

    // grid traversal
    while (true) {
        int index = ix + _nx * iy + _nx * _ny * iz;
        std::vector<scene_obj*> objs = _cells[index];
        if (tx_next < ty_next && tx_next < tz_next) {
            hit close = calculate_hit(objs, ray);
            if (close.collided() && close.distance() < tx_next)
                return close;

            tx_next += dtx;
            ix += ix_step;

            if (ix == ix_stop)
                return hit();
        } else if (ty_next < tz_next) {
            hit close = calculate_hit(objs, ray);
            if (close.collided() && close.distance() < ty_next)
                return close;

            ty_next += dty;
            iy += iy_step;

            if (iy == iy_stop)
                return hit();
        } else {
            
            hit close = calculate_hit(objs, ray);
            if (close.collided() && close.distance() < tz_next)
                return close;

            tz_next += dtz;
            iz += iz_step;

            if (iz == iz_stop)
                return hit();
        }
    }
}
