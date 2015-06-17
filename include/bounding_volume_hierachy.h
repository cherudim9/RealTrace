#ifndef _BOUNDING_VOLUME_HIERACHY_H
#define _BOUNDING_VOLUME_HIERACHY_H

#include "basic_renderer.h"

#define kMaxSlabsNumber 20

class SlabCollection{

 public:

  SlabCollection()
    {
      normals_.clear();
      normals_.push_back(PointT(1.0,0.0,0.0));
      normals_.push_back(PointT(0.0,1.0,0.0));
      normals_.push_back(PointT(0.0,0.0,1.0));
      normals_.push_back(PointT(0.5,0.0,0.5));
      number_of_slab_=normals_.size();
      if (number_of_slab_ > kMaxSlabsNumber)
	throw std::runtime_error("Too many slabs");
      for(auto &a: normals_)
	a=a.Unit();
    }

  int Size()const{
    return normals_.size();
  }

  void DistanceToSlab(Renderer *obj, const int &i, CoordinateT &d1, CoordinateT &d2)const{
    obj->DistanceToSlab(normals_[i], d1, d2);
  }
  
  int number_of_slab_;

  std::vector<PointT> normals_;
  
};

class BVHInitComparetor{
 public:
 BVHInitComparetor(int a):depth(a){}
  bool operator()(Renderer *a, Renderer *b){
    if (a->GetGroup() != b->GetGroup())
      return a->GetGroup() < b->GetGroup();
    return Sign( a->GetBVHIndex(depth) - b->GetBVHIndex(depth) ) < 0;
  }
 private:
  int depth;
};

class BVHRenderer{

 public:

  BVHRenderer(int a=2, SlabCollection *b=NULL)
    :left_son_(NULL), right_son_(NULL),
    renderer_list_(),
    maximum_sons_(a), my_collection_(b),
    min_d_(NULL), max_d_(NULL){
  }

  void Init(std::vector<Renderer*>::iterator objs_begin, std::vector<Renderer*>::iterator objs_end, int before_number);

  void InitCalculation(std::vector<Renderer*>::iterator objs_begin, std::vector<Renderer*>::iterator objs_end, int &answer, double &best);

  bool IsLeaf()const{
    return renderer_list_.size() != 0;
  }

  double Intersect(const RayT &ray)const;

  BVHRenderer *left_son_, *right_son_;
  std::vector<Renderer*> renderer_list_;
  
  int maximum_sons_;
  SlabCollection *my_collection_;
  double *min_d_, *max_d_;
  double *tmp_min_d_[kMaxSlabsNumber], *tmp_max_d_[kMaxSlabsNumber];

  int before_number_;

};

#endif
