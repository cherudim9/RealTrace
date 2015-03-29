#include "../include/basic_tracer.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
using namespace std;

CoordinateT Power(CoordinateT a, int n){
  if (n<1)
    throw runtime_error("power less than 1");
  CoordinateT ret=1.0;
  while(n)
    if (n&1)
      ret*=a, n--;
    else
      a*=a, n>>=1;
  return ret;
}

int Tracer::RayTrace(const RayT &ray, const std::vector<Renderer*> &objs, PointT &accumulate_color, int now_depth){
  
  accumulate_color=PointT(0.0, 0.0, 0.0);

  int hit_id=ray.FindFirstHitInVec(objs);
  if (hit_id<0){
    return -1;
  }

  if (objs[hit_id]->IsLight()){
    accumulate_color=objs[hit_id]->GetColor() * objs[hit_id]->GetIntensity();
    return hit_id;
  }

  PointT ray_hit;
  double t=objs[hit_id]->Intersect(ray, ray_hit);

  PointT N=objs[hit_id]->GetSurfaceNormal(ray_hit); // normal vector
  PointT I = ( ray_hit - ray.GetO() ).Unit(); // incident light
  PointT V = ( I - 2.0 * Dot(I, N) * N ).Unit(); // reflected vector
  
  //let's calculate the color or ray_hit

  //-- first part
  for(int i=0; i!=objs.size(); i++)
    if (objs[i]->IsLight()){
      //check whether the light is shadowed
      SphereT *sphere = (SphereT*)objs[i]; //????
      RayT light=RayT( sphere->GetCenter(), ray_hit - sphere->GetCenter());
      light=RayT(light.GetO()+sphere->GetRadius()*light.GetR(), light.GetR());
      int shadow_hit_id = light.FindFirstHitInVec(objs);
      PointT light_hit;
      objs[shadow_hit_id]->Intersect(light, light_hit);
      bool Shadowed = ( light_hit != ray_hit );
      if (Shadowed)
        continue;
      
      //diffuse light
      PointT L= ( sphere->GetCenter() - ray_hit ).Unit(); //light vector
      CoordinateT dot=Dot(N, L);
      //      assert( dot >= -1.0 - eps && dot <= 1.0 + eps );
      if (Sign(dot)>0){
        accumulate_color += objs[i]->GetIntensity() * objs[hit_id]->GetDiffuse() * dot * objs[hit_id]->GetColor();
      }

      //specular light
      dot=Dot(V,L);
      if (Sign(dot)>0){
        accumulate_color += objs[i]->GetIntensity() * objs[hit_id]->GetReflect() * Power(dot, 20) * objs[i]->GetColor();
      }
    }

  //-- second part
  double reflect=objs[hit_id]->GetReflect();
  if (Sign(reflect)>0 && now_depth<kMaxDepth){
    PointT recurssive_color;
    RayTrace( RayT(ray_hit, V), objs, recurssive_color, now_depth+1 );
    accumulate_color += recurssive_color * reflect * objs[hit_id]->GetColor();
  }

  return hit_id;
}
