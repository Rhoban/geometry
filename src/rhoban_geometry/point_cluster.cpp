#include "rhoban_geometry/point_cluster.h"
#include <algorithm>

namespace rhoban_geometry
{
bool PointCluster::acceptPoint(const Point& candidate, float dist_tol) const
{
  return average.getDist(candidate) <= dist_tol;
}

rhoban_geometry::Point PointCluster::getMedian()
{
  rhoban_geometry::Point result;
  if (points.size() > 0)
  {
    std::sort(points.begin(), points.end(),
              [](const Point& lhs, const Point& rhs) { return lhs.x + lhs.y < rhs.x + rhs.y; });
    return points[points.size() / 2];
  }
  return result;
}

void PointCluster::push(const Point& p)
{
  // Updating average
  int k = size();
  average = (average * k + p) / (k + 1);

  points.push_back(p);
}
void PointCluster::erase(const std::vector<Point>::iterator it)
{
  Point p = *it;
  points.erase(it);

  // Updating average
  int k = size();
  average = (average * k - p) / (k - 1);
}

}  // namespace rhoban_geometry

void addToClusters(const rhoban_geometry::Point& p, std::vector<rhoban_geometry::PointCluster>& clusters,
                   float dist_tol)
{
  bool accepted = false;
  // Insert in a cluster if possible
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    if (clusters[i].acceptPoint(p, dist_tol))
    {
      accepted = true;
      clusters[i].push(p);
      break;
    }
  }
  // If no cluster matches, create a new one
  if (!accepted)
  {
    clusters.push_back(rhoban_geometry::PointCluster(p));
  }
}

std::vector<rhoban_geometry::PointCluster> createClusters(const std::vector<rhoban_geometry::Point>& points,
                                                          float dist_tol)
{
  std::vector<rhoban_geometry::PointCluster> clusters;
  for (const rhoban_geometry::Point& p : points)
  {
    addToClusters(p, clusters, dist_tol);
  }
  return clusters;
}
