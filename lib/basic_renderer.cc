#include "../include/basic_renderer.h"

using namespace std;

double Intersect(SphereT &sphere, const RayT &ray, PointT &ip){
  return sphere.Intersect(ray, ip);
}  

double Intersect(PlaneT &pl, const RayT &ray, PointT &ip){
  return pl.Intersect(ray, ip);
}

double PlaneT::Intersect(const RayT &ray, PointT &ip){
  PointT rr=ray.GetR(), ro=ray.GetO();
  if ( Sign(Dot(rr, normal_)) == 0)
    return -1.0;
  //return t that, ro+t*rr-o_ dot normal_ == 0
  ro=ro-o_;
  //to solve tA+B=0
  double A = rr.GetX() * normal_.GetX() + rr.GetY() * normal_.GetY() + rr.GetZ() * normal_.GetZ();
  double B = ro.GetX() * normal_.GetX() + ro.GetY() * normal_.GetY() + ro.GetZ() * normal_.GetZ();
  if ( Sign(A) == 0 || Sign(B) == 0 )
    return -1.0; 
  double ret=-B/A;
  ip = ro + o_ + ret * rr;
  return ret;
}

double SphereT::Intersect(const RayT &ray, PointT &ip){
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

PointT PlaneT::GetSurfaceNormal(const PointT &surface_point, const PointT &from)const{
  double dot = Dot( from - surface_point , normal_ );
  if (Sign(dot)<0)
    return -normal_;
  return normal_;
}

PointT SphereT::GetSurfaceNormal(const PointT &surface_point, const PointT &from)const{
  PointT normal_ = (surface_point - center_).Unit();
  double dot = Dot( from - surface_point, normal_ );
  if (Sign(dot)<0)
    return -normal_;
  return normal_;
}

void PlaneT::InitTexture(){
  PointT tmp(1.0, 0.0, 0.0);
  if (tmp == normal_)
    tmp=PointT(0.0, 1.0, 0.0);
  u_axis = Cross( normal_, tmp);
  v_axis = Cross( normal_, u_axis);
}

PointT PlaneT::GetColor(PointT surface_point)const{
  if (!Renderer::HasTexture()){
    return Renderer::GetColor();
  }
  int w=GetTGAFile()->imageWidth, h=GetTGAFile()->imageHeight;
  CoordinateT u = Dot(surface_point, u_axis), v=Dot(surface_point, v_axis);
  u*=w/25.0;
  v*=w/25.0;
  long long x=(long long)(u), y=(long long)(v);
  x=((x%w)+w)%w;
  y=((y%h)+h)%h;
  long long x1=(x+1)%w, y1=(y+1)%h;
  CoordinateT du=u-(long long)(u), dv=v-(long long)(v);
  /*
        u
      A | B
    v - . -
      C | D
   */
  CoordinateT A=du*dv, B=(1-du)*dv, C=du*(1-dv), D=(1-du)*(1-dv);
  return GetTGAFile()->GetColor(x,y);
  return A * GetTGAFile()->GetColor(x,y)
    + B * GetTGAFile()->GetColor(x1,y)
    + C * GetTGAFile()->GetColor(x,y1)
    + D * GetTGAFile()->GetColor(x1,y1);
}

PointT SphereT::GetColor(PointT surface_point)const{
  if (!Renderer::HasTexture())
    return Renderer::GetColor();
  int w=GetTGAFile()->imageWidth, h=GetTGAFile()->imageHeight;
  PointT d=(surface_point-GetCenter()).Unit();
  double u = 0.5 + atan(2.0 * d.GetX() / d.GetZ()) / 2.0 / kPI;
  double v = 0.5 - asin(d.GetY()) / kPI;
  int x,y;
  x=int(u*w);
  y=int(v*h);
  x=((x%w)+w)%w;
  y=((y%h)+h)%h;
  return GetTGAFile()->GetColor(x,y);
}

double TriangleT::Intersect(const RayT &ray, PointT &ip){
  //http://en.wikipedia.org/wiki/Möller–Trumbore_intersection_algorithm
  if (!initialized_)
    throw std::runtime_error("triangle not initialized");
  PointT tmp1=Cross(ray.GetR(), p2_);
  double det=Dot(p1_, tmp1);
  if (Sign(det)==0)
    return -1.0;
  double inv_det=1.0/det;
  PointT tmp2=ray.GetO()-p0_;
  u_=Dot(tmp2, tmp1)*inv_det;
  if (u_<0.0 || u_>1.0)
    return -1.0;
  PointT tmp3=Cross(tmp2, p1_);
  v_=Dot(ray.GetR(), tmp3)*inv_det;
  if (v_<0 || u_+v_>1.0)
    return -1.0;
  double ret=Dot(p2_, tmp3)*inv_det;
  if (Sign(ret)<=0)
    return -1.0;
  ip=ray.GetO()+ray.GetR()*ret;
  return ret;
}

double Intersect(TriangleT &triangle, const RayT &ray, PointT &ip){
  return triangle.Intersect(ray, ip);
}

PointT TriangleT::GetSurfaceNormal(const PointT &surface_point, const PointT &from)const{
  if (!initialized_)
    throw std::runtime_error("triangle not initialized");
  PointT ret = (n0_ + n1_ * u_ + n2_ * v_).Unit();
  if (Sign(Dot(ret, from-surface_point))<0)
    return -ret;
  return ret;
}

PointT TriangleT::GetColor(PointT surface_point)const{
  if (!initialized_)
    throw std::runtime_error("triangle not initialized");
  if (!Renderer::HasTexture())
    return Renderer::GetColor();
  throw std::runtime_error("GetColor in class TriangleT is not Contructed");
}
