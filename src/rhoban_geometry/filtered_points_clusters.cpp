#include "rhoban_geometry/filtered_points_clusters.h"
#include <algorithm>

namespace rhoban_geometry
{

void FilteredPointsClusters::addPoint(const Point& p, float dist_tol)
{
  bool accepted = false;
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    if (clusters[i].first.acceptPoint(p, dist_tol))
    {
      accepted = true;
      clusters[i].first.push(p);
      // if (clusters[i].first.size() > historySize)
      // {
      //   clusters[i].first.erase(clusters[i].first.begin());
      // }
      nbNewObs[i]++;
      break;
    }
  }
  // If no cluster matches, create a new one
  if (!accepted)
  {
    clusters.push_back(std::pair(rhoban_geometry::PointCluster(p), 0.6));  // TODO choose this starting score well
    nbNewObs.push_back(1);
  }
}

void FilteredPointsClusters::updateClustersScores(double clusterScoreThreshold, std::map<int, Mate> mates)
{
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    updateClusterScore(i, clusterScoreThreshold, mates);
  }
}

void FilteredPointsClusters::updateClusterScore(int clusterIndex, double clusterScoreThreshold,
                                                std::map<int, Mate> mates)
{
  int nbMatesShouldSee = nbMatesShouldSeePoint(getClusterPosition(clusterIndex), mates);

  float epsilon = 0.0001;
  float score = nbNewObs[clusterIndex] / (nbMatesShouldSee + epsilon);
  nbNewObs[clusterIndex] = 0;
  float currentScore = clusters[clusterIndex].second;
  float newScore = (currentScore * score) / (currentScore * score + (1 - currentScore));

  // capping between [epsilon, 1-epsilon] so that it doesn't go to 0 or 1
  newScore = std::max(epsilon, std::min(1 - epsilon, newScore));

  // If score goes too low, remove cluster
  if (newScore < clusterScoreThreshold)
  {
    clusters.erase(clusters.begin() + clusterIndex);
    nbNewObs.erase(nbNewObs.begin() + clusterIndex);
    return;
  }

  clusters[clusterIndex].second = newScore;
}

std::vector<PointCluster> FilteredPointsClusters::getClusters()
{
  std::vector<PointCluster> clusters;
  for (unsigned int i = 0; i < this->clusters.size(); i++)
  {
    clusters.push_back(this->clusters[i].first);
  }
  return clusters;
}

std::string FilteredPointsClusters::toString()
{
  std::ostringstream oss;
  for (unsigned int i = 0; i < clusters.size(); i++)
  {
    oss << "Cluster " << i << " : score: " << clusters[i].second << std::endl;
  }
  return oss.str();
}

Point FilteredPointsClusters::getClusterPosition(int index)
{
  PointCluster c = clusters[index].first;
  auto center = (c.size() >= 3) ? c.getMedian() : c.getAverage();
  return center;
}

int FilteredPointsClusters::getClusterSize(int index)
{
  return clusters[index].first.size();
}

int FilteredPointsClusters::getNbClusters()
{
  return clusters.size();
}

}  // namespace rhoban_geometry

int nbMatesShouldSeePoint(const rhoban_geometry::Point& p, std::map<int, rhoban_geometry::Mate> mates)
{
  int nb = 0;
  for (std::map<int, rhoban_geometry::Mate>::iterator it = mates.begin(); it != mates.end(); ++it)
  {
    rhoban_geometry::Point mate_pos = it->second.position;
    double mate_angle = it->second.angle;

    double dist = mate_pos.getDist(p);
    double angle = rhoban_utils::Angle::fromXY(p.x - mate_pos.x, p.y - mate_pos.y).getSignedValueRad();
    double diff_angle = rhoban_utils::Angle(mate_angle - angle, rhoban_utils::Angle::DEG).getSignedValue();

    // TODO 3.0 meters and fov should be parameters
    if (dist < 3.0 && diff_angle > -90 && diff_angle < 90)
      nb++;
  }
  return nb;
}