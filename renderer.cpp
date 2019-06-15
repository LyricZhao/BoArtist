# include <cassert>
# include <cstdio>
# include <cstring>
# include <omp.h>

# include "renderer.h"
# include "utils.h"
# include "sppm.h"

// # define L_DEBUG_MODE
# define TO_RENDER dinosaurs

# include "scenes/debug.h"
# include "scenes/pt_sky.h"
# include "scenes/bretesche.h"
# include "scenes/dinosaurs.h"

void Renderer:: load() {
    # ifdef SPPM_MODE
    std:: cout << "Using SPPM mode." << std:: endl;
    # else
    std:: cout << "Using PT mode." << std:: endl;
    # endif

    /* Parameters */
    width = TO_RENDER:: width, height = TO_RENDER:: height;
    n_objects = sizeof(TO_RENDER:: objects) / sizeof(Object*);
    objects = TO_RENDER:: objects;
    samples = TO_RENDER:: samples; /* Different meanings in sppm and normal mode */
    camera = TO_RENDER:: camera;
    ray_generator = TO_RENDER:: ray_generator;
    output = TO_RENDER:: output;
    camera_scale = TO_RENDER:: camera_scale;

    # ifdef SPPM_MODE
    iteration_time = TO_RENDER:: iteration_time;
    sppm_radius = TO_RENDER:: sppm_radius;
    r_alpha = TO_RENDER:: r_alpha;
    energy = TO_RENDER:: energy;
    dof = TO_RENDER:: dof;
    # endif
    
    std:: cout << "Image pararmeters: " << std:: endl;
    std:: cout << " - (w, h): (" << width << ", " << height << ")" << std:: endl;
    std:: cout << " - Objects: " << n_objects << std:: endl;
    std:: cout << " - Samples: " << samples << std:: endl;
    std:: cout << " - Camera Position: " << camera.o << std:: endl;
    std:: cout << " - Camera Direction: " << camera.d << std:: endl;
    std:: cout << " - Camera Scale: " << camera_scale << std:: endl;
    # ifdef SPPM_MODE
    std:: cout << " - Iteration times: " << iteration_time << std:: endl;
    std:: cout << " - Radius: " << sppm_radius << std:: endl;
    std:: cout << " - Radius Alpha: " << r_alpha << std:: endl;
    std:: cout << " - Energy: " << energy << std:: endl;
    # endif
    std:: cout << " - Output filename: " << output << std:: endl;
    std:: cout << std:: endl;

    /* Data loading */
    std:: cout << "Loading scene ... " << std:: flush;
    pixels = (Color_F*) std:: malloc(sizeof(Color_F) * width * height);
    for(int i = 0; i < n_objects; ++ i) objects[i] -> load();
    for(int i = 0; i < n_objects; ++ i) objects[i] -> load_texture();
    std:: cout << "ok!" << std:: endl;

    /* Debuging zone */
    # ifdef L_DEBUG_MODE
        std:: cout << "Debug mode is opened." << std:: endl;
    # endif
    return;
}

bool Renderer:: intersect(const Ray &ray, double &t, int &id, Vector3D &n, Vector3D &x, Color_F &f) {
    double d; t = inf; Vector3D t_norm; Color_F t_f;
    for(int i = 0; i < n_objects; ++ i) {
        if((d = objects[i] -> intersect(ray, t_norm, t_f)) && d < t)
            t = d, id = i, n = t_norm, f = t_f, x = ray.o + ray.d * t;
    }
    return t < inf;
}

# ifndef SPPM_MODE
Color_F Renderer:: radiance(const Ray &ray, int depth, unsigned short *seed) {
    double t; int id, in = 0; Vector3D n, x; Color_F f;
    if(depth > 10 || !intersect(ray, t, id, n, x, f)) return Color_F();
    Object *object = objects[id];
    Vector3D nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    ++ depth; double mx = f.max();
    if(depth > 5) {
        if(erand48(seed) < mx) f /= mx;
        else return object -> emission;
    }
    switch(object -> reflect) {
        case DIFF: {
            double r1 = 2 * M_PI * erand48(seed), r2 = M_PI * erand48(seed);
            Vector3D w = nl, u = (fabs(w.x) > .1 ? Vector3D(0, 1): Vector3D(1)).cross(w).norm(), v = w.cross(u);
            Vector3D d = ((u * cos(r1) + v * sin(r1)) * cos(r2) + w * sin(r2)).norm();
            Color_F tmp = radiance(Ray(x, d), depth, seed);
            return Vector3D(object -> emission) + f.mul(tmp);
        }
        case SPEC: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            return Vector3D(object -> emission) + f.mul(radiance(reflect_ray, depth, seed));
        }
        case REFR: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
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
    return Color_F();
}
# endif

