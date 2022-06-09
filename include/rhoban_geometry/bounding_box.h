#pragma once

#include "rhoban_geometry/point.h"

namespace rhoban_geometry
{
class BoundingBox
{
public:
  BoundingBox();
  BoundingBox(Point lower_left, Point upper_right);

  float surface() const;
  bool intersects(const BoundingBox& other) const;
  float iou(const BoundingBox& other) const;

  Point lower_left;
  Point upper_right;
};
}  // namespace rhoban_geometry