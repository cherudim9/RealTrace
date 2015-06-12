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

vector<Renderer*> obj_vec;

int ToNum(char *c){
  stringstream O;
  O<<string(c);
  int ret;
  O>>ret;
  return ret;
}

int main(int argc, char **argv){

  if (argc<3){
    string s="main <width> <height> <scene> <subpixel size> <sample times> <trace method, 1 for ordi 2 for mc>";
    cout<<s<<endl;
    return 0;
  }

  int width=100, height=100;

  int scene_number=1;

  int subpixel_size=1;

  int samples_number=20;

  int trace_method=1;

  if (argc>=3){
    width=ToNum(argv[1]);
    height=ToNum(argv[2]);
  }

  if (argc>=4)
    scene_number=ToNum(argv[3]);

  if (argc>=5)
    subpixel_size=ToNum(argv[4]);
  
  if (argc>=6)
    samples_number=ToNum(argv[5]);

  if (argc>=7)
    trace_method=ToNum(argv[6]);

  printf("Started to render a %d x %d image with subsize %d and %d samples...\n",width, height, subpixel_size, samples_number);

  BasicTextureVector my_textures;
  Viewer my_viewer;

  my_textures.AddTexture("textures/Wooden_desks.tga");
  my_textures.AddTexture("textures/rock_textures/rock_02_hm.tga");

  using namespace ObjectDescripter;

  if (scene_number==1){

    my_viewer.SetCameraPos(PointT(-10.0, 10.0, -50.0));
    my_viewer.SetEyeDirection(PointT(0.3, 0.0, 1.0));
    my_viewer.SetHeadDirection(PointT(0.0, 1.0, 0.0));
    my_viewer.SetGeometryWidth(17.0);
    my_viewer.SetDistance(13.0);
    my_viewer.Init();

    obj_vec.resize(12);

    obj_vec[0]=new SphereT();
    obj_vec[0]->SetReflect(0.0); 
    obj_vec[0]->SetRefract(0.0);
    obj_vec[0]->SetDiffuse(0.5);
    obj_vec[0]->SetRefractIndex(0.0);
    obj_vec[0]->SetColor(PixelColor(113,83,38));
    ((SphereT*)obj_vec[0])->SetCenter(PointT(0.0, -1010.0, 0.0));
    ((SphereT*)obj_vec[0])->SetRadius(1000.0);
    obj_vec[0]->SetTexture(my_textures.GetTexture(0));

    obj_vec[1]=new SphereT();
    obj_vec[1]->SetReflect(0.5);
    obj_vec[1]->SetRefract(0.0);
    obj_vec[1]->SetDiffuse(1.0);
    obj_vec[1]->SetRefractIndex(0.0);
    obj_vec[1]->SetColor(PixelColor(0,0,230));
    obj_vec[1]->SetColor(PixelColor(230,230,0));
    ((SphereT*)obj_vec[1])->SetCenter(PointT(1040.0, 0.0, 0.0));
    ((SphereT*)obj_vec[1])->SetRadius(1000.0);

    obj_vec[2]=new SphereT();
    obj_vec[2]->SetReflect(0.0); 
    obj_vec[2]->SetRefract(0.0);
    obj_vec[2]->SetDiffuse(1.0);
    obj_vec[2]->SetRefractIndex(0.0);
    obj_vec[2]->SetColor(PixelColor(230,230,0));
    ((SphereT*)obj_vec[2])->SetCenter(PointT(-1040.0, 0.0, 0.0));
    ((SphereT*)obj_vec[2])->SetRadius(1000.0);

    obj_vec[3]=new SphereT();
    obj_vec[3]->SetReflect(0.7);
    obj_vec[3]->SetRefract(0.0);
    obj_vec[3]->SetDiffuse(0.0);
    obj_vec[3]->SetRefractIndex(0.0);
    obj_vec[3]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[3])->SetCenter(PointT(0.0, 0.0, 1100.0));
    ((SphereT*)obj_vec[3])->SetRadius(1000.0);

    obj_vec[4]=new SphereT();
    obj_vec[4]->SetReflect(0.0); 
    obj_vec[4]->SetRefract(1.0);
    obj_vec[4]->SetDiffuse(0.1);
    obj_vec[4]->SetRefractIndex(1.7);
    obj_vec[4]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[4])->SetCenter(PointT(2.0, 5.0, -20.0));
    ((SphereT*)obj_vec[4])->SetRadius(7.0);

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
    obj_vec[7]->SetReflect(1.0); 
    obj_vec[7]->SetRefract(0.0);
    obj_vec[7]->SetDiffuse(0.0);
    obj_vec[7]->SetColor(PixelColor(255,255,0));
    ((SphereT*)obj_vec[7])->SetCenter(PointT(-15.0, 15.0, 10.0));
    ((SphereT*)obj_vec[7])->SetRadius(7.0);

    obj_vec[8]=new SphereT();
    obj_vec[8]->SetReflect(1.0); 
    obj_vec[8]->SetRefract(0.0);
    obj_vec[8]->SetDiffuse(0.0);
    obj_vec[8]->SetColor(PixelColor(0,255,255));
    ((SphereT*)obj_vec[8])->SetCenter(PointT(-10.0, 2.0, 0.0));
    ((SphereT*)obj_vec[8])->SetRadius(7.0);

    obj_vec[9]=new SphereT();
    obj_vec[9]->SetReflect(0.0); 
    obj_vec[9]->SetRefract(0.0);
    obj_vec[9]->SetDiffuse(0.0);
    obj_vec[9]->SetRefractIndex(1.7);
    obj_vec[9]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[9])->SetCenter(PointT(0.0, 1000.0, 29.0));
    ((SphereT*)obj_vec[9])->SetRadius(900);
    obj_vec[9]->SetLight(0.6);
    obj_vec[9]->SetEmission(1.0, 1.0, 1.0);

    obj_vec[10]=new SphereT();
    obj_vec[10]->SetReflect(0.8); 
    obj_vec[10]->SetRefract(0.0);
    obj_vec[10]->SetDiffuse(0.0);
    obj_vec[10]->SetRefractIndex(0.0);
    obj_vec[10]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[10])->SetCenter(PointT(0.0, 0.0, -1100.0));
    ((SphereT*)obj_vec[10])->SetRadius(1000.0);

    obj_vec[11]=new SphereT();
    obj_vec[11]->SetRefract(0.8);
    obj_vec[11]->SetRefractIndex(1.7);
    obj_vec[11]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[11])->SetCenter(PointT(-2.0, 7.0, -35.0));
    ((SphereT*)obj_vec[11])->SetRadius(3);

    obj_vec.erase(obj_vec.begin()+4, obj_vec.begin()+7);  

    if (1){
      ObjectT sth;
      // sth.LoadFromObj("object_file/Buddha.obj");
      sth.LoadFromObj("object_file/fixed.perfect.dragon.100K.0.07.obj");
      sth.Init();

      double x1=1e30, x2=-1e30;
      double y1=1e30, y2=-1e30;
      double z1=1e30, z2=-1e30;

      PointT O(-0.7, -0.44, -1.0);
      for(int i=0; i<sth.m_nVertices; i++)
        sth.m_pVertexList[i]=sth.m_pVertexList[i]-O;
      for(int j=0; j<sth.m_nTriangles; j++){
        obj_vec.push_back(new TriangleT());
        auto i=(TriangleT*)obj_vec.back();
        i->SetDiffuse(0.2);    
        i->SetColor(PixelColor(255,255,255));
        ((TriangleT*)i)->SetP(0,sth.m_pVertexList[sth.m_pTriangleList[j][0]]*25);
        ((TriangleT*)i)->SetP(1,sth.m_pVertexList[sth.m_pTriangleList[j][1]]*25);
        ((TriangleT*)i)->SetP(2,sth.m_pVertexList[sth.m_pTriangleList[j][2]]*25);
        ((TriangleT*)i)->SetN(0,sth.vertex_normal_[sth.m_pTriangleList[j][0]]);
        ((TriangleT*)i)->SetN(1,sth.vertex_normal_[sth.m_pTriangleList[j][1]]);
        ((TriangleT*)i)->SetN(2,sth.vertex_normal_[sth.m_pTriangleList[j][2]]);
        ((TriangleT*)i)->Init();
        x1=min(x1,i->MinX()); x2=max(x2,i->MaxX());
        y1=min(y1,i->MinY()); y2=max(y2,i->MaxY());
        z1=min(z1,i->MinZ()); z2=max(z2,i->MaxZ());
      }

      double len=max(max(x2-x1,y2-y1),z2-z1);

      printf("x1=%lf, y1=%lf, z1=%lf, len=%lf\n",x1, y1, z1, len);

    }

  }
  
  if (scene_number==2){

    my_viewer.SetCameraPos(PointT(0.0, 10.0, -70.0));
    my_viewer.SetEyeDirection(PointT(0.0,0.0,1.0));
    my_viewer.SetHeadDirection(PointT(0.0,1.0,0.0));
    my_viewer.SetGeometryWidth(10.0);
    my_viewer.SetDistance(10.0);
    my_viewer.Init();

    obj_vec.resize(6);

    obj_vec[0]=new SphereT();
    obj_vec[0]->SetReflect(0.0); 
    obj_vec[0]->SetRefract(0.0);
    obj_vec[0]->SetDiffuse(1.0);
    obj_vec[0]->SetRefractIndex(0.0);
    obj_vec[0]->SetColor(PixelColor(113,83,38));
    ((SphereT*)obj_vec[0])->SetCenter(PointT(0.0, -1010.0, 0.0));
    ((SphereT*)obj_vec[0])->SetRadius(1000.0);
    obj_vec[0]->SetTexture(my_textures.GetTexture(0));

    obj_vec[1]=new SphereT();
    obj_vec[1]->SetReflect(0.0); 
    obj_vec[1]->SetRefract(0.0);
    obj_vec[1]->SetDiffuse(0.0);
    obj_vec[1]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[1])->SetCenter(PointT(0.0, 1300.0, 0.0));
    ((SphereT*)obj_vec[1])->SetRadius(1000);
    obj_vec[1]->SetLight(0.8);
    obj_vec[1]->SetEmission(1.0, 1.0, 1.0);

    obj_vec[2]=new SphereT();
    obj_vec[2]->SetReflect(0.0); 
    obj_vec[2]->SetRefract(1.0);
    obj_vec[2]->SetDiffuse(0.0);
    obj_vec[2]->SetRefractIndex(1.7);
    obj_vec[2]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[2])->SetCenter(PointT(0.0, 10.0, 0.0));
    ((SphereT*)obj_vec[2])->SetRadius(10.0);

    obj_vec[3]=new SphereT();
    obj_vec[3]->SetReflect(1.0); 
    obj_vec[3]->SetRefract(0.0);
    obj_vec[3]->SetDiffuse(0.0);
    obj_vec[3]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[3])->SetCenter(PointT(20.0, 10.0, 5.0));
    ((SphereT*)obj_vec[3])->SetRadius(10.0);

    obj_vec[4]=new SphereT();
    obj_vec[4]->SetReflect(0.0); 
    obj_vec[4]->SetRefract(0.0);
    obj_vec[4]->SetDiffuse(1.0);
    obj_vec[4]->SetColor(PixelColor(255,255,0));
    ((SphereT*)obj_vec[4])->SetCenter(PointT(2.0, 10.0, 20.0));
    ((SphereT*)obj_vec[4])->SetRadius(2.0);

    obj_vec[5]=new SphereT();
    obj_vec[5]->SetReflect(0.0); 
    obj_vec[5]->SetRefract(0.0);
    obj_vec[5]->SetDiffuse(1.0);
    obj_vec[5]->SetColor(PixelColor(0,255,255));
    ((SphereT*)obj_vec[5])->SetCenter(PointT(-2.0, 10.0, 25.0));
    ((SphereT*)obj_vec[5])->SetRadius(2.0);

  }

  if (scene_number==3){

    my_viewer.SetCameraPos(PointT(50.0, 28.0, 0));
    my_viewer.SetEyeDirection(PointT(0.0,0.0,1.0));
    my_viewer.SetHeadDirection(PointT(0.0,1.0,0.0));
    my_viewer.SetGeometryWidth(100.0);
    my_viewer.SetDistance(10.0);
    my_viewer.Init();

    double R=120;     // radius
    double T=30*M_PI/180.;
    double D=R/cos(T);     //distance
    double Z=62;
    PointT C=PointT(0.275, 0.612, 0.949);

    obj_vec.resize(5);

    obj_vec[0]=new SphereT();
    obj_vec[0]->SetEmission(C*6e-2);
    obj_vec[0]->SetReflect(0.5); 
    obj_vec[0]->SetRefract(0.0);
    obj_vec[0]->SetDiffuse(0.0);
    obj_vec[0]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[0])->SetCenter(PointT(50,28,Z)+PointT(cos(T),sin(T),0)*D);
    ((SphereT*)obj_vec[0])->SetRadius(R);

    obj_vec[1]=new SphereT();
    obj_vec[1]->SetEmission(C*6e-2);
    obj_vec[1]->SetReflect(0.5); 
    obj_vec[1]->SetRefract(0.0);
    obj_vec[1]->SetDiffuse(0.0);
    obj_vec[1]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[1])->SetCenter(PointT(50,28,Z)+PointT(-cos(T),sin(T),0)*D);
    ((SphereT*)obj_vec[1])->SetRadius(R);

    obj_vec[2]=new SphereT();
    obj_vec[2]->SetEmission(C*6e-2);
    obj_vec[2]->SetReflect(0.5); 
    obj_vec[2]->SetRefract(0.0);
    obj_vec[2]->SetDiffuse(0.0);
    obj_vec[2]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[2])->SetCenter(PointT(50,28,Z)+PointT(0,-1,0)*D);
    ((SphereT*)obj_vec[2])->SetRadius(R);

    obj_vec[3]=new SphereT();
    obj_vec[3]->SetEmission(C*6e-2);
    obj_vec[3]->SetReflect(0.5); 
    obj_vec[3]->SetRefract(0.0);
    obj_vec[3]->SetDiffuse(0.0);
    obj_vec[3]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[3])->SetCenter(PointT(50,28,Z)+PointT(0,0,-1)*R*2*sqrt(2./3.));
    ((SphereT*)obj_vec[3])->SetRadius(R);

    obj_vec[4]=new SphereT(); 
    obj_vec[4]->SetEmission(C*6e-2);
    obj_vec[4]->SetReflect(0.5); 
    obj_vec[4]->SetRefract(0.0);
    obj_vec[4]->SetDiffuse(0.0);
    obj_vec[4]->SetColor(PixelColor(255/2,255/2,255/2));
    ((SphereT*)obj_vec[4])->SetCenter(PointT(50,28,Z)+PointT(0,0,-R*2*sqrt(2./3.)/3.));
    ((SphereT*)obj_vec[4])->SetRadius(2*2*R*2*sqrt(2./3.)-R*2*sqrt(2./3.)/3.);

  //   Sphere spheres[] = {//Scene: radius, position, emission, color, material

