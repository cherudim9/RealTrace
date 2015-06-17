#include "../include/bounding_volume_hierachy.h"

using namespace std;

void BVHRenderer::InitCalculation(std::vector<Renderer*>::iterator objs_begin, std::vector<Renderer*>::iterator objs_end, int &answer, double &best){
  int m=my_collection_->Size(), n=objs_end- objs_begin;
  answer=-1;
  double *tmp2_max_d_, *tmp2_min_d_;
  tmp2_max_d_=new double[m];
  tmp2_min_d_=new double[m];

  for(int i=n-1; i>=1; i--){
    double d1, d2;
    for(int j=0; j<m; j++){
      my_collection_->DistanceToSlab(*(objs_begin+i), j, d1, d2);
      if (i==n-1 || d1<tmp2_min_d_[j])
        tmp2_min_d_[j]=d1;
      if (i==n-1 || d2>tmp2_max_d_[j])
        tmp2_max_d_[j]=d2;
    }
      
    double ret=0.0, tmp1[3], tmp2[3];

    for(int j=0; j<3; j++)
      tmp1[j]=(tmp_max_d_[j][i-1] - tmp_min_d_[j][i-1]);
    //    ret+= i * (tmp1[0]*tmp1[1]+tmp1[0]*tmp1[2]+tmp1[1]*tmp1[2]);
    ret += i * 1.0 * tmp1[0] * tmp1[1] * tmp1[2];
      
    for(int j=0; j<3; j++)
      tmp2[j]=(tmp2_max_d_[j] - tmp2_min_d_[j]);
    //ret+= (n-i) * (tmp2[0]*tmp2[1]+tmp2[0]*tmp2[2]+tmp2[1]*tmp2[2]);
    ret += (n-i) * 1.0 * tmp2[0] * tmp2[1] * tmp2[2];

    if (ret<best || answer==-1){
      answer=i;
      best=ret;
    }
  }
}

void BVHRenderer::Init(std::vector<Renderer*>::iterator objs_begin, std::vector<Renderer*>::iterator objs_end, int before_number){
  before_number_=before_number;
  int m=my_collection_->Size(), n=objs_end- objs_begin;
  min_d_=new double[m];
  max_d_=new double[m];
  for(int i=0; i<m; i++){
    tmp_min_d_[i]=new double[n];
    tmp_max_d_[i]=new double[n];
  }
  for(auto obj=objs_begin; obj!=objs_end; obj++){
    for(int i=0; i<m; i++){
      double d1, d2;
      my_collection_->DistanceToSlab(*obj, i, d1, d2);
      if (d1<min_d_[i] || objs_begin==obj)
        min_d_[i]=d1;
      if (d2>max_d_[i] || objs_begin==obj)
        max_d_[i]=d2;
      tmp_min_d_[i][obj-objs_begin]=min_d_[i];
      tmp_max_d_[i][obj-objs_begin]=max_d_[i];
    }
  }
  //  std::cout<<objs_end-objs_begin<<": x"<<min_d_[0]<<" "<<max_d_[0]<<", y"<<min_d_[1]<<", "<<max_d_[1]<<", z"<<min_d_[2]<<" "<<max_d_[2]<<endl;
  if (n <= maximum_sons_){
    renderer_list_.clear();
    for(auto obj=objs_begin; obj!=objs_end; obj++)
      renderer_list_.push_back(*obj);
  }else{
    left_son_=new BVHRenderer(maximum_sons_, my_collection_);
    right_son_=new BVHRenderer(maximum_sons_, my_collection_);

    int answer[3]={-1, -1, -1};
    double best[3];
    double best_best;
    int best_depth=-1;
    
    for(int depth=0; depth<3; depth++){
      sort(objs_begin, objs_end, BVHInitComparetor(depth));
      InitCalculation(objs_begin, objs_end, answer[depth], best[depth]);
      if (best[depth]<best_best || best_depth==-1){
        best_best=best[depth];
        best_depth=depth;
      }
    }
    int left_number=answer[best_depth];
    
    //    if (0)
    for(auto obj=objs_begin+1; obj!=objs_end; obj++)
      if ((*obj)->GetGroup() != (*(obj-1))->GetGroup()){
        left_number=obj-objs_begin;
        break;
      }

    //cout<<left_number<<" "<<objs_end-objs_begin-left_number<<endl;

    sort(objs_begin, objs_end, BVHInitComparetor(best_depth));
    left_son_->Init(objs_begin, objs_begin+left_number, before_number);
    right_son_->Init(objs_begin+left_number, objs_end, before_number+left_number);
  }
  for(int i=0; i<m; i++){
    delete tmp_min_d_[i];
    delete tmp_max_d_[i];
  }
}

double BVHRenderer::Intersect(const RayT &ray)const{
  double t1 = -1e60, t2 = 1e60;
  for(int i=0; i!=my_collection_->Size(); i++){
    PointT &n=my_collection_->normals_[i], r=ray.GetR(), o=ray.GetO();
    double &d1=min_d_[i], &d2=max_d_[i];
    double nr=Dot(n, r);
    if (Sign(nr)==0){
      double tmp=Dot(n,o);
      if (Sign(tmp-d1)>=0 && Sign(tmp-d2)<=0)
        continue;
      else
        return -1.0;
    }
    double no=Dot(n,o), inv_nr=1.0/nr, t3=(d1-no)*inv_nr, t4=(d2-no)*inv_nr;
    if (t3>t4)swap(t3,t4);
    t1=max(t1, t3);
    t2=min(t2, t4);
    if (t1>t2)
      return -1.0;
  }
  t1=max(t1, 0.0);
  if (t1<=t2)
    return t1;
  else
    return -1.0;
}
