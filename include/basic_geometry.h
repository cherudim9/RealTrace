#ifndef BASIC_GEOMETRY_H_
#define BASIC_GEOMETRY_H_

#include <cmath>
#include "basic_types.h"
#include <vector>

typedef double CoordinateT;

const double kSpaceRefractIndex = 1.0;

template<class T>
T Sqr(const T &a){
  return a*a;
}

class PointT{

 public:
  PointT(){}

  PointT(CoordinateT x, CoordinateT y, CoordinateT z)
    :x_(x), y_(y), z_(z){
  }

  PointT(PixelColor color)
    :x_(color.GetR()/256.0), y_(color.GetG()/256.0), z_(color.GetB()/256.0){
  }

  static PixelColor ToPixelColor(const PointT &p);

  CoordinateT GetX()const{ return x_; }
  CoordinateT GetY()const{ return y_; }
  CoordinateT GetZ()const{ return z_; }
  
  bool operator==(const PointT &b){ return Sign(x_-b.x_)==0 && Sign(y_-b.y_)==0 && Sign(z_-b.z_)==0; }
  bool operator!=(const PointT &b){ return ! ( (*this) == b ); }
  CoordinateT Length2()const{ return Sqr(x_)+Sqr(y_)+Sqr(z_); }
  CoordinateT Length()const{ return sqrt(Length2()); }
  PointT Unit()const{ CoordinateT d=Length(); return PointT(x_/d, y_/d, z_/d); }
  PointT operator+(const PointT &b)const{ return PointT(x_+b.x_, y_+b.y_, z_+b.z_); }
  PointT operator-(const PointT &b)const{ return PointT(x_-b.x_, y_-b.y_, z_-b.z_); }
  PointT operator-()const{ return PointT(-x_, -y_, -z_); }
  PointT operator*(const CoordinateT &k)const{ return PointT(k*x_, k*y_, k*z_); }
  PointT& operator+=(const PointT &b){ x_+=b.x_, y_+=b.y_, z_+=b.z_; return *this; }
  friend PointT operator*(const CoordinateT &k, const PointT &a){ return PointT(k*a.x_, k*a.y_, k*a.z_); }
  PointT operator*(const PointT &b)const{ return PointT(x_*b.x_, y_*b.y_, z_*b.z_); }
  friend PointT Cross(const PointT &a, const PointT &b);
  friend CoordinateT Dot(const PointT &a, const PointT &b);

 private:
  CoordinateT x_, y_, z_;

};

CoordinateT Dot(const PointT &a, const PointT &b);

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
   :reflect_coefficient_(a), refract_coefficient_(b), diffuse_coefficient_(c), refract_index_(d), color_(e), light_(light), light_intensity_(f){
  }

 Renderer(double a, double b, double c, double d, PixelColor e, bool light, double f=0.0)
   :reflect_coefficient_(a), refract_coefficient_(b), diffuse_coefficient_(c), refract_index_(d), color_(e), light_(light), light_intensity_(f){
  }

  virtual double Intersect(const RayT &r, PointT &ip)const{return 0.0;}
  virtual PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const{return PointT();}

  CoordinateT GetReflect()const{ return reflect_coefficient_; }
  CoordinateT GetRefract()const{ return refract_coefficient_; }
  CoordinateT GetDiffuse()const{ return diffuse_coefficient_; }
  virtual CoordinateT GetRefractIndex(PointT p=PointT())const{return refract_index_; }
  PointT GetColor()const{ return color_; }
  bool IsLight(){ return light_; }
  CoordinateT GetIntensity()const{ return light_intensity_; }
  
 private:
  double reflect_coefficient_, refract_coefficient_, diffuse_coefficient_, refract_index_;

  PointT color_;

  bool light_;

  CoordinateT light_intensity_;

};

class PlaneT: public Renderer{
  
 public:
  PlaneT(){}
  
 PlaneT(Renderer a, PointT o, PointT n)
   :Renderer(a), o_(o), normal_(n.Unit()){
  }

  PointT GetO()const{ return o_; }
  PointT GetNormal()const{ return normal_; }

  double Intersect(const RayT &ray, PointT &ip)const;
  friend double Intersect(const PlaneT &plane, const RayT &ray, PointT &ip);

  PointT GetSurfaceNormal(const PointT &surface_point, const PointT &from)const;

 private:
  PointT o_, normal_; //normal_ is unit vector

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

 private:
  PointT center_;
  CoordinateT radius_;
  
};

#endif
