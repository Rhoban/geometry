#pragma once

#include "rhoban_geometry/point_cluster.h"

namespace rhoban_geometry
{

struct Mate
{
  rhoban_geometry::Point position;
  double angle;
  std::vector<rhoban_geometry::Point> obstacles;
};

class FilteredPointsClusters
{
private:
  std::vector<std::pair<PointCluster, float>> clusters;
  std::vector<int> nbNewObs;

  void updateClusterScore(int clusterIndex, double clusterScoreThreshold, std::map<int, Mate> mates, double discount);

  int historySize;

public:
  FilteredPointsClusters(int historySize = 30)
  {
    historySize = historySize;
  }
  void addPoint(const Point& p, float dist_tol);
  void updateClustersScores(double clusterScoreThreshold, std::map<int, Mate> mates, double discount);
  std::vector<PointCluster> getClusters();
  Point getClusterPosition(int index);
  int getClusterSize(int index);
  int getNbClusters();
  std::string toString();
};
}  // namespace rhoban_geometry

int nbMatesShouldSeePoint(const rhoban_geometry::Point& p, std::map<int, rhoban_geometry::Mate> mates);