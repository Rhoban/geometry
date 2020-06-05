#pragma once

#include "rhoban_geometry/point.h"
#include "rhoban_geometry/circle.h"
#include "rhoban_geometry/parametric_line.h"

namespace rhoban_geometry
{
class Segment
{
public:
  Segment();
  Segment(Point A, Point B);

  // Returns the alpha of the projection of a point, the alpha
  // is the value so that:
  //
  // point1 + alpha*(point2-point1) = point
  //
  // If it is between 0 and 1, the projected point is on the segment,
  // else it is not
  double projectAlpha(const Point& point);

  // Does a point projects on the segment?
  bool projects(const Point& point);

  // Projects a point on this segment
  Point project(const Point& point);

  // Distance from point to segment
  double distance(const Point& point);

  /// Does a circle intersects this segment?
  bool intersects(const Circle& circle);

  // Get the segment length
  double getLength();

  // Does two segments intersect ?
  bool intersects(const Segment segment);

  ParametricLine getLine() const;

  // The two segments
  Point A, B;
};

}  // namespace rhoban_geometry
