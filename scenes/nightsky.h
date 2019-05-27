# ifndef __NIGHT_SKY_H__
# define __NIGHT_SKY_H__

# include "../objects/api.h"

namespace nightsky {

int width = 1024, height = 768, samples = 100;

Ray camera(Vector3D(50,52,295.6), Vector3D(0,-0.042612,-1).norm());

Sphere spheres[] = {
  Sphere(2.5e3,   Vector3D(.82,.92,-2)*1e4,    Color_F(1,1,1)*.8e2,     Color_F(), DIFF), // moon
  Sphere(2.5e4, Vector3D(50, 0, 0),  Color_F(0.114, 0.133, 0.212)*1e-2,  Color_F(.216,.384,1)*0.003, DIFF), // sky
  Sphere(5e0,   Vector3D(-.2,0.16,-1)*1e4, Color_F(1.00, 0.843, 0.698)*1e2,   Color_F(), DIFF),  // star
  Sphere(5e0,   Vector3D(0,  0.18,-1)*1e4, Color_F(1.00, 0.851, 0.710)*1e2,  Color_F(), DIFF),  // star
  Sphere(5e0,   Vector3D(.3, 0.15,-1)*1e4, Color_F(0.671, 0.780, 1.00)*1e2,   Color_F(), DIFF),  // star
  Sphere(3.5e4,   Vector3D(600,-3.5e4+1, 300), Color_F(),   Color_F(.6,.8,1)*.01,  REFR),   //pool
  Sphere(5e4,   Vector3D(-500,-5e4+0, 0),   Color_F(),      Color_F(1,1,1)*.35,  DIFF),    //hill
  Sphere(16.5,  Vector3D(27,0,47),         Color_F(),              Color_F(1,1,1)*.33, DIFF), //hut
  Sphere(7,     Vector3D(27+8*sqrt(2),0,47+8*sqrt(2)),Color_F(),  Color_F(1,1,1)*.33,  DIFF), //door
  Sphere(500,   Vector3D(-1e3,-300,-3e3), Color_F(),  Color_F(1,1,1)*.351,    DIFF),  //mnt
  Sphere(830,   Vector3D(0,   -500,-3e3), Color_F(),  Color_F(1,1,1)*.354,    DIFF),  //mnt
  Sphere(490,  Vector3D(1e3,  -300,-3e3), Color_F(),  Color_F(1,1,1)*.352,    DIFF),  //mnt
}; 

const char output[] = "outputs/nightsky";
};

# endif
