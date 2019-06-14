# ifndef __BRETESCHE_H__
# define __BRETESCHE_H__

# include "../objects/api.h"

namespace indoor {

int width = 512, height = 512, samples = 2000000;
Vector3D light_point(0, 0, 0), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(0, 0, 0), Vector3D(0, 1, 0).norm());
int iteration_time = 5;
double r_alpha = 0.7, sppm_radius = 20, energy = 1000, camera_scale = 10;

Ray ray_generator(unsigned short *seed) {
    double r1 = 2 * M_PI * erand48(seed), r2 = erand48(seed), s_r2 = sqrt(r2);
    Vector3D d = ((u * cos(r1) + v * sin(r1)) * s_r2 - w * sqrt(1 - r2)).norm();
    return Ray(light_point, d);
}

Object* objects[] = {
    new Mesh(Vector3D(0, 0, 0), "sources/indoor.obj", DIFF, 1.5, 50000., Texture("sources/indoor.jpg", Color_F(1, 1, 1) * 0.8)),
}; 

const char output[] = "outputs/indoor_sppm";
};

# endif
