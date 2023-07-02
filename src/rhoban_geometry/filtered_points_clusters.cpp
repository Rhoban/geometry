#include "rhoban_geometry/filtered_points_clusters.h"
#include <algorithm>

namespace rhoban_geometry
{
void FilteredPointsClusters::addPoint(const Point& p, float dist_tol, int nb_mates_should_see)
{
  bool accepted = false;
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    if (clusters[i].acceptPoint(p, dist_tol))
    {
      accepted = true;
      clusters[i].push(p);
      nbMatesShouldSee[i] += nb_mates_should_see;
      break;
    }
  }
  // If no cluster matches, create a new one
  if (!accepted)
  {
    clusters.push_back(rhoban_geometry::PointCluster(p));
    clustersScores.push_back(0.6);  // TODO choose this starting score well
    nbMatesShouldSee.push_back(nb_mates_should_see);
  }
}

void FilteredPointsClusters::updateClustersScores(double clusterScoreThreshold)
{
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    updateClusterScore(i, clusterScoreThreshold);
  }
}

void FilteredPointsClusters::updateClusterScore(int clusterIndex, double clusterScoreThreshold)
{
  float score = clusters[clusterIndex].size() / nbMatesShouldSee[clusterIndex];
  float currentScore = clustersScores[clusterIndex];
  float epsilon = 0.0001;
  float newScore = (currentScore * score) / (currentScore * score + (1 - currentScore));

  // capping between [epsilon, 1-epsilon] so that it doesn't go to 0 or 1
  newScore = std::max(epsilon, std::min(1 - epsilon, newScore));

  // If score goes too low, remove cluster
  if (newScore < clusterScoreThreshold)
  {
    clusters.erase(clusters.begin() + clusterIndex);
    nbMatesShouldSee.erase(nbMatesShouldSee.begin() + clusterIndex);
    clustersScores.erase(clustersScores.begin() + clusterIndex);
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

std::vector<Point> FilteredPointsClusters::getClusterPositions()
{
  std::vector<Point> positions;
  for (PointCluster c : clusters)
  {
    auto center = (c.size() >= 3) ? c.getMedian() : c.getAverage();
    positions.push_back(center);
  }
  return positions;
}

};  // namespace rhoban_geometry

bool mateShouldSeePoint(const rhoban_geometry::Point& p, const rhoban_geometry::Point& mate_pos, double mate_angle)
{
  double dist = mate_pos.getDist(p);
  double angle = rhoban_utils::Angle::fromXY(p.x - mate_pos.x, p.y - mate_pos.y).getSignedValueRad();
  double diff_angle = rhoban_utils::Angle(mate_angle - angle, rhoban_utils::Angle::DEG).getSignedValue();

  // TODO 3.0 meters and fov should be parameters
  return (dist < 3.0 && diff_angle > -90 && diff_angle < 90);
}