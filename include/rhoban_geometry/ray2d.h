#pragma once

#include "rhoban_geometry/point.h"
#include "rhoban_geometry/circle.h"

namespace rhoban_geometry
{
class Ray2D
{
public:
  Ray2D(const Eigen::Vector2d& src, const Eigen::Vector2d& dir);
  /// Compute the number of intersections between the ray and the provided circle.
  /// If i1 and/or i2 are not nullptr, the intersections are stored
  int computeIntersections(const Circle& c, Point* i1 = nullptr, Point* i2 = nullptr) const;

private:
  Eigen::Vector2d src;
  Eigen::Vector2d dir;
};
}  // namespace rhoban_geometry