/* TODO: Object has many attributes */
# ifdef SPPM_MODE
void Renderer:: radiance_sppm_backtrace(std:: vector<VisiblePoint> &points, int index, const Ray &ray, int depth, unsigned short *seed, const Color_F &coef, double prob, Pixel *image) {
    double t; int id, in = 0; Vector3D n, x; Color_F f;
    if(depth > 10 || coef.max() < eps || prob < eps || !intersect(ray, t, id, n, x, f)) return;
    Object *object = objects[id];
    /* Note: n must be pointing out, nl = -n while light is going out */
    Vector3D nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    ++ depth;
    double mx = f.max();
    if(depth > 5) {
        if(erand48(seed) < mx) f /= mx;
        else return;
    }
    f = coef.mul(f);
    switch(object -> reflect) {
        case DIFF: {
            points.push_back(VisiblePoint(index, x, f, nl, image[index].r, prob));
            break;
        }
        case SPEC: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob, image);
            break;
        }
        case REFR: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            Vector3D d = ray.d.refract(nl, in ? 1 : object -> ior, in ? object -> ior : 1);
            if(d.length2() < eps) {
                radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob, image);
            } else {
                double a = object -> ior - 1, b = object -> ior + 1, r0 = a * a / (b * b), c = 1 - (in ? -ray.d.dot(nl) : d.dot(n));
                double re = r0 + (1 - r0) * c * c * c * c * c, tr = 1 - re, p = .25 + .5 * re, rp = re / p, tp = tr / (1 - p);
                if(depth > 2) {
                    if(erand48(seed) < p) {
                        radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob * rp, image);
                    } else {
                        radiance_sppm_backtrace(points, index, Ray(x, d), depth, seed, f, prob * tp, image);
                    }
                } else {
                    radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob * re, image);
                    radiance_sppm_backtrace(points, index, Ray(x, d), depth, seed, f, prob * tr, image);
                }
            }
            break;
        }
        default: assert(0);
    }
    return;
}

void Renderer:: radiance_sppm_forward(KDTree *tree, const Ray &ray, int depth, const Color_F &color, unsigned short *seed, Pixel *buffer, double prob) {
    double t; int id, in = 0; Vector3D n, x; Color_F f;
    if(depth > 10 || prob < eps || color.max() < eps || !intersect(ray, t, id, n, x, f)) return;
    Object *object = objects[id];
    Vector3D nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    ++ depth;
    double mx = f.max();
    if(depth > 5) {
        if(erand48(seed) < mx) f /= mx;
        else return;
    }
    f = color.mul(f);
    switch(object -> reflect) {
        case DIFF: {
            tree -> query(x, nl, f, buffer);
            double r1 = 2 * M_PI * erand48(seed), r2 = erand48(seed), s_r2 = sqrt(r2);
            Vector3D w = nl, u = (fabs(w.x) > .1 ? Vector3D(0, 1): Vector3D(1)).cross(w).norm(), v = w.cross(u);
            Vector3D d = ((u * cos(r1) + v * sin(r1)) * s_r2 + w * sqrt(1 - r2)).norm();
            radiance_sppm_forward(tree, Ray(x, d), depth, f, seed, buffer, prob);
            break;
        }
        case SPEC: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            radiance_sppm_forward(tree, reflect_ray, depth, f, seed, buffer, prob);
            break;
        }
        case REFR: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            Vector3D d = ray.d.refract(nl, in ? 1 : object -> ior, in ? object -> ior : 1);
            if(d.length2() < eps)
                return radiance_sppm_forward(tree, reflect_ray, depth, f, seed, buffer, prob);
            double a = object -> ior - 1, b = object -> ior + 1, r0 = a * a / (b * b), c = 1 - (in ? -ray.d.dot(nl) : d.dot(n));
            double re = r0 + (1 - r0) * c * c * c * c * c, tr = 1 - re, p = .25 + .5 * re, rp = re / p, tp = tr / (1 - p);
            return depth > 2 ? (erand48(seed) < p ?
                radiance_sppm_forward(tree, reflect_ray, depth, f, seed, buffer, prob * rp) : radiance_sppm_forward(tree, Ray(x, d), depth, f, seed, buffer, prob * tp)):
                (radiance_sppm_forward(tree, reflect_ray, depth, f, seed, buffer, prob * re), radiance_sppm_forward(tree, Ray(x, d), depth, f, seed, buffer, prob * tr));
            break;
        }
    }
}
# endif

# ifndef SPPM_MODE
void Renderer:: render() {
    Vector3D cx = Vector3D(width * camera_scale / height);
    Vector3D cy = cx.cross(camera.d).norm() * camera_scale;
    std:: cout << "Rendering ... " << std:: endl;
# ifndef L_DEBUG_MODE
    #pragma omp parallel for schedule(dynamic, 1)
# endif    
    for(int y = 0; y < height; ++ y) {
        fprintf(stderr, "Current progress: %d/%d\n", y + 1, height);
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
                        pixel += radiance(Ray(camera.o + d * 140., d.norm()), 0, seed);
                    }
                    pixels[index] += (pixel / samples).clamp() / 4.;
                }
            }
        }
    }
    std:: cout << std:: endl;
    return;
}
# endif

