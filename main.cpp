
# include <iostream>
# include <string>

# include "renderer.h"

int main(int argc, char** argv) {
    Renderer renderer;
    renderer.load();
    renderer.render();
    renderer.save();
    return 0;
}