#pragma once

#include "rhoban_geometry/point_cluster.h"

namespace rhoban_geometry
{
class FilteredPointsClusters
{
private:
  std::vector<PointCluster> clusters;
  std::vector<float> nbMatesShouldSee;
  std::vector<float> clustersScores;

  void updateClusterScore(int clusterIndex, double clusterScoreThreshold);

public:
  void addPoint(const Point& p, float dist_tol, int nb_mates_should_see);
  void updateClustersScores(double clusterScoreThreshold);
  std::vector<PointCluster> getClusters();
  std::vector<Point> getClusterPositions();
  std::string toString();
};
}  // namespace rhoban_geometry

bool mateShouldSeePoint(const rhoban_geometry::Point& p, const rhoban_geometry::Point& mate_pos, double mate_angle);