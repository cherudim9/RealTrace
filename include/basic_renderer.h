#ifndef _BASIC_RENDERER_H
#define _BASIC_RENDERER_H

#include "basic_geometry.h"
#include "basic_texture.h"
#include <algorithm>

const double kSpaceRefractIndex = 1.0;

class RayT{
  
 public:
  RayT(){}

  RayT(PointT o, PointT r)
    :o_(o), r_(r){
    r_=r_.Unit();
  }

  PointT GetO()const{ return o_; }
  PointT GetR()const{ return r_; }

 private:
  PointT o_, r_;

};


class Renderer{

 public:
 Renderer()
   :reflect_coefficient_(0.0), refract_coefficient_(0.0), diffuse_coefficient_(0.0), refract_index_(0.0), color_(), light_(0), light_intensity_(0.0), has_texture_(0){}
  
 Renderer(double a, double b, double c, double d, PointT e, bool light, double f=0.0)
   :reflect_coefficient_(a), refract_coefficient_(b), diffuse_coefficient_(c), refract_index_(d), color_(e), light_(light), light_intensity_(f), has_texture_(0){
  }

 Renderer(double a, double b, double c, double d, PixelColor e, bool light, double f=0.0)
   :reflect_coefficient_(a), refract_coefficient_(b), diffuse_coefficient_(c), refract_index_(d), color_(e), light_(light), light_intensity_(f){
  }

  virtual double Intersect(const RayT &r, PointT &ip)const{return 0.0;}

  virtual PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const{return PointT();}
  
  virtual void InitTexture(){}

  virtual void DistanceToSlab(const PointT &d, CoordinateT &d1, CoordinateT &d2){ }

  virtual CoordinateT GetBVHIndex(int depth)const{
    return 0.0;
  }

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

  void DistanceToSlab(const PointT &d, CoordinateT &d1, CoordinateT &d2){ 
    double tmp=Dot(d, center_);
    d1=std::min(tmp-radius_, tmp+radius_);
    d2=std::max(tmp-radius_, tmp+radius_);
  }

  CoordinateT GetBVHIndex(int depth)const{
    if (depth%3==0)
      return center_.GetX();
    if (depth%3==1)
      return center_.GetY();
    //depth%3==2
    return center_.GetZ();
  }
  
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

  double MinX()const{
    return std::min(std::min(p0_.GetX(), (p0_+p1_).GetX()), (p0_+p2_).GetX());
  }

  double MinY()const{
    return std::min(std::min(p0_.GetY(), (p0_+p1_).GetY()), (p0_+p2_).GetY());
  }

  double MinZ()const{
    return std::min(std::min(p0_.GetZ(), (p0_+p1_).GetZ()), (p0_+p2_).GetZ());
  }

  double MaxX()const{
    return std::max(std::max(p0_.GetX(), (p0_+p1_).GetX()), (p0_+p2_).GetX());
  }

  double MaxY()const{
    return std::max(std::max(p0_.GetY(), (p0_+p1_).GetY()), (p0_+p2_).GetY());
  }

  double MaxZ()const{
    return std::max(std::max(p0_.GetZ(), (p0_+p1_).GetZ()), (p0_+p2_).GetZ());
  }

  double Intersect(const RayT &ray, PointT &ip)const;
  
  friend double Intersect(const TriangleT &triangle, const RayT &ray, PointT &ip);

  PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const;

  PointT GetColor(PointT surface_point)const;

  void DistanceToSlab(const PointT &d, CoordinateT &d1, CoordinateT &d2){
    CoordinateT tmp1=Dot(d,p0_), tmp2=Dot(d,p0_+p1_), tmp3=Dot(d,p0_+p2_);
    d1=std::min(std::min(tmp1,tmp2),tmp3);
    d2=std::max(std::max(tmp1,tmp2),tmp3);
  }

  CoordinateT GetBVHIndex(int depth)const{
    if (depth%3==0)
      return p0_.GetX();
    if (depth%3==1)
      return p0_.GetY();
    //depth%3==2
    return p0_.GetZ();
  }

 private:
  
  PointT p0_, p1_, p2_, n_;
  bool initialized_;
  
};


#endif
