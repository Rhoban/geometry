#include "rhoban_geometry/filtered_points_clusters.h"
#include <algorithm>

namespace rhoban_geometry
{
void FilteredPointsClusters::addPoint(const Point& p, float dist_tol)
{
  bool accepted = false;
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    if (clusters[i].acceptPoint(p, dist_tol))
    {
      accepted = true;
      clusters[i].push(p);
      nbNewObs[i]++;
      break;
    }
  }
  // If no cluster matches, create a new one
  if (!accepted)
  {
    clusters.push_back(rhoban_geometry::PointCluster(p));
    clustersScores.push_back(0.6);  // TODO choose this starting score well
    nbNewObs.push_back(1);
  }
}

void FilteredPointsClusters::updateClustersScores(double clusterScoreThreshold, std::vector<Point> mates_pos,
                                                  std::vector<double> mates_angles)
{
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    updateClusterScore(i, clusterScoreThreshold, mates_pos, mates_angles);
  }
}

void FilteredPointsClusters::updateClusterScore(int clusterIndex, double clusterScoreThreshold,
                                                std::vector<Point> mates_pos, std::vector<double> mates_angles)
{
  int nbMatesShouldSee = 0;
  for (int i = 0; i < mates_pos.size(); i++)
    if (mateShouldSeePoint(getClusterPosition(clusterIndex), mates_pos[i], mates_angles[i]))
      nbMatesShouldSee++;

  float score = nbNewObs[clusterIndex] / nbMatesShouldSee;
  nbNewObs[clusterIndex] = 0;
  float currentScore = clustersScores[clusterIndex];
  float epsilon = 0.0001;
  float newScore = (currentScore * score) / (currentScore * score + (1 - currentScore));

  // capping between [epsilon, 1-epsilon] so that it doesn't go to 0 or 1
  newScore = std::max(epsilon, std::min(1 - epsilon, newScore));

  // If score goes too low, remove cluster
  if (newScore < clusterScoreThreshold)
  {
    clusters.erase(clusters.begin() + clusterIndex);
    clustersScores.erase(clustersScores.begin() + clusterIndex);
    nbNewObs.erase(nbNewObs.begin() + clusterIndex);
    return;
  }

  clustersScores[clusterIndex] = newScore;
}

std::vector<PointCluster> FilteredPointsClusters::getClusters()
{
  return clusters;
}

std::string FilteredPointsClusters::toString()
{
  std::ostringstream oss;
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    oss << "Cluster " << i << " : score: " << clustersScores[i] << std::endl;
  }
  return oss.str();
}

Point FilteredPointsClusters::getClusterPosition(int index)
{
  PointCluster c = clusters[index];
  auto center = (c.size() >= 3) ? c.getMedian() : c.getAverage();
  return center;
}

int FilteredPointsClusters::getClusterSize(int index)
{
  return clusters[index].size();
}

int FilteredPointsClusters::getNbClusters()
{
  return clusters.size();
}

}  // namespace rhoban_geometry

bool mateShouldSeePoint(const rhoban_geometry::Point& p, const rhoban_geometry::Point& mate_pos, double mate_angle)
{
  double dist = mate_pos.getDist(p);
  double angle = rhoban_utils::Angle::fromXY(p.x - mate_pos.x, p.y - mate_pos.y).getSignedValueRad();
  double diff_angle = rhoban_utils::Angle(mate_angle - angle, rhoban_utils::Angle::DEG).getSignedValue();

  // TODO 3.0 meters and fov should be parameters
  return (dist < 3.0 && diff_angle > -90 && diff_angle < 90);
}