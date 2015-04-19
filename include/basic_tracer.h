#ifndef BASIC_TRACER_H_
#define BASIC_TRACER_H_

#include "basic_renderer.h"
#include <vector>

const int kMaxDepth=5;

class Tracer{

 public:

  static int RayTrace(const RayT &r, const std::vector<Renderer*> &objs, PointT &accumulate_color, int now_depth, double rafract_index, bool debug);

};

#endif
