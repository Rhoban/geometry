#pragma once

#include "rhoban_geometry/point_cluster.h"

namespace rhoban_geometry
{
class FilteredPointsClusters
{
private:
  std::vector<PointCluster> clusters;
  std::vector<float> clustersScores;

  bool updateClusterScore(int clusterIndex, float score, double clusterScoreThreshold);

public:
  void addPoint(const Point& p, float dist_tol, float score, double clusterScoreThreshold);
  void getClusters(std::vector<PointCluster>& clusters_out) const;
  std::string toString();
};
}  // namespace rhoban_geometry

bool mateShouldSeePoint(const rhoban_geometry::Point& p, const rhoban_geometry::Point& mate_pos, double mate_angle);