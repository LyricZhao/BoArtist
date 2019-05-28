
# include <iostream>
# include <string>

# define __MAIN_CPP_COMPILE__
# include "renderer.h"
# undef __MAIN_CPP_COMPILE__

int main(int argc, char** argv) {
    Renderer renderer;
    renderer.load();
    renderer.render();
    renderer.save();
    return 0;
}