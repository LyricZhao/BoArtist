# ifndef __RENDERER_H__
# define __RENDERER_H__

# include <algorithm>
# include <string>

# include "objects/api.h"

class Renderer {
private:
    int n_objects, width, height, samples; std:: string output;
    Ray camera; Object* *objects; Color_F *pixels;

    Color_F radiance(const Ray &ray, int depth, unsigned short *seed);
    bool intersect(const Ray &ray, double &t, int &id);
public:    
    Renderer(): camera(Vector3D(), Vector3D()), pixels(nullptr) {}
    ~Renderer() {
        if(pixels != nullptr) {
            free(pixels);
        }
    }

    void load();
    void render();
    void save();
};

# endif