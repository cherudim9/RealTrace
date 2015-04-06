#include "../include/basic_types.h"
#include "../include/basic_geometry.h"
#include <iostream>

using namespace std;

PixelColor PointT::ToPixelColor(const PointT &p){
  double x=p.GetX(), y=p.GetY(), z=p.GetZ();
  x=min(x, 1.0), x=max(x, 0.0);  
  y=min(y, 1.0), y=max(y, 0.0);  
  z=min(z, 1.0), z=max(z, 0.0);  
  return PixelColor( static_cast<BYTE>((int)(x*255)), static_cast<BYTE>((int)(y*255)), static_cast<BYTE>((int)(z*255)) );
}

PointT Cross(const PointT &a, const PointT &b){
  return PointT( a.y_ * b.z_ - a.z_ * b.y_, a.z_ * b.x_ - a.x_ * b.z_, a.x_ * b.y_ - a.y_ * b.x_ );
}

CoordinateT Dot(const PointT &a, const PointT &b){
  return a.x_*b.x_ + a.y_*b.y_ + a.z_*b.z_;
}


