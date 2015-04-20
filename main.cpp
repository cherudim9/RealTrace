#include <iostream>
#include "include/basic_types.h"
#include "include/bmpio.h"
#include "include/basic_geometry.h"
#include "include/utility.h"
#include "include/basic_tracer.h"
#include "include/basic_texture.h"
#include "include/view.h"
#include "include/object_descripter.h"
#include <vector>
#include <sstream>

using namespace std;

PointT Color[2000][2000];

vector<Renderer*> obj_vec, lights;

int ToNum(char *c){
  stringstream O;
  O<<string(c);
  int ret;
  O>>ret;
  return ret;
}

int main(int argc, char **argv){

  int width=100, height=100;

  if (argc>=3){
    width=ToNum(argv[1]);
    height=ToNum(argv[2]);
  }

  printf("Started to render a %d x %d image...\n",width, height);

  BasicTextureVector textures;

  textures.AddTexture("textures/Wooden_desks.tga");
  textures.AddTexture("textures/rock_textures/rock_02_hm.tga");

  int choo=1;

  if (choo==1){

    obj_vec.resize(10);

    obj_vec[0]=new SphereT();
    obj_vec[0]->SetReflect(0.0); 
    obj_vec[0]->SetRefract(0.0);
    obj_vec[0]->SetDiffuse(1.0);
    obj_vec[0]->SetRefractIndex(0.0);
    obj_vec[0]->SetColor(PixelColor(113,83,38));
    ((SphereT*)obj_vec[0])->SetCenter(PointT(0.0, -1010.0, 0.0));
    ((SphereT*)obj_vec[0])->SetRadius(1000.0);
    obj_vec[0]->SetTexture(textures.GetTexture(0));

    obj_vec[1]=new SphereT();
    obj_vec[1]->SetReflect(0.5); 
    obj_vec[1]->SetRefract(0.0);
    obj_vec[1]->SetDiffuse(1.0);
    obj_vec[1]->SetRefractIndex(0.0);
    obj_vec[1]->SetColor(PixelColor(0,0,230));
    ((SphereT*)obj_vec[1])->SetCenter(PointT(1040.0, 0.0, 0.0));
    ((SphereT*)obj_vec[1])->SetRadius(1000.0);

    obj_vec[2]=new SphereT();
    obj_vec[2]->SetReflect(0.0); 
    obj_vec[2]->SetRefract(0.0);
    obj_vec[2]->SetDiffuse(1.0);
    obj_vec[2]->SetRefractIndex(0.0);
    obj_vec[2]->SetColor(PixelColor(230,230,0));
    ((SphereT*)obj_vec[2])->SetCenter(PointT(-1025.0, 0.0, 0.0));
    ((SphereT*)obj_vec[2])->SetRadius(1000.0);

    obj_vec[3]=new SphereT();
    obj_vec[3]->SetReflect(0.7); 
    obj_vec[3]->SetRefract(0.0);
    obj_vec[3]->SetDiffuse(0.0);
    obj_vec[3]->SetRefractIndex(0.0);
    obj_vec[3]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[3])->SetCenter(PointT(0.0, 0.0, 1030.0));
    ((SphereT*)obj_vec[3])->SetRadius(1000.0);

    obj_vec[4]=new SphereT();
    obj_vec[4]->SetReflect(0.0); 
    obj_vec[4]->SetRefract(0.0);
    obj_vec[4]->SetDiffuse(1.5);
    obj_vec[4]->SetRefractIndex(1.7);
    obj_vec[4]->SetColor(PixelColor(40,255,40));
    ((SphereT*)obj_vec[4])->SetCenter(PointT(-15.0, 0.0, 20.0));
    ((SphereT*)obj_vec[4])->SetRadius(2.0);

    obj_vec[5]=new SphereT();
    obj_vec[5]->SetReflect(0.0); 
    obj_vec[5]->SetRefract(1.0);
    obj_vec[5]->SetDiffuse(1.5);
    obj_vec[5]->SetRefractIndex(1.7);
    obj_vec[5]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[5])->SetCenter(PointT(-10.0, 3.0, -10.0));
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
    ((SphereT*)obj_vec[7])->SetCenter(PointT(0.0, 5.0, -20.0));
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

    obj_vec[9]=new SphereT();
    obj_vec[9]->SetReflect(0.0); 
    obj_vec[9]->SetRefract(0.0);
    obj_vec[9]->SetDiffuse(0.0);
    obj_vec[9]->SetRefractIndex(1.7);
    obj_vec[9]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[9])->SetCenter(PointT(0.0, 50.0, 29.0));
    ((SphereT*)obj_vec[9])->SetRadius(0.5);
    obj_vec[9]->SetLight(0.6);

  }else{

  }

  using namespace ObjectDescripter;
  
  ObjectT sth;
  sth.LoadFromObj("object_file/fixed.perfect.dragon.100K.0.07.obj");


  double x1=1e30, x2=-1e30;
  double y1=1e30, y2=-1e30;
  double z1=1e30, z2=-1e30;

  if(1){
  obj_vec.erase(obj_vec.begin()+5, obj_vec.begin()+8);
  
  PointT O(-0.7, -0.44, -1.0);
  for(int i=0; i<sth.m_nVertices; i++)
    sth.m_pVertexList[i]=sth.m_pVertexList[i]-O;
  for(int j=0; j<sth.m_nTriangles; j++){
    obj_vec.push_back(new TriangleT());
    auto i=(TriangleT*)obj_vec.back();
    i->SetDiffuse(0.5);
    i->SetColor(PixelColor(0,255,255));
    ((TriangleT*)i)->SetP(0,sth.m_pVertexList[sth.m_pTriangleList[j][0]]*25);
    ((TriangleT*)i)->SetP(1,sth.m_pVertexList[sth.m_pTriangleList[j][1]]*25);
    ((TriangleT*)i)->SetP(2,sth.m_pVertexList[sth.m_pTriangleList[j][2]]*25);
    ((TriangleT*)i)->Init();
    x1=min(x1,i->MinX()); x2=max(x2,i->MaxX());
    y1=min(y1,i->MinY()); y2=max(y2,i->MaxY());
    z1=min(z1,i->MinZ()); z2=max(z2,i->MaxZ());
  }
  }

  double len=max(max(x2-x1,y2-y1),z2-z1);

  printf("x1=%lf, y1=%lf, z1=%lf, len=%lf\n",x1, y1, z1, len);

  PixelColor *bmp_data=new PixelColor[width*height+10];

  // //freopen("a.txt","w",stdout);
  // double x=-10.0, scale=1.0/width*height, y;
  // for(int i=0; i<width; i++){
  //   y=-10.0*scale;
  //   for(int j=0; j<height; j++){
  //     RayT ray(camera, head_direction * y + righthand_direction * x + eye_direction * 20);
  //     PointT color;
  //     bool debug=0;
  //     Tracer::RayTrace(ray, obj_vec, color, 0, 1.0, debug);
  //     Color[i][j]=color;
  //     bmp_data[i+j*width]=PointT::ToPixelColor(color);
  //     y+=20.0*scale/height;
  //   }
  //   x+=20.0/width;
  // }

  lights.clear();
  for(auto a: obj_vec)
    if (a->IsLight())
      lights.push_back(a);

  Viewer my_viewer;
  my_viewer.SetCameraPos(PointT(-10.0, 10.0, -50.0));
  my_viewer.SetEyeDirection(PointT(0.3, 0.0, 1.0));
  my_viewer.SetHeadDirection(PointT(0.0, 1.0, 0.0));
  my_viewer.SetGeometryWidth(17.0);
  my_viewer.SetDistance(15.0);
  my_viewer.Init();

  Tracer my_tracer;
  my_tracer.Init(obj_vec);

  Process render_process("Rendering whole", width*height, 1, 10);
  render_process.Start();
  for(int i=0; i<width; i++)
    for(int j=0; j<height; j++){
      render_process.Update(i*height+j);
      RayT ray=my_viewer.GetRay(width, height, i, j);
      PointT color;
      bool debug=0;
      my_tracer.RayTrace(ray, obj_vec, lights, color, 0, 1.0, debug);
      Color[i][j]=color;
      bmp_data[i+j*width]=PointT::ToPixelColor(color);
    }
  render_process.Stop();

  OutputBmp(width, height, bmp_data, "result.bmp"); 
  
  return 0;
}
