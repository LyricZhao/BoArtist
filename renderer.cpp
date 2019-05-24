# include <cassert>
# include <cstdio>

# include "renderer.h"
# include "utils.h"

void Renderer:: load() {
    width = TO_RENDER:: width, height = TO_RENDER:: height;
    n_sphere = sizeof(TO_RENDER:: spheres) / sizeof(Sphere);
    spheres = TO_RENDER:: spheres, samples = TO_RENDER:: samples, camera = TO_RENDER:: camera;
    output = std:: string(TO_RENDER:: output);
    pixels = (Color_F*) std:: malloc(sizeof(Color_F) * width * height);
    return;
}

Color_F Renderer:: radiance(Ray ray, int id) {

}

void Renderer:: render() {
    Vector3D cx = Vector3D(width * .5135 / height);
    Vector3D cy = cx.cross(camera.d).norm() * .5135;
    for(int y = 0; y < height; ++ y) {
        for(int x = 0; x < width; ++ x) {
            int index = (height - y - 1) * width + x;
            for(int sy = 0; sy < 2; ++ sy) {
                for(int sx = 0; sx < 2; ++ sx) {
                    Color_F pixel;
                    for(int s = 0; s < samples; ++ s) {
                        double r1 = 2 * rand01(), dx = r1 < 1 ? sqrt(r1 - 1) : 1 - sqrt(2 - r1);
                        double r2 = 2 * rand01(), dy = r2 < 1 ? sqrt(r2 - 1) : 1 - sqrt(2 - r2);
                        Vector3D d = cx * (((sx + .5 + dx) / 2 + x) / width - .5) + cy * (((sy + .5 + dy) / 2 + y) / height - .5) + camera.d;
                        pixel += radiance(Ray(camera.o + d * 140, d.norm()), 0) / samples;
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
    fprintf(file, "P3\n%d %d\n%d\n", width, height, 255);
    for(int i = 0; i < width * height; ++ i)
        fprintf(file, "%d %d %d", pixels[i].r(), pixels[i].g(), pixels[i].b());
    return;
}