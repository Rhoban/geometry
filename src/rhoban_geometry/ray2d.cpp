#include <rhoban_geometry/ray2d.h>

namespace rhoban_geometry
{
Ray2D::Ray2D(const Eigen::Vector2d& src, const Eigen::Vector2d& dir) : src(src), dir(dir.normalized())
{
}

int Ray2D::computeIntersections(const Circle& circle, Point* i1, Point* i2) const
{
  // Implementation based on: https://stackoverflow.com/a/1084899
  Eigen::Vector2d CS = src - circle.getCenter().toVector();
  double r = circle.getRadius();
  double a = dir.dot(dir);
  double b = 2 * CS.dot(dir);
  double c = CS.dot(CS) - r * r;

  double discriminant = b * b - 4 * a * c;

  if (discriminant < 0)
  {
    return 0;
  }

  double d = std::sqrt(discriminant);
  int nb_sols = 0;
  double t1 = (-b - d) / (2 * a);
  double t2 = (-b + d) / (2 * a);

  if (t1 > 0)
  {
    nb_sols++;
    if (i1)
      *i1 = Point(src + t1 * dir);
  }
  if (d != 0 && t2 > 0)
  {
    nb_sols++;
    if (nb_sols == 1 && i1)
      *i1 = Point(src + t2 * dir);
    if (nb_sols == 2 && i2)
      *i2 = Point(src + t2 * dir);
  }
  return nb_sols;
}

}  // namespace rhoban_geometry
