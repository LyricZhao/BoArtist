
# include <iostream>
# include <string>

# include "renderer.h"

int main(int argc, char** argv) {
    Renderer renderer;
    renderer.load();
    renderer.render_sppm();
    renderer.save();
    return 0;
}