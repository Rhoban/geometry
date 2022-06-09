#include "rhoban_geometry/bounding_box.h"

namespace rhoban_geometry
{
BoundingBox::BoundingBox() : lower_left(0, 0), upper_right(0, 0)
{
}

BoundingBox::BoundingBox(Point lower_left, Point upper_right) : lower_left(lower_left), upper_right(upper_right)
{
}

float BoundingBox::surface() const
{
  return fabs(upper_right.x - lower_left.x) * fabs(upper_right.y - lower_left.y);
}

bool BoundingBox::intersects(const BoundingBox& other) const
{
  return (lower_left.x < other.upper_right.x && upper_right.x > other.lower_left.x) &&
         (lower_left.y < other.upper_right.y && upper_right.y > other.lower_left.y);
}

float BoundingBox::iou(const BoundingBox& other) const
{
  if (intersects(other))
  {
    float dx = std::min(upper_right.x, other.upper_right.x) - std::max(lower_left.x, other.lower_left.x);
    float dy = std::min(upper_right.y, other.upper_right.y) - std::max(lower_left.y, other.lower_left.y);

    float intersection = dx * dy;
    float union_ = (surface() + other.surface()) - intersection;

    return intersection / union_;
  }
  else
  {
    return 0.;
  }
}
}  // namespace rhoban_geometry