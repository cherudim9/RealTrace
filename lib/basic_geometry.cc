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


double Intersect(const SphereT &sphere, const RayT &ray, PointT &ip){
  return sphere.Intersect(ray, ip);
}  

double Intersect(const PlaneT &pl, const RayT &ray, PointT &ip){
  return pl.Intersect(ray, ip);
}

double PlaneT::Intersect(const RayT &ray, PointT &ip)const{
  PointT rr=ray.GetR(), ro=ray.GetO();
  if ( Sign(Dot(rr, normal_)) == 0)
    return -1.0;
  //return t that, ro+t*rr-o_ dot normal_ == 0
  ro=ro-o_;
  //to solve tA+B=0
  double A = rr.GetX() * normal_.GetX() + rr.GetY() * normal_.GetY() + rr.GetZ() * normal_.GetZ();
  double B = ro.GetX() * normal_.GetX() + ro.GetY() * normal_.GetY() + ro.GetZ() * normal_.GetZ();
  if ( Sign(A) == 0 )
    return -1.0; //ignore when A==B==0
  double ret=-B/A;
  ip = ro + o_ + ret * rr;
  return ret;
}

double SphereT::Intersect(const RayT &ray, PointT &ip)const{
  PointT rr=ray.GetR(), ro=ray.GetO();
  ro=ro-center_;
  //solve At^2+bt+c=0
  double A = Sqr(rr.GetX()) + Sqr(rr.GetY()) + Sqr(rr.GetZ());
  double B = 2 * ( rr.GetX() * ro.GetX() + rr.GetY() * ro.GetY() + rr.GetZ() * ro.GetZ() );
  double C = Sqr(ro.GetX()) + Sqr(ro.GetY()) + Sqr(ro.GetZ()) - Sqr(radius_);
  //A != 0
  double delta = Sqr(B) - 4 * A * C;
  if (Sign(delta)<0)
    return -1.0;
  delta=sqrt(delta);
  double t1 = ( - B - delta ) / 2.0 / A, t2 = ( -B + delta ) / 2.0 / A;
  double t=-1.0;
  if (Sign(t1)>0)
    t=t1;
  if (Sign(t2)>0 && ( t2<t || Sign(t+1)==0 ))
    t=t2;
  ip=ro+center_+t*rr;
  return t;
}

PointT PlaneT::GetSurfaceNormal(const PointT &surface_point)const{
  return normal_;
}

PointT SphereT::GetSurfaceNormal(const PointT &surface_point)const{
  return (surface_point - center_).Unit();
}

int RayT::FindFirstHitInVec(const vector<Renderer*> &objs)const{
  double hit_dis=1e30;
  int hit_ret=-1;
  int i=0;
  for(int i=0; i!=objs.size(); i++){
    PointT ip;
    double x=objs[i]->Intersect(*this, ip);
    if (Sign(x)>=0)
      if (x<hit_dis){
        hit_dis=x;
        hit_ret=i;
      }
  }
  return hit_ret;
}
