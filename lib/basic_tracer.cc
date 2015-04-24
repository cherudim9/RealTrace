#include "../include/basic_tracer.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <queue>
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

int Tracer::FindFirstHitInVec(const RayT &ray, const std::vector<Renderer*> &objs){
  if (0){
    double hit_dis=1e30;
    int hit_ret=-1;
    for(int i=0; i!=objs.size(); i++){
      PointT ip;
      double x=objs[i]->Intersect(ray, ip);
      if (Sign(x)>=0)
        if (x<hit_dis){
          hit_dis=x;
          hit_ret=i;
        }
    }
    return hit_ret;
  }
  
  priority_queue<pair<double, BVHRenderer*>> Q;
  bool found=0;
  double answer;
  int hit_id;
  Q.push(make_pair(-root_bvh_renderer->Intersect(ray), root_bvh_renderer));
  int q_len=0;
  while(!Q.empty()){
    BVHRenderer *now_bvh_renderer=Q.top().second;
    double estimated_distance=-Q.top().first;
    Q.pop();
    if (found && estimated_distance>answer)
      continue;
    if (now_bvh_renderer->IsLeaf()){
      int i_number=0;
      for(const auto &i: now_bvh_renderer->renderer_list_){
        PointT ip;
        double t=i->Intersect(ray, ip);
        q_len++;
        if (Sign(t)>0){
          if (!found || (found && answer>t)){
            found=1;
            answer=t;
            hit_id=now_bvh_renderer->before_number_+i_number;
          }            
        }
        i_number++;
      }
    }else{
      double tmp;
      tmp=now_bvh_renderer->left_son_->Intersect(ray);
      q_len+=2;
      if (Sign(tmp)>=0)
        Q.push(make_pair( -(tmp), now_bvh_renderer->left_son_));
      tmp=now_bvh_renderer->right_son_->Intersect(ray);
      if (Sign(tmp)>=0)
        Q.push(make_pair( -(tmp), now_bvh_renderer->right_son_));
    }
  }
  enter_times++;
  q_len_tot+=q_len;
  if (found)
    return hit_id;
  return -1;
}

int Tracer::RayTrace(const RayT &ray, const std::vector<Renderer*> &objs, PointT &accumulate_color, int now_depth, double refract_index, bool debug){
  
  accumulate_color=PointT(0.0, 0.0, 0.0);

  int hit_id=FindFirstHitInVec(ray, objs);
  if (hit_id<0){
    return -1;
  }

  if (objs[hit_id]->IsLight()){
    accumulate_color=objs[hit_id]->GetColor() * objs[hit_id]->GetIntensity();
    return hit_id;
  }

  PointT ray_hit;
  objs[hit_id]->Intersect(ray, ray_hit);

  accumulate_color += objs[hit_id]->GetEmission();

  PointT N=objs[hit_id]->GetSurfaceNormal(ray_hit, ray.GetO()); // normal vector
  PointT I = ( ray_hit - ray.GetO() ).Unit(); // incident light
  PointT V = ( I - 2.0 * Dot(I, N) * N ).Unit(); // reflected vector
  
  //let's calculate the color or ray_hit

  //-- first part
  for(int i=0; i!=lights_.size(); i++)
    if (lights_[i]->IsLight()){
      //check whether the light is shadowed
      SphereT *sphere = (SphereT*)lights_[i]; //????
      RayT light=RayT( sphere->GetCenter(), ray_hit - sphere->GetCenter());
      light=RayT(light.GetO()+sphere->GetRadius()*light.GetR(), light.GetR());
      int shadow_hit_id = FindFirstHitInVec(light, objs);
      PointT light_hit;
      objs[shadow_hit_id]->Intersect(light, light_hit);
      bool Shadowed = ( light_hit != ray_hit );
      if (Shadowed)
        continue;
      
      //diffuse light
      PointT L= ( sphere->GetCenter() - ray_hit ).Unit(); //light vector
      CoordinateT dot=Dot(N, L);
      if (Sign(dot)>0){
        accumulate_color += lights_[i]->GetIntensity() * objs[hit_id]->GetDiffuse() * dot * objs[hit_id]->GetColor(ray_hit);
      }

      //specular light
      dot=Dot(V,L);
      if (Sign(dot)>0){
        accumulate_color += lights_[i]->GetIntensity() * objs[hit_id]->GetReflect() * Power(dot, 35) * lights_[i]->GetColor(ray_hit);
      }
    }

  //-- second part
  double reflect=objs[hit_id]->GetReflect();
  if (Sign(reflect)>0 && now_depth<kMaxDepth){
    PointT recurssive_color;
    RayTrace( RayT(ray_hit, V), objs, recurssive_color, now_depth+1 , refract_index, debug);
    accumulate_color += recurssive_color * reflect * objs[hit_id]->GetColor(ray_hit);
  }

  //-- third part
  double refract=objs[hit_id]->GetRefract();
  if (Sign(refract)>0 && now_depth<kMaxDepth){
    double cos=Dot(I, N);
    double index_ratio = refract_index / objs[hit_id]->GetRefractIndex(ray.GetO());
    double sin2= Sqr(index_ratio) * ( 1 - Sqr(cos) ) ;
    PointT T= index_ratio * I - ( index_ratio * cos + sqrt( 1 - sin2 ) ) * N ;//refractory direction
    PointT recurssive_color;
    RayTrace( RayT(ray_hit, T), objs, recurssive_color, now_depth+1, objs[hit_id]->GetRefractIndex(ray.GetO()), debug);
    double ratio=1.0;//Beer's Law
    if (index_ratio < 1.0){
      ratio=exp(-0.010*(ray_hit-ray.GetO()).Length());
    }
    accumulate_color += recurssive_color * refract * objs[hit_id]->GetColor(ray_hit) * ratio;
  }

  if (debug){
    printf("-> (%f, %f, %f) to (%f, %f, %f), color=(%f, %f, %f)\n", 
           ray.GetO().GetX(), ray.GetO().GetY(), ray.GetO().GetZ(),           
           ray_hit.GetX(), ray_hit.GetY(), ray_hit.GetZ(),
           accumulate_color.GetX(), accumulate_color.GetY(), accumulate_color.GetZ());
  }

  return hit_id;
}
