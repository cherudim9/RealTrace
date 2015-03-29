#include <iostream>
#include "include/basic_types.h"
#include "include/bmpio.h"
#include "include/basic_geometry.h"
#include "include/utility.h"
#include "include/basic_tracer.h"
#include <vector>

using namespace std;

const int width=1280;
const int height=1280;

vector<Renderer*> obj_vec;

int main(){

  int choo=2;

  if (choo==2){

  obj_vec.push_back(new PlaneT( Renderer(0.0, 0.0, 0.5, PixelColor(113, 83, 38), false),
                               PointT(0.0, -10.0, 0.0), 
                               PointT(0.0, 1.0, 0.0)));
  obj_vec.push_back(new SphereT( Renderer(1.0, 0.0, 0.5, PixelColor(230, 77, 166), false),
                                PointT(-10.0, 0.0, -10.0),
                                3.0));
  obj_vec.push_back(new SphereT( Renderer(0.5, 0.3, 0.8, PixelColor(230, 230, 230), false),
                                PointT(10.0, 0.0, 0.0),
                                4.0));
  obj_vec.push_back(new SphereT( Renderer(0.0, 0.0, 0.0, PixelColor(255,255,0), true, 3.0),
                                PointT(0.0, 20.0, 0.0),
                                2.0));
  obj_vec.push_back(new SphereT( Renderer(0.0, 0.0, 0.0, PixelColor(0,255,255), true, 1.0),
                                PointT(20.0, 0.0, 0.0),
                                4.0));

  }else{
      obj_vec.push_back(new SphereT( Renderer(0.0, 0.0, 0.0, PixelColor(255,255,255), true, 3.0),
                                     PointT(0.0, 20.0, 0.0),
                                     2.0));
      obj_vec.push_back(new SphereT( Renderer(0.5, 0.0, 0.5, PixelColor(230, 77, 166), false),
                                     PointT(0.0, 0.0, 0.0),
                                     3.0));
  }

  PointT camera(0.0, 5.0, -150.0), eye(0.0, 0.0, 1.0), head(0.0, 1.0, 0.0);
  
  PixelColor bmp_data[width*height+10];

  freopen("a.txt","w",stdout);
  double x=-10.0, scale=1.0/width*height, y;
  for(int i=0; i<width; i++){
    y=-10.0*scale;
    for(int j=0; j<height; j++){
      RayT ray(camera, PointT(x, y, -100.0) - camera);
      PointT color;
      Tracer::RayTrace(ray, obj_vec, color, 0);
      bmp_data[i+j*width]=PointT::ToPixelColor(color);
      y+=20.0*scale/height;
    }
    x+=20.0/width;
  }

  OutputBmp(width, height, bmp_data, "result.bmp"); 

  return 0;
}
