#ifndef BASIC_GEOMETRY_H_
#define BASIC_GEOMETRY_H_

#include <cmath>
#include "basic_types.h"
#include <vector>
#include <iostream>

typedef double CoordinateT;

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
  void Normalize(){ *this=Unit(); }
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

#endif
