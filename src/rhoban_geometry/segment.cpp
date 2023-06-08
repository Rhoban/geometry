#include <cmath>
#include "rhoban_geometry/segment.h"

namespace rhoban_geometry
{
Segment::Segment() : A(0.0, 0.0), B(0.0, 0.0)
{
}

Segment::Segment(Point A, Point B) : A(A), B(B)
{
}

double Segment::projectAlpha(const Point& P)
{
  return ((A.x - B.x) * (A.x - P.x) + (A.y - B.y) * (A.y - P.y)) / (pow(A.x - B.x, 2) + pow(A.y - B.y, 2));
}

bool Segment::projects(const Point& point)
{
  double alpha = projectAlpha(point);

  return alpha >= 0 && alpha <= 1;
}

Point Segment::project(const Point& point)
{
  double alpha = projectAlpha(point);

  return A + alpha * (B - A);
}

double Segment::distance(const Point& point)
{
  auto projection = project(point);

  return (projection - point).getLength();
}

bool Segment::intersects(const Circle& circle)
{
  // Circle parameters
  double Cx = circle.getCenter().x;
  double Cy = circle.getCenter().y;
  double Cr2 = pow(circle.getRadius(), 2);

  // We want a*alpha^2 + b*alpha + c = radius^2
  double c = pow(A.x, 2) - 2 * A.x * Cx + pow(A.y, 2) - 2 * A.y * Cy - Cr2 + pow(Cx, 2) + pow(Cy, 2);
  double b = (-2 * pow(A.x, 2) + 2 * A.x * B.x + 2 * A.x * Cx - 2 * pow(A.y, 2) + 2 * A.y * B.y + 2 * A.y * Cy -
              2 * B.x * Cx - 2 * B.y * Cy);
  double a = (pow(A.x, 2) - 2 * A.x * B.x + pow(A.y, 2) - 2 * A.y * B.y + pow(B.x, 2) + pow(B.y, 2));

  double det = pow(b, 2) - 4 * a * c;

  if (det > 0 && a != 0)
  {
    double a1 = (-b - sqrt(det)) / (2 * a);
    double a2 = (-b + sqrt(det)) / (2 * a);

    return (a1 >= 0 && a1 <= 1) || (a2 >= 0 && a2 <= 1) || (a1 < 0 && a2 > 1);
  }

  return false;
}

bool Segment::intersects(const Segment segment)
{
  Eigen::Vector2d A1 = (A).toVector();
  Eigen::Vector2d u1 = (B - A).toVector();
  Eigen::Vector2d A2 = (segment.A).toVector();
  Eigen::Vector2d u2 = (segment.B - segment.A).toVector();

  // We check that there is an intersection between the two segments
  // A1 + lambda1 u1 = A2 + lambda2 u2
  // lambda1 u1 - lambda2 u2 = A2 - A1

  Eigen::Matrix2d M;
  M << u1(0), -u2(0), u1(1), -u2(1);

  // The segments are likely parallel
  if (M.determinant() == 0)
  {
    return false;
  }

  // We solve the system
  Eigen::Vector2d V = A2 - A1;
  Eigen::Vector2d lambdas = M.inverse() * V;

  // We check that the intersection is in the segments and not in the lines
  return lambdas(0) >= 0 && lambdas(0) <= 1 && lambdas(1) >= 0 && lambdas(1) <= 1;
}

BoundingBox Segment::getBoundingBox() const
{
  BoundingBox bbox;
  bbox.lower_left.x = std::min(A.x, B.x);
  bbox.lower_left.y = std::min(A.y, B.y);
  bbox.upper_right.x = std::max(A.x, B.x);
  bbox.upper_right.y = std::max(A.y, B.y);

  return bbox;
}

double Segment::getLength()
{
  return (B - A).getLength();
}

ParametricLine Segment::getLine() const
{
  return ParametricLine(A, B);
}

}  // namespace rhoban_geometry
