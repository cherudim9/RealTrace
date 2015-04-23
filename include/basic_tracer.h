#ifndef BASIC_TRACER_H_
#define BASIC_TRACER_H_

#include "basic_renderer.h"
#include "utility.h"
#include "bounding_volume_hierachy.h"
#include <vector>

const int kMaxDepth=15;

class Tracer{

 public:

  void Init(std::vector<Renderer*> &objs){
    my_slab_collection_=new SlabCollection();
    root_bvh_renderer=new BVHRenderer(6, my_slab_collection_);
    Process tree_building_process("BVH building", 1, 1);
    tree_building_process.Start();
    root_bvh_renderer->Init(objs.begin(), objs.end(), 0, 0);
    tree_building_process.Stop();
  }

  int FindFirstHitInVec(const RayT &ray, const std::vector<Renderer*> &objs);

  int RayTrace(const RayT &r, const std::vector<Renderer*> &objs, const std::vector<Renderer*> &lights, PointT &accumulate_color, int now_depth, double rafract_index, bool debug);

  int enter_times, q_len_tot;

 private:

  BVHRenderer *root_bvh_renderer;
  SlabCollection *my_slab_collection_;

};

#endif
