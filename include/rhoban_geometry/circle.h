#pragma once

#include "rhoban_geometry/point.h"
#include "rhoban_geometry/bounding_box.h"

namespace rhoban_geometry
{
class ImpossibleCircleException : public std::exception
{
  const char* what() const throw()
  {
    return "Circle can't be found with given arguments\n";
  };
};

class Circle
{
private:
  Point center;
  double radius;

public:
  Circle() : center(0.0, 0.0), radius(0.0){};
  Circle(const Point& center, double radius) : center(center), radius(radius){};
  Circle(double x, double y, double radius) : center(x, y), radius(radius){};
  Circle(const Circle& other) : center(other.center), radius(other.radius){};

  const Point& getCenter() const
  {
    return center;
  };
  double getRadius() const
  {
    return radius;
  };

  void setCenter(const Point center);
  void setRadius(const double radius);

  Point getPoint(rhoban_utils::Angle theta) const;

  /* Return the theta of this point to the center (position in a clock, in degree) */
  rhoban_utils::Angle getTheta(const Point& p) const;

  /**
   * Does this circle contains the point p ?
   */
  bool contains(const Point& p) const;

  /**
   * Move the given circle of delta
   */
  void translate(const Point& delta);

  /* Return the circle which has two tangents, one in p1 with a vector d1
   * and the other in p2 with a vector d2.
   * Throw ImpossibleCircleException if the given arguments doesn't allow to
   * create a circle
   */
  static Circle circleFromTwoTangents(const Point& p1, const Point& d1, const Point& p2, const Point& d2);

  /* Return the circle which contains the three given points
   * Throw ImpossibleCircleException if the three points are on the same line
   */
  static Circle circleFromPoints(const Point& p1, const Point& p2, const Point& p3);

  /* Return the circle which is inscribed in the triangle composed of p1,p2,p3
   * Throw ImpossibleCircleException if the three points are on the same line
   */
  static Circle inscribedCircle(const Point& p1, const Point& p2, const Point& p3);

  Point projectPoint(const Point& p) const;

  /**
   * Compute the tangents to one circle that pass to the point p
   * There will be usually two results, except if the point is inside
   * the circle where there is none
   */
  std::vector<Point> tangents(const Point& p);

  /**
   * Makes a bounding box over this circle
   */
  BoundingBox getBoundingBox() const;
};

std::ostream& operator<<(std::ostream& out, const rhoban_geometry::Circle& c);

}  // namespace rhoban_geometry
