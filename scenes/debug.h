# ifndef __SMALLPT_V0_H__
# define __SMALLPT_V0_H__

# include "../objects/api.h"

namespace debug_scene {

int width = 512, height = 512, samples = 3000;

Ray camera(Vector3D(0, 0, 0), Vector3D(0, 1, 0).norm());

Vector2D points[] = {
    Vector2D(11, 36), Vector2D(10.5, 42.5), Vector2D(15.5, 43), Vector2D(16.6, 38)
};

Object* objects[] = {
    // new Plane(Vector3D(0, 1. / 120), DIFF, 1.5, Texture(), Color_F(), Vector3D(0, 120), Vector3D(1), Vector3D(0, 0, 1)),
    // new Plane(Vector3D( 1. / 120, 1. / 120), DIFF, 1.5, Texture("", Color_F(0.75, 0.25, 0.75)), Color_F()),
    // new Plane(Vector3D(0,  1. / 120, -1. / 120), DIFF, 1.5, Texture("", Color_F(0.75, 0.75, 0.25)), Color_F()),
    // new Plane(Vector3D(0,  1. / 120,  1. / 120), DIFF, 1.5, Texture("", Color_F(0.75, 0.5, 0.75)), Color_F()),
    // new Sphere(5., Vector3D(0, 100, 0), 1.5, Texture(), REFR, Color_F(3, 3, 3))
    // new Sphere(10., Vector3D( 20, 100, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), SPEC, Color_F()),
    // new Sphere(10., Vector3D(-20, 100, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), REFR, Color_F())
    // new Sphere(1., Vector3D(0, 100, 0), 1.5, Texture(), REFR, Color_F(12, 12, 12)),
    // new Cube(Vector3D(5, 100, -5), Vector3D(15, 100, -5), Vector3D(5, 110, -5), Vector3D(5, 100, 5), DIFF, 1.5, Texture("", Color_F(0.75, 0.25, 0.75)), Color_F())
    new Bezier3D(Bezier2D(sizeof(points) / sizeof(Vector2D), points), Range1D(100, 110), DIFF, 1.5, Texture(), Color_F())
}; 

const char output[] = "outputs/test";
};

# endif
