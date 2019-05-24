# ifndef __RENDERER_H__
# define __RENDERER_H__

# define TO_RENDER smallpt_v0

# include <algorithm>
# include <string>

# include "objects/api.h"
# include "scenes/smallpt_v0.h"

class Renderer {
private:
    int n_sphere, width, height, samples; std:: string output;
    Ray camera; Sphere *spheres; Color_F *pixels;

    Color_F radiance(Ray ray, int id);
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