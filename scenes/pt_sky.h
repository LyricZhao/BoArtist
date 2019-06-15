# ifndef __PT_SKY_H__
# define __PT_SKY_H__

/* Only in PT Mode */

# include "../objects/api.h"

namespace pt_sky {

int width = 2560, height = 1440, samples = 200000;
Vector3D light_point(0, 80, 0), u(1, 0, 0), v(0, 1, 0), w(0, 0, 1);

Ray camera(Vector3D(50, 52, 295.6), Vector3D(0, -0.042612, -1).norm());
int iteration_time = 1000;
double r_alpha = 0.7, sppm_radius = 2, energy = 10., camera_scale = 0.5135, dof = 0, focal_distance = 1.;

Ray ray_generator(unsigned short *seed) {
    double x = erand48(seed) * 2 - 1, y = erand48(seed) * 2 - 1, z = erand48(seed) * 2 - 1;
    return Ray(light_point, Vector3D(x, y, z).norm());
}

Vector3D Cen(50, 40.8, -860);

Object* objects[] = {
    new Sphere(1600, Vector3D(1, 0, 2) * 3000, 1.5, Texture("", Color_F()), DIFF, Color_F(1, .9, .8) * 1.2e1 * 1.56 * 2),
    new Sphere(1560, Vector3D(1, 0, 2) * 3500, 1.5, Texture("", Color_F()), DIFF, Color_F(1, .5, .05) * 4.8e1 * 1.56 * 2),
    new Sphere(10000, Cen + Vector3D(0, 0, -200), 1.5, Texture("", Color_F(.7, .7, 1) * .25), DIFF, Color_F(0.00063842, 0.02001478, 0.28923243) * 6e-2 * 8),
    new Sphere(100000, Vector3D(50, -100000, 0), 1.5, Texture("", Color_F(.3, .3, .3)), DIFF, Color_F()),
    new Sphere(110000, Vector3D(50, -110048.5, 0), 1.5, Texture("", Color_F()), DIFF, Color_F(.9, .5, .05) * 4),
    new Sphere(4e4, Vector3D(50, -4e4-30, -3000), 1.5, Texture("", Color_F(.2, .2, .2)), DIFF, Color_F()),
    new Sphere(26.5, Vector3D(22, 26.5, 42), 1.5, Texture("", Color_F(1, 1, 1) * .596), SPEC, Color_F()),
    new Sphere(13, Vector3D(75, 13, 82), 1.5, Texture("", Color_F(.96, .96, .96) * .96), REFR, Color_F()),
    new Sphere(12, Vector3D(87, 22, 24), 1.5, Texture("", Color_F(.6, .6, .6) * .696), SPEC, Color_F()),
}; 

const char output[] = "outputs/pt_sky";
};



# endif
