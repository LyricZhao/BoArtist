# ifndef __SMALLPT_V0_H__
# define __SMALLPT_V0_H__

# include "../objects/api.h"

namespace debug_scene {

int width = 512, height = 512, samples = 300;

Ray camera(Vector3D(0, 0, 0), Vector3D(0, 1, 0).norm());

Object* objects[] = {
    new Plane(Vector3D(-1. / 120, 1. / 120), DIFF, 1.5, Texture("", Color_F(0.25, 0.75, 0.75)), Color_F()),
    new Plane(Vector3D( 1. / 120, 1. / 120), DIFF, 1.5, Texture("", Color_F(0.75, 0.25, 0.75)), Color_F()),
    new Plane(Vector3D(0,  1. / 120, -1. / 120), DIFF, 1.5, Texture("", Color_F(0.75, 0.75, 0.25)), Color_F()),
    new Plane(Vector3D(0,  1. / 120,  1. / 120), DIFF, 1.5, Texture("", Color_F(0.75, 0.5, 0.75)), Color_F()),
    new Sphere(5., Vector3D(0, 100, 0), 1.5, Texture(), REFR, Color_F(3, 3, 3)),
    new Sphere(10., Vector3D( 20, 100, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), SPEC, Color_F()),
    new Sphere(10., Vector3D(-20, 100, 0), 1.5, Texture("", Color_F(1, 1, 1) * 0.999), REFR, Color_F())
}; 

const char output[] = "outputs/test";
};

# endif
