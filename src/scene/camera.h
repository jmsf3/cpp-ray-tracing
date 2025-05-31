#pragma once

#include <cstdint>
#include "lib/point.h"
#include "lib/ray.h"
#include "lib/vector.h"

class Camera
{
private:
    Point camera_center {};
    Point image_center {};
    Vector up {};
    Vector u, v, w {};
    float image_distance {};
    uint32_t height, width {};
    float vfov {};
    float aspect {};
public:

    Camera() = default;
    Camera(const Camera &) = default;
    ~Camera() = default;
    Camera &operator=(const Camera &) = default;
    explicit Camera(Point camera_center, Point image_center, Vector up,
        uint32_t height, uint32_t width, float vfov) :
        camera_center{camera_center},
        image_center{image_center},
        up{up},
        height{height},
        width{width},
        vfov {vfov}
    {
        image_distance = (image_center - camera_center).norm();
        w = -(image_center - camera_center).normalized();
        v = (up - w*(dot(w, up)/ pow(w.norm(),2))).normalized();
        u = cross(w,v).normalized();
        aspect = static_cast<float> (width)/ height;
    }

    Ray cast(float s, float t) const{
        float theta = vfov * M_PI/180;
        float half_height = tan(theta/2) * image_distance;
        float half_width = aspect * half_height;

        Vector horizontal = u * (2 * half_width);
        Vector vertical = v * (2 * half_height);
        Point lower_left_corner = image_center - horizontal/2 - vertical/2 -w * image_distance;

        Point pixel = lower_left_corner + horizontal * s + vertical*t;
        Vector ray_direction = (pixel - camera_center).normalized();
        return Ray(camera_center, ray_direction);
    }

};
