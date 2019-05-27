# ifndef __SMALLPT_V0_H__
# define __SMALLPT_V0_H__

# include "../objects/api.h"

namespace smallpt_v0 {

int width = 512, height = 512, samples = 100;

Ray camera(Vector3D(70, 32, 280), Vector3D(-0.15, 0.05, -1).norm());

Sphere spheres[] = {
   Sphere(1e5, Vector3D( 1e5+1,40.8,81.6), Color_F(),Color_F(.1,.25,.25),DIFF),//Left 
   Sphere(1e5, Vector3D(-1e5+99,40.8,81.6),Color_F(),Color_F(.25,.75,.25),DIFF),//Rght 
   Sphere(1e5, Vector3D(50,40.8, 1e5),     Color_F(),Color_F(.75,.75,.75),DIFF),//Back 
   Sphere(1e5, Vector3D(50,40.8,-1e5+170), Color_F(),Color_F(.25,.25,.25),DIFF),//Frnt 
   Sphere(1e5, Vector3D(50, 1e5, 81.6),    Color_F(),Color_F(.75,.75,.75),DIFF),//Botm 
   Sphere(1e5, Vector3D(50,-1e5+81.6,81.6),Color_F(),Color_F(.75,.75,.75),DIFF),//Top 
   Sphere(16.5,Vector3D(27,16.5,47),       Color_F(),Color_F(1,1,1)*.999, SPEC),//Mirr 
   Sphere(16.5,Vector3D(73,16.5,78),       Color_F(),Color_F(1,1,1)*.999, REFR),//Glas 
   Sphere(16.5,Vector3D(20,60,100),       Color_F(),Color_F(.75,.25,.25), DIFF),//Glas 
   Sphere(600, Vector3D(50,681.6-.27,81.6),Color_F(12,12,12),  Color_F(), DIFF) //Lite 
}; 

const char output[] = "outputs/smallpt_v0";
};

# endif
