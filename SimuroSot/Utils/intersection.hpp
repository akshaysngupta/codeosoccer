#ifndef INTERSECTION_HPP
#define INTERSECTION_HPP
#include <stdio.h>
#include <math.h>
#include <algorithm>
#include <vector>
#include <set>
#include <assert.h>
#include "..\common\include\geometry.hpp"
using namespace std;

namespace Intersection
{
/**
 * @brief 
 * @param p1
 * @param p2
 * @param p3
 * @return 
 */
int findDet(Point2D<int> p1, Point2D<int> p2, Point2D<int> p3);
// Does not work!!!!
inline bool lineIntersection(Point2D<int> p1, Point2D<int> p2, Point2D<int> p3, Point2D<int> p4);
// Works
bool lineIntersection2(Point2D<int> p1, Point2D<int> p2, Point2D<int> p3, Point2D<int> p4);
bool pointInCircle(double x, double y, double cx, double cy, double r);
bool lineInCircle(double ax, double ay, double bx, double by, double cx, double cy, double cr);
std::pair<Point2D<int>, Point2D<int> > getLineCirclePoints(double ax, double ay, double bx, double by, double cx, double cy, double cr);
}


struct AngleInterval
{
  double start;
  mutable double end;
  AngleInterval(double start, double end):start(start), end(end) {}
  bool operator<(const AngleInterval& ai) const;
};

class AngleChooser
{
  std::set<AngleInterval> invalidSet;
  bool insideInterval(double angle1, double angle2, double angle);

public:
  AngleChooser();
  static inline float angleDifference(float angle1, float angle2)
  {
    if (angle1 > angle2)
      return fabs((float)2*PI - (angle1 - angle2));
    return fabs((float)angle1 - angle2);
  }
  bool isInvalid(double angle);
  void setInvalid(double angle1, double angle2);
  void printInvalid();
  std::pair<double,double> getLimits(double angle);
};

/*int main()
{
	Point2D<int> p1(0,0),p2(1,1),p3(0,1),p4(1,0);
	if(lineIntersection(p1,p2,p3,p4))
		printf("Lines intersect\n");
	else
		printf("Lines do not intersect\n");
	p1=Point2D<int>(0,0);
	p2=Point2D<int>(1,1);
	p3=Point2D<int>(2,0);
	p4=Point2D<int>(0,3);
	if(lineIntersection(p1,p2,p3,p4))
		printf("Lines intersect\n");
	else
		printf("Lines do not intersect\n");
	if(lineInCircle(-2,0,-1.5,0,0,0,1))
		printf("Lines intersect\n");
	else
		printf("Lines do not intersect\n");
	if(lineInCircle(-0.5,0,-1.5,0,0,0,1))
		printf("Lines intersect\n");
	else
		printf("Lines do not intersect\n");
}*/
#endif
