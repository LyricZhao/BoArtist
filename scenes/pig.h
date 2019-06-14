# ifndef __DEBUG_SCENE_H__
# define __DEBUG_SCENE_H__

# include "../objects/api.h"

namespace pig {

int width = 2560, height = 1440, samples = 400000;
Vector3D light_point(20, 400, 20), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(0, 300, 0), Vector3D(0.1, 1, 0.1).norm());
int iteration_time = 1000;
double r_alpha = 0.7, sppm_radius = 10, energy = 20., camera_scale = 0.4;

Vector2D points[] = {
    Vector2D(10.94, 36.08), Vector2D(10.32, 37.57), Vector2D(9.84, 39.59), Vector2D(10.12, 41.95),
    Vector2D(11.89, 43.38), Vector2D(14.59, 43.39), Vector2D(16.13, 42.10), Vector2D(16.68, 40.18),
    Vector2D(16.66, 37.89), Vector2D(20.04, 38.26), Vector2D(25.12, 37.96), Vector2D(30.90, 36.16),
    Vector2D(31.54, 38.46), Vector2D(32.87, 40.46), Vector2D(34.51, 41.19), Vector2D(36.13, 40.8),
    Vector2D(38.02, 39.44), Vector2D(38.45, 37.56), Vector2D(37.79, 35.63),  Vector2D(36.87, 34.13), Vector2D(35.92, 33.09), Vector2D(37.08, 32.00),
    Vector2D(38.44, 30.54), Vector2D(40.53, 27.56), Vector2D(41.89, 24.59), Vector2D(42.69, 21.17),
    Vector2D(42.79, 18.63), Vector2D(42.54, 15.73), Vector2D(41.65, 12.66), Vector2D(39.65, 8.96),
    Vector2D(36.97, 5.87), Vector2D(33.51, 3.25), Vector2D(29.70, 1.46), Vector2D(26.04, 0.45),
    Vector2D(21.93, 0.14), Vector2D(17.99, 0.39), Vector2D(14.63, 1.20), Vector2D(10.64, 2.85),
    Vector2D(7.50, 4.89), Vector2D(4.73, 7.46), Vector2D(3.09, 9.53), Vector2D(1.76, 11.94),
    Vector2D(0.73, 14.50), Vector2D(0.16, 16.97), Vector2D(0.04, 19.33), Vector2D(0.46, 23.52),
    Vector2D(1.41, 26.39), Vector2D(3.05, 29.36), Vector2D(4.85, 31.68), Vector2D(7.52, 34.00),
    Vector2D(9.99, 35.56),
};

Ray ray_generator(unsigned short *seed) {
    double x = erand48(seed) * 2 - 1, y = erand48(seed) * 2 - 1, z = erand48(seed) * 2 - 1;
    return Ray(light_point, Vector3D(x, y, z).norm());
}

Object* objects[] = {
    new Plane(Vector3D(0, 1. / 620), DIFF, 1.5, Texture("sources/angrybirds.jpg", Color_F(0.25, 0.75, 0.75)), Color_F(), Vector3D(0, 0, -30), Vector3D(1, 0, 0), Vector3D(0, 0, 1)),
    // new Sphere(100000., Vector3D(0, 600 + 100000, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), REFR, Color_F()),
    new Bezier3D(Bezier2D(sizeof(points) / sizeof(Vector2D), points), Range1D(500, 540), DIFF, 1.5, Texture("sources/pig_c2.png", Color_F(0.41, 0.72, 0.24), true), Color_F())
}; 

const char output[] = "outputs/pig";
};

# endif
