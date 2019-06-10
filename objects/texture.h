# ifndef __TEXTURE_H__
# define __TEXTURE_H__

# include <string>

# include "base.h"

# include "../stb/stb_image.h"

class Texture {
public:
    int width, height, channel;
    Color_F color; stbi_uc *buffer; std:: string path;

    Texture(std:: string _path="", Color_F _color=Color_F()): path(_path), width(0), height(0), buffer(nullptr), color(_color) { }

    ~Texture() {
        if(buffer != nullptr) {
            stbi_image_free(buffer);
            buffer = nullptr;
        }
        return;
    }

    void load() {
        if(path.length()) {
            buffer = stbi_load(path.c_str(), &width, &height, &channel, 0);
            // std:: cout << width << " " << height << std:: endl;
        }
        return;
    }

    bool none() const {
        return width == 0;
    }

    Color_F pixel(int x, int y) const {
        y = height - y - 1;
        // std:: cout << x << " " << y << std:: endl;
        // if(x >= width || y >= height) while(1) std:: cout << width << " " << height << std:: endl;
        int index = (y * width + x) * channel;
        return Color_F(buffer[index], buffer[index + 1], buffer[index + 3]) / 255.;
    }

    Color_F get(double dx, double dy) const {
        if(none()) return color;
        int x = (int)(dx * 20) % width, y = (int)(dy * 20) % height;
        if(x < 0) x += width;
        if(y < 0) y += height;
        return pixel(x, y);
    }
};

# endif