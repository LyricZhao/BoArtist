# ifndef __BRETESCHE_H__
# define __BRETESCHE_H__

# include "../objects/api.h"

namespace bretesche {

int width = 2560, height = 1440, samples = 8000000;
Vector3D light_point(0, 200, 90), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(0, 0, 30), Vector3D(0, 1, -0.14).norm());
int iteration_time = 1000;
double r_alpha = 0.7, sppm_radius = 0.4, energy = 1, camera_scale = 0.07;

Ray ray_generator(unsigned short *seed) {
    double r1 = 2 * M_PI * erand48(seed), r2 = erand48(seed), s_r2 = sqrt(r2);
    Vector3D d = ((u * cos(r1) + v * sin(r1)) * s_r2 - w * sqrt(1 - r2)).norm();
    return Ray(light_point, d);
}

Object* objects[] = {
    new Mesh(Vector3D(0, 200, 0), "sources/Bretesche.obj", "sources/Bretesche.jpg", DIFF, 1.5, 1, Texture("", Color_F(1, 1, 1) * 0.8)),
    new Plane(Vector3D(0, 1. / 300, 0), DIFF, 1.5, Texture("sources/sky.jpg", Color_F(0.25, 0.25, 0.75)), Color_F(), Vector3D(-50, 300, 0), Vector3D(1, 0, 0), Vector3D(0, 0, 1)),
}; 

const char output[] = "outputs/Bretesche";
};

# endif
