# ifndef __SMALLPT_V0_H__
# define __SMALLPT_V0_H__

# include "../objects/api.h"

namespace debug_scene {

int width = 512, height = 512, samples = 3000000;
Vector3D light_point(10, 90, 0), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(0, 0, 0), Vector3D(0, 1, 0).norm());
int iteration_time = 1;
double r_alpha = 0.9, sppm_radius = 1;

/*
Vector2D points[] = {
    Vector2D(11, 36), Vector2D(10.5, 42.5), Vector2D(15.5, 43), Vector2D(16.6, 38), Vector2D(22.1, 38.4), Vector2D(27.7, 37.4),
    Vector2D(31, 36.2), Vector2D(34.5, 41.2), Vector2D(38.3, 39), Vector2D(36, 33), Vector2D(38.5, 30.5), Vector2D(40.5, 27.5), Vector2D(43, 19),
    Vector2D(36.5, 5.5), Vector2D(30.3, 1.7), Vector2D(22, 0.1), Vector2D(10.6, 2.9), Vector2D(6, 6.3), Vector2D(0, 19.5),
    Vector2D(3.5, 30), Vector2D(7, 33.5)
};
*/

Ray ray_generator(unsigned short *seed) {
    double alpha = 2 * M_PI * erand48(seed), beta = 2 * M_PI * erand48(seed);
    return Ray(light_point, u * sin(alpha) * sin(beta) + v * cos(alpha) * sin(beta) + w * cos(beta));
}

Object* objects[] = {
    // new Plane(Vector3D(0, 1. / 650), DIFF, 1.5, Texture("", Color_F(0.75, 0.25, 0.75)), Color_F(), Vector3D(0, 120), Vector3D(1), Vector3D(0, 0, 1)),
    // new Plane(Vector3D(1. / 650, 1. / 650), DIFF, 1.5, Texture("", Color_F(0.75, 0.25, 0.75)), Color_F()),
    // new Plane(Vector3D(0,  1. / 650, -1. / 650), DIFF, 1.5, Texture("", Color_F(0.75, 0.75, 0.25)), Color_F()),
    // new Plane(Vector3D(0,  1. / 650,  1. / 650), DIFF, 1.5, Texture("", Color_F(0.75, 0.5, 0.75)), Color_F()),
    // new Sphere(5., Vector3D(0, 100, 0), 1.5, Texture(), REFR, Color_F(3, 3, 3))
    new Sphere(5, Vector3D(0, 100, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), DIFF, Color_F()),
    // new Sphere(10., Vector3D(-20, 100, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), REFR, Color_F())
    // new Sphere(5., Vector3D(0, 480, 0), 1.5, Texture(), REFR, Color_F(12, 12, 12)),
    // new Sphere(5., Vector3D(40, 480, 0), 1.5, Texture(), REFR, Color_F(12, 12, 12)),
    // new Cube(Vector3D(5, 100, -5), Vector3D(15, 100, -5), Vector3D(5, 110, -5), Vector3D(5, 100, 5), DIFF, 1.5, Texture("", Color_F(0.75, 0.25, 0.75)), Color_F())
    // new Bezier3D(Bezier2D(sizeof(points) / sizeof(Vector2D), points), Range1D(500, 600), DIFF, 1.5, Texture("sources/pig.png", Color_F(0.75, 0.75, 0.75)), Color_F())
}; 

const char output[] = "outputs/test";
};

# endif