//       Sphere(R, Vec(50,28,Z)+Vec( cos(T),sin(T),0)*D,    C*6e-2,Vec(1,1,1)*.996, SPEC), //red
//       Sphere(R, Vec(50,28,Z)+Vec(-cos(T),sin(T),0)*D,    C*6e-2,Vec(1,1,1)*.996, SPEC), //grn
//       Sphere(R, Vec(50,28,Z)+Vec(0,-1,0)*D,              C*6e-2,Vec(1,1,1)*.996, SPEC), //blue
//       Sphere(R, Vec(50,28,Z)+Vec(0,0,-1)*R*2*sqrt(2./3.),C*0e-2,Vec(1,1,1)*.996, SPEC), //back
//       Sphere(2*2*R*2*sqrt(2./3.)-R*2*sqrt(2./3.)/3., Vec(50,28,Z)+Vec(0,0,-R*2*sqrt(2./3.)/3.),   Vec(1,1,1)*0,Vec(1,1,1)*.5, SPEC), //front
// };

  }

  if (scene_number==4){

    my_viewer.SetCameraPos(PointT(5.0, 10.0, -70.0));
    my_viewer.SetEyeDirection(PointT(0.0,0.0,1.0));
    my_viewer.SetHeadDirection(PointT(0.0,1.0,0.0));
    my_viewer.SetGeometryWidth(10.0);
    my_viewer.SetDistance(26.0);
    my_viewer.Init();

    obj_vec.resize(1);

    obj_vec[0]=new SphereT();
    obj_vec[0]->SetReflect(0.0); 
    obj_vec[0]->SetRefract(0.0);
    obj_vec[0]->SetDiffuse(0.0);
    obj_vec[0]->SetRefractIndex(0.0);
    obj_vec[0]->SetColor(PixelColor(255,255,255));
    ((SphereT*)obj_vec[0])->SetCenter(PointT(0.0, 100.0, -100.0));
    ((SphereT*)obj_vec[0])->SetRadius(1.0);
    obj_vec[0]->SetLight(1.0);

    ObjectT sth;
    // sth.LoadFromObj("object_file/Buddha.obj");
    sth.LoadFromObj("object_file/Buddha.obj");
    sth.Init();

    double x1=1e30, x2=-1e30;
    double y1=1e30, y2=-1e30;
    double z1=1e30, z2=-1e30;
  
    PointT O(-0.4,-1.0,-0.4);
    for(int i=0; i<sth.m_nVertices; i++)
      sth.m_pVertexList[i]=sth.m_pVertexList[i]-O;
    for(int j=0; j<sth.m_nTriangles; j++){
      obj_vec.push_back(new TriangleT());
      auto i=(TriangleT*)obj_vec.back();
      i->SetDiffuse(0.8);    
      i->SetColor(PixelColor(255,255,255));
      ((TriangleT*)i)->SetP(0,sth.m_pVertexList[sth.m_pTriangleList[j][0]]*10);
      ((TriangleT*)i)->SetP(1,sth.m_pVertexList[sth.m_pTriangleList[j][1]]*10);
      ((TriangleT*)i)->SetP(2,sth.m_pVertexList[sth.m_pTriangleList[j][2]]*10);
      ((TriangleT*)i)->SetN(0,sth.vertex_normal_[sth.m_pTriangleList[j][0]]);
      ((TriangleT*)i)->SetN(1,sth.vertex_normal_[sth.m_pTriangleList[j][1]]);
      ((TriangleT*)i)->SetN(2,sth.vertex_normal_[sth.m_pTriangleList[j][2]]);
      ((TriangleT*)i)->Init();
      x1=min(x1,i->MinX()); x2=max(x2,i->MaxX());
      y1=min(y1,i->MinY()); y2=max(y2,i->MaxY());
      z1=min(z1,i->MinZ()); z2=max(z2,i->MaxZ());
    }

    double len=max(max(x2-x1,y2-y1),z2-z1);

    printf("x1=%lf, y1=%lf, z1=%lf, len=%lf\n",x1, y1, z1, len);
  }

  PixelColor *bmp_data=new PixelColor[width*height+10];

  Tracer my_tracer;
  my_tracer.Init(obj_vec);

  Process render_process("Rendering whole", width*height, 1, width*height/1000);
  render_process.Start();
  // #pragma omp parallel for schedule(dynamic,1) 
  for(int i=0, im; i<width; i++)
    for(int j=0, jm, last_hit_seq=-1; j<height; j++){
      render_process.Update(i*height+j);
      bool debug=0;
      PointT color;
      im=jm=subpixel_size;
      RayT ray=my_viewer.GetRay(width, height, i, j);
      PointT tmp;
      int now_hit_seq;
      if (trace_method==1)
        now_hit_seq=my_tracer.RayTrace(ray, obj_vec, tmp, 0, 1.0, debug);
      else
        now_hit_seq=my_tracer.MonteCarloRayTrace(ray, obj_vec, tmp, 0, 1.0, debug);
      if (now_hit_seq == last_hit_seq){
        im=jm=1;
      }
      last_hit_seq=now_hit_seq;
      for(int ii=0; ii<im; ii++)
        for(int jj=0; jj<jm; jj++){
          for(int _=0; _<=samples_number; _++){
            ray=my_viewer.GetRay(width, height, i+ii*1./im+RandUnit()*1./im, j+jj*1./jm+RandUnit()*1./jm);
            if (trace_method==1)
              my_tracer.RayTrace(ray, obj_vec, tmp, 0, 1.0, debug);
            else
              my_tracer.MonteCarloRayTrace(ray, obj_vec, tmp, 0, 1.0, debug);
            color+=tmp/im/jm/samples_number;
          }
        }
      Color[i][j]=color;
      bmp_data[i+j*width]=PointT::ToPixelColor(color.GammaCorrect());
    }
  render_process.Stop();

  cout<<"total first hit test: "<<my_tracer.enter_times<<endl;

  cout<<"average intersection test: "<<1.0*my_tracer.q_len_tot/my_tracer.enter_times<<endl;

  OutputBmp(width, height, bmp_data, "result.bmp"); 
  
  return 0;
}
