#pragma once

#include <iostream>
#include <vector>

#include "rhoban_utils/angle.h"

#include <Eigen/Core>

namespace rhoban_geometry
{

class Point{
public:
  double x;
  double y;
  Point(): x(0.0), y(0.0) {};
  Point(const Point & other): x(other.x), y(other.y) {};
  Point(double x, double y): x(x), y(y) {};
  Point(const Eigen::Vector2d & p);

  double getX() const{ return x;};
  double getY() const{ return y;};

//TODO handle case with point at 0.0, 0.0 in getTheta and isCollinear
  rhoban_utils::Angle getTheta() const;
  bool isCollinear(const Point & other) const;
  /* Return the distance to point in {0,0} */
  double getLength() const;
  double getDist(const Point & other) const;
  Point & normalize(double newLength = 1);

  /* Return a Point with a rotation of 90° */
  Point perpendicular() const;
  Point rotation(const rhoban_utils::Angle & a) const;
  Point rotation(const rhoban_utils::Angle & a,
                 const Point & rotCenter) const;

  static double dotProduct(const Point & p1, const Point & p2);
  static double perpDotProduct(const Point & p1, const Point & p2);

  Point operator-() const;

  Point operator+(const Point & other) const;
  Point operator-(const Point & other) const;
  Point operator*(double ratio) const;
  Point operator/(double ratio) const;

  Point& operator+=(const Point & other);
  Point& operator-=(const Point & other);
  Point& operator*=(double ratio);
  Point& operator/=(double ratio);

  static Point mkPointFromPolar(double rho, rhoban_utils::Angle theta);

  // Random points creators
  static Point mkRandomPolar(double rhoMax);
  static Point mkRandomPolar(double rhoMin, double rhoMax,
                             double thetaMin =   0.0,
                             double thetaMax = 360.0);

  bool equals(const Point & other , double precision = FLOAT_PRECISION) const;
  bool operator==(const Point & other) const;

private:
  constexpr static const double FLOAT_PRECISION = 0.001;
};


rhoban_geometry::Point operator*(double ratio, const rhoban_geometry::Point & p);

std::ostream& operator<<(std::ostream& out, const rhoban_geometry::Point & p);

}

/* Return the point which is the average of the points in the vector */
rhoban_geometry::Point average(const std::vector<rhoban_geometry::Point> & points);
/* Return sum(points[i]* weights[i]) / sum(weights[i]) */
rhoban_geometry::Point average(const std::vector<rhoban_geometry::Point> & points,
                               const std::vector<double> & weights);
/* Return the standard deviation to the average of the points */
double stdDev(const std::vector<rhoban_geometry::Point> & points);
