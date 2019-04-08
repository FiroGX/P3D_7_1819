#include "b_box.hpp"

#include <limits>

p3d::b_box::b_box() : 
    _x0(std::numeric_limits<float>::max()), _y0(std::numeric_limits<float>::max()), _z0(std::numeric_limits<float>::max()),
    _x1(std::numeric_limits<float>::min()), _y1(std::numeric_limits<float>::min()), _z1(std::numeric_limits<float>::min()) {
}

p3d::b_box::b_box(float x0, float y0, float z0,
    float x1, float y1, float z1) :
    _x0(x0), _y0(y0), _z0(z0),
    _x1(x1), _y1(y1), _z1(z1) {
}

bool p3d::b_box::collided(const ray &ray) {
    float tx_min, ty_min, tz_min;
    float tx_max, ty_max, tz_max;

    float a = 1.0f / ray.d().x();
    if (a >= 0) {
        tx_min = (_x0 - ray.o().x()) * a;
        tx_max = (_x1 - ray.o().x()) * a;
    } else {
        tx_min = (_x1 - ray.o().x()) * a;
        tx_max = (_x0 - ray.o().x()) * a;
    }

    float b = 1.0f / ray.d().y();
    if (b >= 0) {
        ty_min = (_y0 - ray.o().y()) * a;
        ty_max = (_y1 - ray.o().y()) * a;
    } else {
        ty_min = (_y1 - ray.o().y()) * a;
        ty_max = (_y0 - ray.o().y()) * a;
    }

    float c = 1.0f / ray.d().z();
    if (c >= 0) {
        tz_min = (_z0 - ray.o().z()) * a;
        tz_max = (_z1 - ray.o().z()) * a;
    } else {
        tz_min = (_z1 - ray.o().z()) * a;
        tz_max = (_z0 - ray.o().z()) * a;
    }

    float t0, t1;
    // find the largest entering t value
    if (tx_min > ty_min)
        t0 = tx_min;
    else
        t0 = ty_min;
    if (tz_min > t0)
        t0 = tz_min;

    // find the smallest exiting t value
    if (tx_max < ty_max)
        t1 = tx_max;
    else
        t1 = ty_max;
    if (tz_max < t1)
        t1 = tz_max;

    return t0 < t1 && t1 > math::KEPSILON;
}

bool p3d::b_box::inside(math::vec3 p) {
    return _x0 <= p.x() && p.x() >= _x1 &&
        _y0 <= p.y() && p.y() >= _y1 &&
        _z0 <= p.z() && p.z() >= _z1;
}
