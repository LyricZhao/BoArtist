# include <cassert>
# include <cstdio>
# include <cstring>

# include "renderer.h"
# include "utils.h"
# include "sppm.h"

# define SPPM_MODE
# define L_DEBUG_MODE
# define TO_RENDER debug_scene

# include "scenes/debug.h"

/* TODO: Add SPPM control macro */

void Renderer:: load() {
    
    /* Parameters */
    width = TO_RENDER:: width, height = TO_RENDER:: height;
    n_objects = sizeof(TO_RENDER:: objects) / sizeof(Object*);
    objects = TO_RENDER:: objects;
    samples = TO_RENDER:: samples;
    camera = TO_RENDER:: camera;
    ray_generator = TO_RENDER:: ray_generator;
    iteration_time = TO_RENDER:: iteration_time;
    sppm_radius = TO_RENDER:: sppm_radius;
    r_alpha = TO_RENDER:: r_alpha;
    output = TO_RENDER:: output;
    std:: cout << "Image pararmeters: " << std:: endl;
    std:: cout << " - (w, h): (" << width << ", " << height << ")" << std:: endl;
    std:: cout << " - Objects: " << n_objects << std:: endl;
    std:: cout << " - Samples: " << samples << std:: endl;
    std:: cout << " - Camera Position: " << camera.o << std:: endl;
    std:: cout << " - Camera Direction: " << camera.d << std:: endl;
    std:: cout << " - Iteration times: " << iteration_time << std:: endl;
    std:: cout << " - Radius: " << sppm_radius << std:: endl;
    std:: cout << " - Radius Alpha: " << r_alpha << std:: endl;
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
        /* Debuging tests */
        std:: cout << "Debug mode is opened." << std:: endl;
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

/* TODO: Object has many attributes */ 
void Renderer:: radiance_sppm_backtrace(std:: vector<VisiblePoint> &points, int index, const Ray &ray, int depth, unsigned short *seed, const Color_F &coef, double prob) {
    double t; int id, in = 0; Vector3D n;
    if(depth > 10 || coef.max() < eps || prob < eps || !intersect(ray, t, id, n)) return;
    Object *object = objects[id];
    /* Note: n must be pointing out, nl = -n while light is going out */
    Vector3D x = ray.o + ray.d * t, nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    Color_F f = object -> color(x);
    /*
    TODO: Consider to delete
    double mx = f.max();
    if(++ depth > 5) {
        if(erand48(seed) < mx) f /= mx;
        else return;
    }
    */
    f = coef.mul(f);
    switch(object -> reflect) {
        case DIFF: {
            points.push_back(VisiblePoint(index, x, f, nl, sppm_radius, prob));
            break;
        }
        case SPEC: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob);
            break;
        }
        case REFR: {
            Ray reflect_ray = Ray(x, ray.d.reflect(nl));
            Vector3D d = ray.d.refract(nl, in ? 1 : object -> ior, in ? object -> ior : 1);
            if(d.length2() < eps) {
                radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob);
            } else {
                double a = object -> ior - 1, b = object -> ior + 1, r0 = a * a / (b * b), c = 1 - (in ? -ray.d.dot(nl) : d.dot(n));
                double re = r0 + (1 - r0) * c * c * c * c * c, tr = 1 - re, p = .25 + .5 * re, rp = re / p, tp = tr / (1 - p);
                if(depth > 2) {
                    if(erand48(seed) < p) {
                        radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob * rp);
                    } else {
                        radiance_sppm_backtrace(points, index, Ray(x, d), depth, seed, f, prob * tp);
                    }
                } else {
                    radiance_sppm_backtrace(points, index, reflect_ray, depth, seed, f, prob * re);
                    radiance_sppm_backtrace(points, index, Ray(x, d), depth, seed, f, prob * tr);
                }
            }
            break;
        }
        default: assert(0);
    }
    return;
}

