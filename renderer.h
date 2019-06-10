# ifndef __RENDERER_H__
# define __RENDERER_H__

# include <algorithm>
# include <string>

# include "objects/api.h"
# include "sppm.h"

class Renderer {
private:
    int n_objects, width, height, samples, iteration_time;
    double sppm_radius, r_alpha; std:: string output;
    Ray camera; Object* *objects; Color_F *pixels;
    Ray (*ray_generator)(unsigned short *seed); 

    Color_F radiance(const Ray &ray, int depth, unsigned short *seed);
    void radiance_sppm_backtrace(std:: vector<VisiblePoint> &points, int index, const Ray &ray, int depth, unsigned short *seed, const Color_F &coef, double prob);
    void radiance_sppm_forward(KDTree *tree, const Ray &ray, int depth, const Color_F &color, unsigned short *seed, Pixel *buffer, double prob);
    bool intersect(const Ray &ray, double &t, int &id, Vector3D &n);
public:    
    Renderer(): camera(Vector3D(), Vector3D()), pixels(nullptr) {}
    ~Renderer() {
        if(pixels != nullptr) {
            std:: free(pixels);
        }
        return;
    }

    void load();
    void render();
    void render_sppm();
    void save(std:: string suffix="");
};

# endif