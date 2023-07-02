#pragma once

#include "rhoban_geometry/point_cluster.h"

namespace rhoban_geometry
{
class FilteredPointsClusters
{
private:
  std::vector<PointCluster> clusters;
  std::vector<float> clustersScores;
  std::vector<int> nbNewObs;

  void updateClusterScore(int clusterIndex, double clusterScoreThreshold, std::vector<Point> mates,
                          std::vector<double> mates_angles);

public:
  void addPoint(const Point& p, float dist_tol);
  void updateClustersScores(double clusterScoreThreshold, std::vector<Point> mates, std::vector<double> mates_angles);
  std::vector<PointCluster> getClusters();
  Point getClusterPosition(int index);
  int getClusterSize(int index);
  int getNbClusters();
  std::string toString();
};
}  // namespace rhoban_geometry

bool mateShouldSeePoint(const rhoban_geometry::Point& p, const rhoban_geometry::Point& mate_pos, double mate_angle);