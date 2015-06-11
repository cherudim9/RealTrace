#ifndef BASIC_TRACER_H_
#define BASIC_TRACER_H_

#include "basic_renderer.h"
#include "utility.h"
#include "bounding_volume_hierachy.h"
#include <vector>

const int kMaxDepth=10;

class Tracer{

 public:

  void Init(std::vector<Renderer*> &objs){
    my_slab_collection_=new SlabCollection();
    root_bvh_renderer=new BVHRenderer(10, my_slab_collection_);
    Process tree_building_process("BVH building", 1, 1);
    tree_building_process.Start();
    root_bvh_renderer->Init(objs.begin(), objs.end(), 0);
    tree_building_process.Stop();

    lights_.clear();
    int i=0;
    for(auto a: objs){
      if (a->IsLight()){
	lights_.push_back(a);
	lights_.back()->SetId(i);
      }
      i++;
    }
    //this process must put after tree building, which will alter the order of objs
  }

  int FindFirstHitInVec(const RayT &ray, const std::vector<Renderer*> &objs=std::vector<Renderer*>(), int opt=2);

  int RayTrace(const RayT &r, const std::vector<Renderer*> &objs, PointT &accumulate_color, int now_depth, double refract_index, bool debug);

  int MonteCarloRayTrace(const RayT &r, const std::vector<Renderer*> &objs, PointT &accumulate_color, int now_depth, double refract_index, bool debug, int emission_coefficient=1);

  int enter_times, q_len_tot;

 private:

  BVHRenderer *root_bvh_renderer;
  SlabCollection *my_slab_collection_;
  std::vector<Renderer*> lights_;

};

#endif
