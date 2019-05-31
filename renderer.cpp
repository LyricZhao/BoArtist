# include <cassert>
# include <cstdio>

# include "renderer.h"
# include "utils.h"

# define L_DEBUG_MODE
# define TO_RENDER debug_scene
# include "scenes/debug.h"

void Renderer:: load() {
    std:: cout << "Loading scene ... " << std:: flush;
    width = TO_RENDER:: width, height = TO_RENDER:: height;
    n_objects = sizeof(TO_RENDER:: objects) / sizeof(Object*);
    objects = TO_RENDER:: objects, samples = TO_RENDER:: samples, camera = TO_RENDER:: camera;
    output = std:: string(TO_RENDER:: output);
    pixels = (Color_F*) std:: malloc(sizeof(Color_F) * width * height);
    for(int i = 0; i < n_objects; ++ i) objects[i] -> load();
    for(int i = 0; i < n_objects; ++ i) objects[i] -> load_texture();
    std:: cout << "ok!" << std:: endl;
    # ifdef L_DEBUG_MODE
        /* Debuging tests */
        // objects[0] -> load();
        // objects[0] -> print();
        // objects[0] -> debug();
        // exit(0);
    # endif
    return;
}

bool Renderer:: intersect(const Ray &ray, double &t, int &id, Vector3D &n) {
    double d; t = inf; Vector3D t_norm;
    for(int i = 0; i < n_objects; ++ i) {
        if((d = objects[i] -> intersect(ray, t_norm)) && d < t)
            t = d, id = i, n = t_norm;
    }
    return t < inf;
}

Color_F Renderer:: radiance(const Ray &ray, int depth, unsigned short *seed) {
    double t; int id, in = 0; Vector3D n;
    if(depth > 10 || !intersect(ray, t, id, n)) return Color_F();
    Object *object = objects[id];
    Vector3D x = ray.o + ray.d * t;
    Vector3D nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    Color_F f = object -> color(x);
    double mx = f.max();
    if(++ depth > 5) {
        if(erand48(seed) < mx) f /= mx;
        else return object -> emission;
    }
    switch (object -> reflect) {
        case DIFF: {
            double r1 = 2 * M_PI * erand48(seed), r2 = M_PI * erand48(seed);
            Vector3D w = nl, u = (fabs(w.x) > .1 ? Vector3D(0, 1): Vector3D(1)).cross(w).norm(), v = w.cross(u);
            Vector3D d = ((u * cos(r1) + v * sin(r1)) * cos(r2) + w * sin(r2)).norm();
            Color_F tmp = radiance(Ray(x, d), depth, seed);
            return Vector3D(object -> emission) + f.mul(tmp);
        }
        case SPEC: {
            Ray reflect_ray = Ray(x, ray.d.reflect(n));
            return Vector3D(object -> emission) + f.mul(radiance(reflect_ray, depth, seed));
        }
        case REFR: {
            Ray reflect_ray = Ray(x, ray.d.reflect(n));
            Vector3D d = ray.d.refract(nl, in ? 1 : object -> ior, in ? object -> ior : 1);
            if(d.length2() < eps) return object -> emission + f.mul(radiance(reflect_ray, depth, seed));
            double a = object -> ior - 1, b = object -> ior + 1, r0 = a * a / (b * b), c = 1 - (in ? -ray.d.dot(nl) : d.dot(n));
            double re = r0 + (1 - r0) * c * c * c * c * c, tr = 1 - re, p = .25 + .5 * re, rp = re / p, tp = tr / (1 - p);
            return object -> emission + f.mul(depth > 2 ?
                (erand48(seed) < p ? radiance(reflect_ray, depth, seed) * rp : radiance(Ray(x, d), depth, seed) * tp) :
                (radiance(reflect_ray, depth, seed) * re + radiance(Ray(x, d), depth, seed) * tr));
        }
        default: assert(0);
    }
}

void Renderer:: render() {
    Vector3D cx = Vector3D(width * .5 / height);
    Vector3D cy = cx.cross(camera.d).norm() * .5;
    std:: cout << "Rendering ... " << std:: endl;
# ifndef L_DEBUG_MODE
    #pragma omp parallel for schedule(dynamic, 1)
# endif
    for(int y = 0; y < height; ++ y) {
        fprintf(stderr, "\rRendering %f%%", 100. * y / height);
        for(int x = 0; x < width; ++ x) {
            int index = (height - y - 1) * width + (width - x - 1);
            for(int sy = 0; sy < 2; ++ sy) {
                for(int sx = 0; sx < 2; ++ sx) {
                    Color_F pixel;
                    unsigned short seed[3] = {(unsigned short)(x), (unsigned short)(y), (unsigned short)(sx * 2 + sy)};
                    for(int s = 0; s < samples; ++ s) {
                        double r1 = 2 * erand48(seed), dx = r1 < 1 ? sqrt(r1) : 2 - sqrt(2 - r1);
                        double r2 = 2 * erand48(seed), dy = r2 < 1 ? sqrt(r2) : 2 - sqrt(2 - r2);
                        Vector3D d = cx * ((sx + dx / 2 + x) / width - .5) + cy * ((sy + dy / 2 + y) / height - .5) + camera.d;
                        pixel += radiance(Ray(camera.o + d, d.norm()), 0, seed);
                    }
                    pixels[index] += (pixel / samples).clamp() / 4.;
                }
            }
        }
    }
    return;
}

void Renderer:: save() {
    FILE *file = fopen((output + ".ppm").c_str(), "w");
    fprintf(file, "P6\n%d %d\n%d\n", width, height, 255);
    for(int i = 0; i < width * height; ++ i)
        fprintf(file, "%c%c%c", pixels[i].r(), pixels[i].g(), pixels[i].b());
    return;
}