void Renderer:: radiance_sppm_forward(KDTree *tree, const Ray &ray, int depth, const Color_F &color, unsigned short *seed, Pixel *buffer, double prob) {
    double t; int id, in = 0; Vector3D n;
    if(depth > 10 || prob < eps || color.max() < eps || !intersect(ray, t, id, n)) return;
    Object *object = objects[id];
    Vector3D x = ray.o + ray.d * t, nl = n.dot(ray.d) < 0 ? in = 1, n : -n;
    Color_F f = object -> color(x);
    /*
    TODO: Consider to delete
    double mx = f.max();
    if(++ depth > 5) {
        if(erand48(seed) < mx) f /= mx;
        else {
            tree -> query(x, nl, color, buffer);
            return;
        }
    }
    */
    f = color.mul(f);
    switch(object -> reflect) {
        case DIFF: {
            tree -> query(x, nl, f, buffer);
            double r1 = 2 * M_PI * erand48(seed), r2 = M_PI * erand48(seed);
            Vector3D w = nl, u = (fabs(w.x) > .1 ? Vector3D(0, 1): Vector3D(1)).cross(w).norm(), v = w.cross(u);
            Vector3D d = ((u * cos(r1) + v * sin(r1)) * cos(r2) + w * sin(r2)).norm();
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

void Renderer:: render() {
    Vector3D cx = Vector3D(width * .2 / height);
    Vector3D cy = cx.cross(camera.d).norm() * .2;
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

/* TODO: OpenMP */
void Renderer:: render_sppm() {
    /* Data allocation */
    std:: cout << "Allocing data ... " << std:: flush;
    Vector3D cx = Vector3D(width * .2 / height);
    Vector3D cy = cx.cross(camera.d).norm() * .2;
    Pixel *pixels_worker = (Pixel *) std:: malloc(width * height * sizeof(Pixel));
    memset(pixels_worker, 0, sizeof(Pixel) * width * height);
    KDTree *tree = new KDTree();
    std:: cout << "ok !" << std:: endl;
    std:: cout << std:: endl;

    std:: cout << "Begin iterations: " << std:: endl;
    for(int iter = 0; iter < iteration_time; ++ iter) {
        /* Backtrace */
        std:: cout << "Iteration #[" << iter + 1 << "/" << iteration_time << "]:" << std:: endl;
        std:: cout << " - Radius: " << sppm_radius << std:: endl;
        std:: cout << " - Samples: " << samples << std:: endl;
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Backtracing ... " << std:: flush;
        std:: vector<VisiblePoint> points;
        for(int y = 0; y < height; ++ y) {
            for(int x = 0; x < width; ++ x) {
                int index = (height - y - 1) * width + (width - x - 1);
                for(int sy = 0; sy < 2; ++ sy) {
                    for(int sx = 0; sx < 2; ++ sx) {
                        unsigned short seed[3] = {(unsigned short)x, (unsigned short)y, (unsigned short)time(nullptr)};
                        double r1 = 2 * erand48(seed), dx = r1 < 1 ? sqrt(r1) : 2 - sqrt(2 - r1);
                        double r2 = 2 * erand48(seed), dy = r2 < 1 ? sqrt(r2) : 2 - sqrt(2 - r2);
                        Vector3D d = cx * ((sx + dx / 2 + x) / width - .5) + cy * ((sy + dy / 2 + y) / height - .5) + camera.d;
                        radiance_sppm_backtrace(points, index, Ray(camera.o + d, d.norm()), 0, seed, Color_F(1, 1, 1), 1);
                    }
                }
            }
        }
        std:: cout << "ok !" << std:: endl;

        /* Build KD-tree */
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Building KD-tree ... " << std:: flush;
        tree -> build(points);
        std:: cout << "ok !" << std:: endl;
        
        /* Shoot random light */
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Shooting lights ... " << std:: flush;
        unsigned short seed[3] = {(unsigned short)iter, (unsigned short)0, (unsigned short)time(nullptr)}; // TODO: omp_id
        for(int s = 0; s < samples; ++ s) {
            Ray light = ray_generator(seed);
            radiance_sppm_forward(tree, light, 0, Color_F(1, 1, 1), seed, pixels_worker, 1.);
        }
        std:: cout << "ok !" << std:: endl;

        /* Collect result */
        std:: cout << " - [" << iter + 1 << "/" << iteration_time << "] Collecting results ... " << std:: flush;
        for(int i = 0; i < height * width; ++ i) {
            pixels[i] = pixels_worker[i].value();
            // if(pixels_worker[i].count) pixels_worker[i].print();
        }
        save(std:: to_string(iter));
        std:: cout << "ok !" << std:: endl;

        /* Change coef */
        samples /= sqrt(r_alpha);
        sppm_radius *= r_alpha;
        std:: cout << std:: endl;
    }

    /* Free resources */
    std:: free(pixels_worker);
    tree -> free();
    return;
}

void Renderer:: save(std:: string suffix) {
    if(!suffix.length()) std:: cout << "Saving result ... " << std:: flush;
    FILE *file = fopen((output + suffix + ".ppm").c_str(), "w");
    fprintf(file, "P6\n%d %d\n%d\n", width, height, 255);
    for(int i = 0; i < width * height; ++ i)
        fprintf(file, "%c%c%c", pixels[i].r(), pixels[i].g(), pixels[i].b());
    if(!suffix.length()) std:: cout << "ok !" << std:: endl;
    return;
}