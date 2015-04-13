#ifndef _VIEW_H
#define _VIEW_H

#include "basic_geometry.h"
#include "basic_renderer.h"

class Viewer{

 public:

  Viewer()
    :init_(false){}

  PointT GetCameraPos()const{ return camera_pos_; }
  PointT GetEyeDirection()const{ return eye_direction_; }
  PointT GetHeadDirection()const{ return head_direction_; }
  PointT GetRighthandDirection()const{ return righthand_direction_; }
  double GetDistance()const{ return distance_; }
  double GetGeometryWidth()const{ return geometry_width_; }

  void SetCameraPos(PointT a){ camera_pos_=a; }
  void SetEyeDirection(PointT a){ eye_direction_=a; }
  void SetHeadDirection(PointT a){ head_direction_=a; }
  void SetRighthandDirection(PointT a){ righthand_direction_=a; }
  void SetDistance(double a){ distance_=a; }
  void SetGeometryWidth(double a){ geometry_width_=a; }

  void Init(){
    init_=true;
    eye_direction_.Normalize();
    head_direction_.Normalize();
    righthand_direction_=Cross(head_direction_, eye_direction_);
    head_direction_=Cross(eye_direction_, righthand_direction_);
  }

  RayT GetRay(int picture_w, int picture_h, int picture_i, int picture_j)const{
    if (!init_)
      throw std::runtime_error("Viewer not initialized yet");
    double geometry_height_ = geometry_width_ / picture_w * picture_h;
    double geo_x = (1.0 * picture_i / picture_w) * geometry_width_ - geometry_width_/2.0;
    double geo_y = (1.0 * picture_j / picture_h) * geometry_height_ - geometry_height_/2.0;
    return RayT(camera_pos_, 
		eye_direction_ * distance_ +
		head_direction_ * geo_y +
		righthand_direction_ * geo_x);
  }

 private:
  
  PointT camera_pos_;
  PointT eye_direction_, head_direction_, righthand_direction_;
  double distance_, geometry_width_;

  bool init_;

};

#endif
