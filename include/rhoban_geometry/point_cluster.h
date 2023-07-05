#pragma once

#include "rhoban_geometry/point.h"

namespace rhoban_geometry
{
class PointCluster
{
private:
  std::vector<Point> points;
  Point average;

public:
  PointCluster(const Point& p) : average(p)
  {
    points.push_back(p);
  }

  unsigned int size() const
  {
    return points.size();
  }

  const Point& getAverage() const
  {
    return average;
  }

  /* no const policy applied */
  Point getMedian();

  bool acceptPoint(const Point& candidate, float dist_tol) const;

  void push(const Point& p);

  void erase(const std::vector<Point>::iterator it);

  std::vector<Point>::iterator begin()
  {
    return points.begin();
  }
};

}  // namespace rhoban_geometry

/**
 * Add the given point to the clusters, if the point doesn't match any of
 * the clusters, then create a new Cluster
 */
void addToClusters(const rhoban_geometry::Point& p, std::vector<rhoban_geometry::PointCluster>& clusters,
                   float dist_tol);

std::vector<rhoban_geometry::PointCluster> createClusters(const std::vector<rhoban_geometry::Point>& points,
                                                          float dist_tol);
