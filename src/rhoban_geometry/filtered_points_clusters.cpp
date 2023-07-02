#include "rhoban_geometry/filtered_points_clusters.h"
#include <algorithm>

namespace rhoban_geometry
{
void FilteredPointsClusters::addPoint(const Point& p, float dist_tol, float score, double clusterScoreThreshold)
{
  bool accepted = false;
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    if (clusters[i].acceptPoint(p, dist_tol))
    {
      accepted = true;
      bool ok = updateClusterScore(i, score, clusterScoreThreshold);
      if (ok)
        clusters[i].push(p);
      break;
    }
  }
  // If no cluster matches, create a new one
  if (!accepted)
  {
    clusters.push_back(rhoban_geometry::PointCluster(p));
    clustersScores.push_back(score);
  }
}

bool FilteredPointsClusters::updateClusterScore(int clusterIndex, float score, double clusterScoreThreshold)
{
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
    return false;
  }

  clustersScores[clusterIndex] = newScore;

  return true;
}

void FilteredPointsClusters::getClusters(std::vector<PointCluster>& clusters_out) const
{
  clusters_out = clusters;
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

};  // namespace rhoban_geometry

bool mateShouldSeePoint(const rhoban_geometry::Point& p, const rhoban_geometry::Point& mate_pos, double mate_angle)
{
  double dist = mate_pos.getDist(p);
  double angle = rhoban_utils::Angle::fromXY(p.x - mate_pos.x, p.y - mate_pos.y).getSignedValueRad();
  double diff_angle = rhoban_utils::Angle(mate_angle - angle, rhoban_utils::Angle::DEG).getSignedValue();

  // TODO 3.0 meters and fov should be parameters
  return (dist < 3.0 && diff_angle > -90 && diff_angle < 90);
}