# ifdef SPPM_MODE
void Renderer:: render() {
    /* Data allocation */
    std:: cout << "Allocing data ... " << std:: flush;
    int n_threads = omp_get_max_threads();
    Vector3D cx = Vector3D(width * camera_scale / height);
    if(fabs(fabs(camera.d.x) - 1) < eps)
        cx = Vector3D(0, 1, 0);
    Vector3D cy = cx.cross(camera.d).norm() * camera_scale;
    Pixel **pixels_workers = (Pixel **) std:: malloc(sizeof(Pixel*) * n_threads);
    Pixel *image = (Pixel *) std:: malloc(sizeof(Pixel) * width * height);
    memset(image, 0, sizeof(Pixel) * width * height);
    for(int i = 0; i < width * height; ++ i)
        image[i].r = sppm_radius;
    for(int i = 0; i < n_threads; ++ i)
        pixels_workers[i] = (Pixel *) std:: malloc(sizeof(Pixel) * height * width);
    KDTree *tree = new KDTree();
    std:: cout << "ok !" << std:: endl;
    std:: cout << std:: endl;

    std:: cout << "Using " << n_threads << " OpenMP threads." << std:: endl;
    std:: cout << "Begin iterations: " << std:: endl;
    for(int iter = 0; iter < iteration_time; ++ iter) {
        /* Backtrace */
        std:: cout << "Iteration #[" << iter + 1 << "/" << iteration_time << "]:" << std:: endl;
        std:: cout << " - Samples: " << samples << std:: endl;
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Backtracing ... " << std:: flush;
        std:: vector<VisiblePoint> points[n_threads];
        # pragma omp parallel for schedule(dynamic, 1)
        for(int y = 0; y < height; ++ y) {
            int thread_id = omp_get_thread_num();
            for(int x = 0; x < width; ++ x) {
                int index = (height - y - 1) * width + (width - x - 1);
                for(int sy = 0; sy < 2; ++ sy) {
                    for(int sx = 0; sx < 2; ++ sx) {
                        unsigned short seed[3] = {(unsigned short)x, (unsigned short)y, (unsigned short)time(nullptr)};
                        double r1 = 2 * erand48(seed), dx = r1 < 1 ? sqrt(r1) : 2 - sqrt(2 - r1);
                        double r2 = 2 * erand48(seed), dy = r2 < 1 ? sqrt(r2) : 2 - sqrt(2 - r2);
                        Vector3D d = cx * ((sx + dx / 2 + x) / width - .5) + cy * ((sy + dy / 2 + y) / height - .5) + camera.d;
                        double theta = 2 * M_PI * erand48(seed), r_rd = erand48(seed);
                        Vector3D pop = camera.o + d * 217, o_r = camera.o + (cx * cos(theta) + cy * sin(theta)) * r_rd * dof;
                        radiance_sppm_backtrace(points[thread_id], index, Ray(pop, (pop - o_r).norm()), 0, seed, Color_F(1, 1, 1), 1, image);
                    }
                }
            }
        }
        std:: cout << "ok !" << std:: endl;

        /* Build KD-tree */
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Building KD-tree ... " << std:: flush;
        tree -> build(points, n_threads);
        std:: cout << "ok !" << std:: endl;
        
        /* Shoot random light */
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Shooting lights ... " << std:: flush;
        int per_samples = samples / n_threads;
        # pragma omp parallel
        {
            int thread_id = omp_get_thread_num();
            unsigned short seed[3] = {(unsigned short)iter, (unsigned short)thread_id, (unsigned short)time(nullptr)};
            memset(pixels_workers[thread_id], 0, sizeof(Pixel) * width * height);
            for(int s = 0; s < per_samples; ++ s) {
                int thread_id = omp_get_thread_num();
                Ray light = ray_generator(seed);
                radiance_sppm_forward(tree, light, 0, Color_F(1, 1, 1), seed, pixels_workers[thread_id], 1.);
            }
        }
        std:: cout << "ok !" << std:: endl;

        /* Collect result */
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Collecting results ... " << std:: flush;
        # pragma omp parallel for
        for(int i = 0; i < height * width; ++ i) {
            Pixel pixel;
            for(int j = 0; j < n_threads; ++ j) pixel = pixel + pixels_workers[j][i];
            image[i].modify(pixel, r_alpha);
            pixels[i] = image[i].value() * energy;
        }
        save("_" + std:: to_string(iter));
        std:: cout << "ok !" << std:: endl;

        /* Free KD-tree */
        std:: cout << std:: endl;
        tree -> free();
    }

    /* Free resources */
    for(int i = 0; i < n_threads; ++ i)
        std:: free(pixels_workers[i]);
    std:: free(pixels_workers);
    std:: free(image);
    tree -> free();
    return;
}
# endif

void Renderer:: save(std:: string suffix) {
    if(!suffix.length()) std:: cout << "Saving result ... " << std:: flush;
    FILE *file = fopen((output + suffix + ".ppm").c_str(), "w");
    fprintf(file, "P6\n%d %d\n%d\n", width, height, 255);
    for(int i = 0; i < width * height; ++ i)
        fprintf(file, "%c%c%c", pixels[i].r(), pixels[i].g(), pixels[i].b());
    fflush(file);
    if(!suffix.length()) std:: cout << "ok !" << std:: endl;
    return;
}