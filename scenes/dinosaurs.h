# ifndef __DINOSAURS_H__
# define __DINOSAURS_H__

# include "../objects/api.h"

namespace dinosaurs {

int width = 2560, height = 1440, samples = 6000000;
Vector3D light_point(400, 200, 100), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(-200, 0, 0), Vector3D(1.1, 1, 0).norm());
int iteration_time = 1000;
double dof = 3.9, r_alpha = 0.7, sppm_radius = 10, energy = 90, camera_scale = 0.5;

Ray ray_generator(unsigned short *seed) {
    double x = erand48(seed) * 2 - 1, y = erand48(seed) * 2 - 1, z = erand48(seed) * 2 - 1;
    return Ray(light_point, Vector3D(x, y, z).norm());
}

# define GRAY_SCALE 0.6

Object* objects[] = {
    new Plane(Vector3D(0, 1 / 1470.0, 0), DIFF, 1.5, Texture("sources/dinosaur_texture.jpg", Color_F(0.75, 0.75, 0.75)), Color_F(), Vector3D(0, 0, -30), Vector3D(10, 0, 0), Vector3D(0, 0, 10)),
    new Plane(Vector3D(0, 0, 1. / (-45)), DIFF, 1.5, Texture("sources/dinosaur_texture.jpg", Color_F(0.75, 0.75, 0.75)), Color_F(), Vector3D(0, 0, -30), Vector3D(10, 0, 0), Vector3D(0, 10, 0)),
    new Mesh(Vector3D(0, 200, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(50, 350, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(100, 500, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(150, 650, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(200, 800, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(250, 950, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(300, 1000, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(350, 1150, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(400, 1300, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
    new Mesh(Vector3D(450, 1450, 0), "sources/dinosaur.2k.obj", DIFF, 1.5, 1, Texture("", Color_F(rand01(), rand01(), rand01()) * GRAY_SCALE)),
}; 

const char output[] = "outputs/dinosaurs_dof";
};

# endif
