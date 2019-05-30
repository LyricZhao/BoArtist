# ifndef __OBJECT_H__
# define __OBJECT_H__

# include "base.h"
# include "texture.h"

class Object {
public:
    ReflectType reflect; double ior;
    Texture texture; Color_F emission;

    Object(ReflectType _reflect, double _ior=1.5, Texture _texture=Texture(), Color_F _emission=Color_F()):
        reflect(_reflect), ior(_ior), texture(_texture), emission(_emission) {}
    ~Object() { }

    void load_texture() { texture.load(); return; }
    virtual double intersect(const Ray &ray, Vector3D &gn) = 0;
    virtual Color_F color(const Vector3D &x) = 0;
    virtual void print() { std:: cout << "Error, it's in Object." << std:: endl; }
};

# endif