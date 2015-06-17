#include "../include/basic_tracer.h"
#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <queue>
using namespace std;

double Abs(double x){
  if (x<0) return -x;
  return x;
}

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

int Tracer::FindFirstHitInVec(const RayT &ray, const std::vector<Renderer*> &objs, int opt){
  if (opt==1){
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

  enter_times++;
  
  priority_queue<pair<double, BVHRenderer*> > Q;
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
        q_len_tot++;
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
      q_len_tot++;
      
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
  if (found)
    return hit_id;
  return -1;
}

int Tracer::RayTrace(const RayT &ray, const std::vector<Renderer*> &objs, PointT &accumulate_color, int now_depth, double refract_index, bool debug){
  
  int hit_seq=1;
  
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
      if (shadow_hit_id==-1){
        cout<<"fuck"<<endl;
        continue;
      }
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
    hit_seq*=RayTrace( RayT(ray_hit, V), objs, recurssive_color, now_depth+1 , refract_index, debug);
    accumulate_color += recurssive_color * reflect * objs[hit_id]->GetColor(ray_hit);
  }

  //-- third part
  double refract=objs[hit_id]->GetRefract();
  if (Sign(refract)>0 && now_depth<kMaxDepth){
    double cos=Dot(I, N);
    double index_ratio = refract_index / objs[hit_id]->GetRefractIndex(ray.GetO(), ray_hit);
    double sin2= Sqr(index_ratio) * ( 1 - Sqr(cos) ) ;
    PointT T= index_ratio * I - ( index_ratio * cos + sqrt( 1 - sin2 ) ) * N ;//refractory direction
    PointT recurssive_color;
    hit_seq*=RayTrace( RayT(ray_hit, T), objs, recurssive_color, now_depth+1, objs[hit_id]->GetRefractIndex(ray.GetO(), ray_hit), debug);
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

  return hit_seq;
}

PointT RandUnitVector(){
  return PointT(RandUnit(), RandUnit(), RandUnit());
}

int Tracer::MonteCarloRayTrace(const RayT &ray, const vector<Renderer*> &objs, PointT &accumulate_color, int now_depth, double refract_index, bool debug, int emission_coefficient, bool into){
  accumulate_color=PointT();
  int hit_seq=FindFirstHitInVec(ray, objs);
  if (hit_seq==-1)
    return hit_seq;
  if (now_depth > kMaxDepth)
    return hit_seq;

  int hit_id=hit_seq;
  Renderer *hit_obj=objs[hit_id];
  accumulate_color = hit_obj->GetEmission() * emission_coefficient;
  PointT ray_hit, reflect_color, refract_color;;
  hit_obj->Intersect(ray, ray_hit);
  PointT N=hit_obj->GetSurfaceNormal(ray_hit, ray.GetO());
  PointT I = ( ray_hit - ray.GetO() ).Unit(); // incident light
  PointT V = ( I - 2.0 * Dot(I, N) * N ).Unit(); // reflected vector
  PointT F=hit_obj->GetColor(ray_hit);
  
  double p=max(F.GetX(), max(F.GetY(), F.GetZ()));
  if (now_depth>5 || !p){
    if (RandUnit()>p){
      return hit_seq;
    }
    else
      F*=1/p;
  }

  double diffuse=objs[hit_id]->GetDiffuse();
  if (diffuse>eps){

    PointT direct_color;
    for(int i=0; i<lights_.size(); i++){
      SphereT *sphere = (SphereT*) lights_[i];
      PointT W=(sphere->GetCenter()-ray_hit), U=Cross( (Abs(W.Unit().GetX())>0.1?PointT(0.0,1.0,0.0):PointT(1.0,0.0,0.0)), W ).Unit(), V=Cross(W,U);
      double max_cosa=sqrt(1.-Sqr(sphere->GetRadius())/(sphere->GetCenter()-ray_hit).Length2());
      double rand1=RandUnit(), rand2=RandUnit();
      double cosa=1-rand1*(1-max_cosa);
      double sina=sqrt(1.0-Sqr(cosa));
      double phi=rand2*2*kPI;
      RayT light_ray=RayT( ray_hit , W*cosa+U*sina*cos(phi)+V*sina*sin(phi) );
      int light_hit_id=FindFirstHitInVec(light_ray, objs);
      if (light_hit_id==sphere->GetId()){
        direct_color += sphere->GetEmission() * Dot(N,light_ray.GetR()) * (2*(1-max_cosa));
      }
    }
    accumulate_color+=direct_color*F*diffuse;

    double a1=2*kPI*RandUnit();
    double r2=RandUnit(), r2_sqrt=sqrt(r2), r1=sqrt(1-r2);
    PointT W=N, U= Cross( ( (Abs(W.GetX())>0.1) ? PointT(0,1,0) : PointT(1,0,0) ) , W).Unit(), V=Cross(W,U).Unit(); 
    RayT diffuse_ray=RayT(ray_hit, W*r1 + U*r2_sqrt*cos(a1) + V*r2_sqrt*sin(a1) );
    PointT diffuse_color;
    hit_seq *= MonteCarloRayTrace(diffuse_ray, objs, diffuse_color, now_depth+1, refract_index, debug, 0, into);
    accumulate_color+=diffuse_color*F*diffuse;
  }

  double reflect=objs[hit_id]->GetReflect();
  if (reflect>eps){
    hit_seq*=MonteCarloRayTrace(RayT(ray_hit, V), objs, reflect_color, now_depth+1, refract_index, debug, 1, into);
    accumulate_color+=reflect_color*F*reflect;
  }

  double refract=objs[hit_id]->GetRefract();
  if (refract>eps){
    //bool into=!((SphereT*)objs[hit_id])->Inside((ray_hit+ray.GetO())*0.5);
    double nc=1., nt=objs[hit_id]->GetRefractIndex(), ratio=(into?nc/nt:nt/nc),  ddn=Dot(ray.GetR(), N);
    double cos2t = 1. - Sqr(ratio) * ( 1. - Sqr(ddn) );
    if (cos2t<0){ //total internal refraction
      hit_seq*=MonteCarloRayTrace(RayT(ray_hit, V), objs, reflect_color, now_depth+1, refract_index, debug, 1, into);
      accumulate_color+=reflect_color*F*refract;
      return hit_seq;
    }
    PointT refract_dir=( ray.GetR()*ratio - N * ( ddn * ratio + sqrt(cos2t) ) ).Unit();

    //hit_seq*=MonteCarloRayTrace(RayT(ray_hit, refract_dir), objs, refract_color, now_depth+1, refract_index, debug);
    //accumulate_color+=refract_color*F*refract;
    //return hit_seq;

    double aa=nt-nc, bb=nt+nc, r0=Sqr(aa/bb), cc= 1. - (into ? -ddn : Dot(refract_dir, into?N:-N ));
    double re=r0+(1-r0)*cc*cc*cc*cc*cc, tr=1-re, p=0.25+0.5*re, rp=re/p, tp=tr/(1-p);
    if (now_depth>2){
      if (RandUnit()<p){
        hit_seq*=MonteCarloRayTrace(RayT(ray_hit, V), objs, reflect_color, now_depth+1, refract_index, debug, 1, into);
        accumulate_color+=F*reflect_color*rp*refract;
      }else{
        hit_seq*=MonteCarloRayTrace(RayT(ray_hit, refract_dir), objs, refract_color, now_depth+1, refract_index, debug, 1, !into);
        accumulate_color+=F*refract_color*tp*refract;
      }
    }else{
      hit_seq*=MonteCarloRayTrace(RayT(ray_hit, V), objs, reflect_color, now_depth+1, refract_index, debug, 1, into);
      hit_seq*=MonteCarloRayTrace(RayT(ray_hit, refract_dir), objs, refract_color, now_depth+1, refract_index, debug, 1, !into);
      accumulate_color+= F * refract * ( reflect_color * re + refract_color * tr );
    }
  }
  
  return hit_seq;
}
