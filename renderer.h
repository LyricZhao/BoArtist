# ifndef __RENDERER_H__
# define __RENDERER_H__

# include <algorithm>
# include <string>

# include "objects/api.h"
# include "sppm.h"

// # define SPPM_MODE

class Renderer {
private:
    int n_objects, width, height, samples;
    std:: string output;
    Ray camera; Object* *objects; Color_F *pixels;
    Ray (*ray_generator)(unsigned short *seed); 
    double camera_scale;

    # ifdef SPPM_MODE
    int iteration_time;
    double sppm_radius, r_alpha, energy;
    # endif

    Color_F radiance(const Ray &ray, int depth, unsigned short *seed);
    # ifdef SPPM_MODE
    void radiance_sppm_backtrace(std:: vector<VisiblePoint> &points, int index, const Ray &ray, int depth, unsigned short *seed, const Color_F &coef, double prob, Pixel *image);
    void radiance_sppm_forward(KDTree *tree, const Ray &ray, int depth, const Color_F &color, unsigned short *seed, Pixel *buffer, double prob);
    # endif
    bool intersect(const Ray &ray, double &t, int &id, Vector3D &n, Vector3D &x, Color_F &f);
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
    void save(std:: string suffix="");
};

# endif