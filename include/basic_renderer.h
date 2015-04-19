#ifndef _BASIC_RENDERER_H
#define _BASIC_RENDERER_H

#include "basic_geometry.h"
#include "basic_texture.h"

const double kSpaceRefractIndex = 1.0;

class Renderer;

class RayT{
  
 public:
  RayT(){}

  RayT(PointT o, PointT r)
    :o_(o), r_(r){
    r_=r_.Unit();
  }

  PointT GetO()const{ return o_; }
  PointT GetR()const{ return r_; }

  int FindFirstHitInVec(const std::vector<Renderer*> &objs)const;

 private:
  PointT o_, r_;

};


class Renderer{

 public:
  Renderer(){}
  
 Renderer(double a, double b, double c, double d, PointT e, bool light, double f=0.0)
   :reflect_coefficient_(a), refract_coefficient_(b), diffuse_coefficient_(c), refract_index_(d), color_(e), light_(light), light_intensity_(f), has_texture_(0){
  }

 Renderer(double a, double b, double c, double d, PixelColor e, bool light, double f=0.0)
   :reflect_coefficient_(a), refract_coefficient_(b), diffuse_coefficient_(c), refract_index_(d), color_(e), light_(light), light_intensity_(f){
  }

  virtual double Intersect(const RayT &r, PointT &ip)const{return 0.0;}
  virtual PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const{return PointT();}

  virtual void InitTexture(){}

  CoordinateT GetReflect()const{ return reflect_coefficient_; }
  CoordinateT GetRefract()const{ return refract_coefficient_; }
  CoordinateT GetDiffuse()const{ return diffuse_coefficient_; }
  virtual CoordinateT GetRefractIndex(PointT p=PointT())const{return refract_index_; }
  virtual PointT GetColor(PointT surface_point=PointT())const{ return color_; }
  bool IsLight(){ return light_; }
  CoordinateT GetIntensity()const{ return light_intensity_; }
  bool HasTexture()const{ return has_texture_; }
  TGAFILE* GetTGAFile()const{ return tga_file_; }

  void SetReflect(double a){ reflect_coefficient_=a; }
  void SetRefract(double a){ refract_coefficient_=a; }
  void SetDiffuse(double a){ diffuse_coefficient_=a; }
  void SetRefractIndex(double a){ refract_index_=a; }
  void SetColor(PointT col){ color_=col; }
  void SetColor(PixelColor color){ color_=PointT(color); }
  void SetLight(CoordinateT a){ light_=1; light_intensity_=a; }
  void SetTexture(TGAFILE *tgafile){ 
    has_texture_=1;
    tga_file_=tgafile; 
    InitTexture();
  }
  
 private:
  double reflect_coefficient_, refract_coefficient_, diffuse_coefficient_, refract_index_;

  PointT color_;

  bool light_;

  CoordinateT light_intensity_;

  bool has_texture_;

  TGAFILE *tga_file_;

};

class PlaneT: public Renderer{
  
 public:
  PlaneT(){}
  
 PlaneT(Renderer a, PointT o, PointT n)
   :Renderer(a), o_(o), normal_(n.Unit()){
  }

  PointT GetO()const{ return o_; }

  PointT GetNormal()const{ return normal_; }

  void InitTexture();

  PointT GetColor(PointT surface_point)const;

  void SetO(const PointT &p){ o_=p; }
  
  void SetNormal(const PointT &p){ normal_=p; }

  double Intersect(const RayT &ray, PointT &ip)const;

  friend double Intersect(const PlaneT &plane, const RayT &ray, PointT &ip);

  PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const;

 private:
  PointT o_, normal_; //normal_ is unit vector

  PointT u_axis, v_axis;
};

class SphereT: public Renderer{
  
 public:
  SphereT(){}
  
 SphereT(Renderer a, PointT c, CoordinateT r)
   :Renderer(a), center_(c), radius_(r){
  }

  PointT GetCenter()const{ return center_; }

  CoordinateT GetRadius()const{ return radius_; }

  double Intersect(const RayT &ray, PointT &ip)const;

  friend double Intersect(const SphereT &sphere, const RayT &ray, PointT &ip);

  PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const;

  void SetCenter(const PointT &p){ center_=p; }

  void SetRadius(const CoordinateT &d){ radius_=d; }

  bool Inside(const PointT &point)const{
    return Sign( ( point - center_ ).Length2() - Sqr(radius_) ) < 0;
  }

  bool OnBroader(const PointT &point)const{
    return Sign( ( point - center_ ).Length2() - Sqr(radius_) ) == 0;
  }

  CoordinateT GetRefractIndex(PointT p=PointT())const{
    if (!Inside(p))
      return Renderer::GetRefractIndex();
    return kSpaceRefractIndex / Renderer::GetRefractIndex();
  }

  PointT GetColor(PointT surface_point)const;

 private:
  PointT center_;
  CoordinateT radius_;
  
};

class TriangleT: public Renderer{

 public:
  
  TriangleT(){}

 TriangleT(Renderer a, PointT _p0, PointT _p1, PointT _p2)
   :Renderer(a), p0_(_p0), p1_(_p1), p2_(_p2), initialized_(0){
    Init();
  }

  void Init(){
    initialized_=1;
    p1_=p1_-p0_;
    p2_=p2_-p0_;
    n_=Cross(p1_, p2_).Unit();
  }

  void SetP(int o, PointT p){
    if (o==0) p0_=p;
    if (o==1) p1_=p;
    if (o==2) p2_=p;
  }

  PointT Getp(int o)const{
    if (o==0) return p0_;
    if (o==1) return p1_;
    if (o==2) return p2_;
    throw std::runtime_error("GetP in TriangleT");
  }

  double Intersect(const RayT &ray, PointT &ip)const;
  
  friend double Intersect(const TriangleT &triangle, const RayT &ray, PointT &ip);

  PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const;

  PointT GetColor(PointT surface_point)const;

 private:
  
  PointT p0_, p1_, p2_, n_;
  bool initialized_;
  
};


#endif
