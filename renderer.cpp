# include <cassert>
# include <cstdio>

# include "renderer.h"
# include "utils.h"

# define TO_RENDER smallpt_v0
# include "scenes/smallpt_v0.h"

void Renderer:: load() {
    width = TO_RENDER:: width, height = TO_RENDER:: height;
    n_sphere = sizeof(TO_RENDER:: spheres) / sizeof(Sphere);
    spheres = TO_RENDER:: spheres, samples = TO_RENDER:: samples, camera = TO_RENDER:: camera;
    output = std:: string(TO_RENDER:: output);
    pixels = (Color_F*) std:: malloc(sizeof(Color_F) * width * height);
    return;
}

bool Renderer:: intersect(const Ray &ray, double &t, int &id) {
    double d; t = inf;
    for(int i = 0; i < n_sphere; ++ i) {
        d = spheres[i].intersect(ray);
        if(d && d < t) t = d, id = i;
    }
    return t < inf;
}

Color_F Renderer:: radiance(const Ray &ray, int depth) {
    double t; int id, in;
    if(!intersect(ray, t, id)) return Color_F();
    const Sphere &sphere = spheres[id];
    Vector3D x = ray.o + ray.d * t;
    Vector3D n = (x - sphere.pos).norm();
    Vector3D nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    Color_F f = sphere.color;
    double mx = f.max();
    if(++ depth > 5) {
        if(rand01() < mx) f /= mx;
        else return sphere.emission;
    }
    switch (sphere.reflect) {
        case DIFF: {
            double r1 = 2 * M_PI * rand01(), r2 = rand01(), s_r2 = sqrt(r2);
            Vector3D w = nl, u = (fabs(w.x) > .1 ? Vector3D(0, 1): Vector3D(1)).cross(w).norm(), v = w.cross(u);
            Vector3D d = (u * cos(r1) * s_r2 + v * sin(r1) * s_r2 + w * sqrt(1 - r2)).norm();
            return Vector3D(sphere.emission) + f.mul(radiance(Ray(x, d), depth));
        }
        case SPEC: {
            Ray reflect_ray = Ray(x, ray.d.reflect(n));
            return Vector3D(sphere.emission) + f.mul(radiance(reflect_ray, depth));
        }
        case REFR: {
            Ray reflect_ray = Ray(x, ray.d.reflect(n));
            Vector3D d = ray.d.refract(n, in ? 1 : sphere.n_s, in ? sphere.n_s : 1);
            if(d.length2() < eps) return sphere.emission + f.mul(radiance(reflect_ray, depth));
            double a = sphere.n_s - 1, b = sphere.n_s + 1, r0 = a * a / (b * b), c = 1 - (in ? ray.d.dot(nl) : d.dot(n));
            double re = r0 + (1 - r0) * c * c * c * c * c, tr = 1 - re, p = .25 + .5 * re, rp = re / p, tp = tr / (1 - p);
            return sphere.emission + f.mul(depth > 2 ?
            (rand01() < p ? radiance(reflect_ray, depth) * rp : radiance(Ray(x, d), depth) * tp) :
            (radiance(reflect_ray, depth) * re + radiance(Ray(x, d), depth) * tr));
        }
        default: assert(0);
    }
}

void Renderer:: render() {
    Vector3D cx = Vector3D(width * .5 / height);
    Vector3D cy = cx.cross(camera.d).norm() * .5;
    // #pragma omp parallel for schedule(dynamic, 1)
    for(int y = 0; y < height; ++ y) {
        fprintf(stderr, "\rRendering %f%%", 100. * y / height);
        for(int x = 0; x < width; ++ x) {
            int index = (height - y - 1) * width + (width - x - 1);
            for(int sy = 0; sy < 2; ++ sy) {
                for(int sx = 0; sx < 2; ++ sx) {
                    Color_F pixel;
                    for(int s = 0; s < samples; ++ s) {
                        double r1 = 2 * rand01(), dx = r1 < 1 ? sqrt(r1) : 2 - sqrt(2 - r1);
                        double r2 = 2 * rand01(), dy = r2 < 1 ? sqrt(r2) : 2 - sqrt(2 - r2);
                        Vector3D d = cx * ((sx + dx / 2 + x) / width - .5) + cy * ((sy + dy / 2 + y) / height - .5) + camera.d;
                        pixel += radiance(Ray(camera.o + d * 120, d.norm()), 0) / samples;
                    }
                    pixels[index] += pixel.clamp() / 4.;
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