#include <iostream>
#include "include/basic_types.h"
#include "include/bmpio.h"
#include "include/basic_geometry.h"
#include "include/utility.h"
#include "include/basic_tracer.h"
#include "include/basic_texture.h"
#include <vector>

using namespace std;

const int width=1280;
const int height=1280;

PointT Color[2000][2000];

vector<Renderer*> obj_vec;

int main(){

  BasicTextureVector textures;

  textures.AddTexture("textures/Wooden_desks.tga");
  textures.AddTexture("textures/rock_textures/rock_02_hm.tga");

  int choo=1;

  if (choo==1){

    obj_vec.resize(9);

    obj_vec[0]=new PlaneT();
    obj_vec[0]->SetReflect(0.0); 
    obj_vec[0]->SetRefract(0.0);
    obj_vec[0]->SetDiffuse(1.0);
    obj_vec[0]->SetRefractIndex(0.0);
    obj_vec[0]->SetColor(PixelColor(113,83,38));
    ((PlaneT*)obj_vec[0])->SetO(PointT(0.0, -10.0, 0.0));
    ((PlaneT*)obj_vec[0])->SetNormal(PointT(0.0, 1.0, 0.0));
    obj_vec[0]->SetTexture(textures.GetTexture(0));

    obj_vec[1]=new PlaneT();
    obj_vec[1]->SetReflect(0.5); 
    obj_vec[1]->SetRefract(0.0);
    obj_vec[1]->SetDiffuse(1.0);
    obj_vec[1]->SetRefractIndex(0.0);
    obj_vec[1]->SetColor(PixelColor(0,0,230));
    ((PlaneT*)obj_vec[1])->SetO(PointT(40.0, 0.0, 0.0));
    ((PlaneT*)obj_vec[1])->SetNormal(PointT(-1.0, 0.0, 0.0));

    obj_vec[2]=new PlaneT();
    obj_vec[2]->SetReflect(0.5); 
    obj_vec[2]->SetRefract(0.0);
    obj_vec[2]->SetDiffuse(1.0);
    obj_vec[2]->SetRefractIndex(0.0);
    obj_vec[2]->SetColor(PixelColor(230,230,0));
    ((PlaneT*)obj_vec[2])->SetO(PointT(-25.0, 0.0, 0.0));
    ((PlaneT*)obj_vec[2])->SetNormal(PointT(1.0, 0.0, 0.0));

    obj_vec[3]=new PlaneT();
    obj_vec[3]->SetReflect(0.7); 
    obj_vec[3]->SetRefract(0.0);
    obj_vec[3]->SetDiffuse(0.0);
    obj_vec[3]->SetRefractIndex(0.0);
    obj_vec[3]->SetColor(PixelColor(255,255,255));
    ((PlaneT*)obj_vec[3])->SetO(PointT(0.0, 0.0, 30.0));
    ((PlaneT*)obj_vec[3])->SetNormal(PointT(0.0, 0.0, 1.0));

    obj_vec[4]=new SphereT();
    obj_vec[4]->SetReflect(0.0); 
    obj_vec[4]->SetRefract(0.0);
    obj_vec[4]->SetDiffuse(1.5);
    obj_vec[4]->SetRefractIndex(1.7);
    obj_vec[4]->SetColor(PixelColor(40,255,40));
    ((SphereT*)obj_vec[4])->SetCenter(PointT(-10.0, 0.0, 20.0));
    ((SphereT*)obj_vec[4])->SetRadius(2.0);

    obj_vec[5]=new SphereT();
    obj_vec[5]->SetReflect(0.0); 
    obj_vec[5]->SetRefract(1.0);
    obj_vec[5]->SetDiffuse(1.5);
    obj_vec[5]->SetRefractIndex(1.7);
    obj_vec[5]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[5])->SetCenter(PointT(-10.0, 0.0, -10.0));
    ((SphereT*)obj_vec[5])->SetRadius(7.0);

    obj_vec[6]=new SphereT();
    obj_vec[6]->SetReflect(1.0); 
    obj_vec[6]->SetRefract(0.0);
    obj_vec[6]->SetDiffuse(1.5);
    obj_vec[6]->SetRefractIndex(1.7);
    obj_vec[6]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[6])->SetCenter(PointT(10.0, 0.0, 2.0));
    ((SphereT*)obj_vec[6])->SetRadius(10.0);

    obj_vec[7]=new SphereT();
    obj_vec[7]->SetReflect(0.0); 
    obj_vec[7]->SetRefract(0.0);
    obj_vec[7]->SetDiffuse(2.0);
    obj_vec[7]->SetRefractIndex(1.7);
    obj_vec[7]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[7])->SetCenter(PointT(0.0, 5.0, -15.0));
    ((SphereT*)obj_vec[7])->SetRadius(3.0);
    obj_vec[7]->SetTexture(textures.GetTexture(1));

    obj_vec[8]=new SphereT();
    obj_vec[8]->SetReflect(0.0); 
    obj_vec[8]->SetRefract(0.0);
    obj_vec[8]->SetDiffuse(0.0);
    obj_vec[8]->SetRefractIndex(1.7);
    obj_vec[8]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[8])->SetCenter(PointT(0.0, 50.0, -50.0));
    ((SphereT*)obj_vec[8])->SetRadius(0.5);
    obj_vec[8]->SetLight(0.6);

  }else{

  }

  PointT camera(0.0, 8.0, -50.0), eye(0.0, 0.0, 1.0), head(0.0, 1.0, 0.0);
  
  PixelColor bmp_data[width*height+10];

  //freopen("a.txt","w",stdout);
  double x=-10.0, scale=1.0/width*height, y;
  for(int i=0; i<width; i++){
    y=-10.0*scale;
    for(int j=0; j<height; j++){
      RayT ray(camera, PointT(x, y, 10));
      PointT color;
      bool debug=0;
      Tracer::RayTrace(ray, obj_vec, color, 0, 1.0, debug);
      Color[i][j]=color;
      bmp_data[i+j*width]=PointT::ToPixelColor(color);
      y+=20.0*scale/height;
    }
    x+=20.0/width;
  }

  OutputBmp(width, height, bmp_data, "result.bmp"); 

  return 0;
}
