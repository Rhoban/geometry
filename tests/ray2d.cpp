#include <gtest/gtest.h>

#include <rhoban_geometry/ray2d.h>

using namespace rhoban_geometry;

double epsilon = std::pow(10, -12);

TEST(computeIntersections, none)
{
  // Ray pass to the side of the circle
  Ray2D ray(Eigen::Vector2d(1, 0), Eigen::Vector2d(0, 1));
  Circle c(2, 3, 0.5);
  EXPECT_EQ(ray.computeIntersections(c), 0);
}

TEST(computeIntersections, opposite)
{
  // Ray aims opposite to the circle from outside
  Ray2D ray(Eigen::Vector2d(0, 2), Eigen::Vector2d(1, 1));
  Circle c(-1, 1, 0.5);
  EXPECT_EQ(ray.computeIntersections(c), 0);
}

TEST(computeIntersections, exit)
{
  // Ray aims outside of the circle from the inside
  Ray2D ray(Eigen::Vector2d(2, 2), Eigen::Vector2d(1, -1));
  Circle c(-1, 1, 4);
  Point p1, p2;
  EXPECT_EQ(ray.computeIntersections(c, &p1, &p2), 1);
  EXPECT_NEAR(3, p1.x, epsilon);
  EXPECT_NEAR(1, p1.y, epsilon);
  EXPECT_EQ(p2.x, 0);
  EXPECT_EQ(p2.y, 0);
}

TEST(computeIntersections, poke)
{
  // Ray aims opposite to the circle from outside
  Ray2D ray(Eigen::Vector2d(3, 6), Eigen::Vector2d(0, -1));
  Circle c(1, 1, 2);
  Point p1, p2;
  EXPECT_EQ(ray.computeIntersections(c, &p1, &p2), 1);
  EXPECT_NEAR(3, p1.x, epsilon);
  EXPECT_NEAR(1, p1.y, epsilon);
}

TEST(computeIntersections, through)
{
  // Ray enters and leaves the circle
  Ray2D ray(Eigen::Vector2d(-1, 1), Eigen::Vector2d(-1, 0));
  Circle c(-5, 1, 2);
  Point p1, p2;
  EXPECT_EQ(ray.computeIntersections(c, &p1, &p2), 2);
  EXPECT_NEAR(-3, p1.x, epsilon);
  EXPECT_NEAR(1, p1.y, epsilon);
  EXPECT_NEAR(-7, p2.x, epsilon);
  EXPECT_NEAR(1, p2.y, epsilon);
}

int main(int argc, char** argv)
{
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
