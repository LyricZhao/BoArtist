# ifndef __BRETESCHE_H__
# define __BRETESCHE_H__

# include "../objects/api.h"

namespace bretesche {

int width = 512, height = 512, samples = 20000;
Vector3D light_point(0, 200, 40), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(0, 0, 5), Vector3D(0, 1, -1).norm());
int iteration_time = 100;
double r_alpha = 0.7, sppm_radius = 2, energy = 10.;

Ray ray_generator(unsigned short *seed) {
    double x = erand48(seed) * 2 - 1, y = erand48(seed) * 2 - 1, z = erand48(seed) * 2 - 1;
    return Ray(light_point, Vector3D(x, y, z).norm());
}

Object* objects[] = {
    new Mesh(Vector3D(0, 200, 0), "sources/Bretesche.obj", DIFF, 1.5, 1, Texture("", Color_F(1, 1, 1) * 0.8)),
}; 

const char output[] = "outputs/Bretesche";
};

# endif
