#include "stdafx.h"
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include "pathPlanners.h"
#include "../common/include/config.h"
#include "intersection.hpp"
#include <algorithm>
using namespace Util;

namespace MyStrategy
{
  std::vector<std::pair<Point2D<int>, Point2D<int> > > LocalAvoidance::boundaries;
  std::vector<std::pair<Point2D<int>, Point2D<int> > > LocalAvoidance::closeBoundaries;
  void LocalAvoidance::_addLineToBoundaries(vector<int> xi, vector<int> yi)
  {
    assert(xi.size() == 12 && yi.size() == 12);
    for(int i=0; i<11; i++)
    {
      addLineToBoundaries(xi[i], yi[i], xi[i+1], yi[i+1] );      
    }
    addLineToBoundaries(xi[11], yi[11], xi[0], yi[0]);
  }
  LocalAvoidance::LocalAvoidance() :
    phiStar(0),
    phi(0),
    theta(0),
    thetaD(0),
    thetaDD(0),
    alpha(0),
    beta(0)
  {
    //adding all 12 corner points.
    boundaries.clear();
    closeBoundaries.clear();
    vector<int> xi;
    vector<int> yi;
    xi.push_back(-4220); yi.push_back(800);
    xi.push_back(-4220); yi.push_back(3350);
    xi.push_back(4220); yi.push_back(3350);
    xi.push_back(4220); yi.push_back(800);
    xi.push_back(4630); yi.push_back(800);
    xi.push_back(4630); yi.push_back(-800);
    xi.push_back(4220); yi.push_back(-925);
    xi.push_back(4220); yi.push_back(-3350);
    xi.push_back(-4220); yi.push_back(-3350);
    xi.push_back(-4220); yi.push_back(-925);
    xi.push_back(-4630); yi.push_back(-800);
    xi.push_back(-4630); yi.push_back(800);
    //_addLineToBoundaries(xi,yi);
    // All pts (100,100) closer to (0,0)
    //timepass points
    LocalAvoidance::closeBoundaries.push_back(std::pair<Point2D<int>, Point2D<int> >(Point2D<int>(4200, -3350), Point2D<int>(-4200, -3350)));
    LocalAvoidance::closeBoundaries.push_back(std::pair<Point2D<int>, Point2D<int> >(Point2D<int>(-4200, -3350), Point2D<int>(-4200, 3350)));
    LocalAvoidance::closeBoundaries.push_back(std::pair<Point2D<int>, Point2D<int> >(Point2D<int>(-4200, 3350), Point2D<int>(4200, 3350)));
    LocalAvoidance::closeBoundaries.push_back(std::pair<Point2D<int>, Point2D<int> >(Point2D<int>(4200, 3350), Point2D<int>(4200, -3350)));
  }
  float area(int x1, int y1, int x2, int y2, int x3, int y3)
    {
      return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0);
    }
  bool LocalAvoidance::isInTriangle(int x1, int y1, int x2, int y2, int x3, int y3, int x, int y)
  {
    Point2D<float> p1(x1, y1);
    Point2D<float> p2(x2, y2);
    Point2D<float> p3(x3, y3);
    Point2D<float> p(x, y);
    float alpha = ((p2.y - p3.y)*(p.x - p3.x) + (p3.x - p2.x)*(p.y - p3.y)) /
        ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
    float beta = ((p3.y - p1.y)*(p.x - p3.x) + (p1.x - p3.x)*(p.y - p3.y)) /
       ((p2.y - p3.y)*(p1.x - p3.x) + (p3.x - p2.x)*(p1.y - p3.y));
    float gamma = 1.0f - alpha - beta;
    if(alpha > 0 && beta > 0 && gamma > 0)
      return true;
    return false;
  }
  bool LocalAvoidance::isPointInField(Point2D<int> pt)
  {
    assert(closeBoundaries.size() >= 4);
    Point2D<int> &pt1 = closeBoundaries[0].first;
    Point2D<int> &pt2 = closeBoundaries[1].first;
    Point2D<int> &pt3 = closeBoundaries[2].first;
    Point2D<int> &pt4 = closeBoundaries[3].first;
    if(isInTriangle(pt1.x, pt1.y, pt2.x, pt2.y, pt3.x, pt3.y, pt.x, pt.y) || isInTriangle(pt1.x, pt1.y, pt3.x, pt3.y, pt4.x, pt4.y, pt.x, pt.y))
      return true;
    return false;
  }

  LocalAvoidance::~LocalAvoidance() {}
  bool LocalAvoidance::plan(Vector2D<int> initial, Vector2D<float> velocity, Vector2D<int> final, std::vector<obstacle> obstacles, int current_id, bool teamBlue,
                            float curSlope, float finalSlope, float &t, float &r, CommType *comm, int clearance,bool avoid_obstacle)
  {
    /// Adding debug circle/line for destination point/angle
    Vector2D<int> boundaryPoint, leftBiasI, rightBiasI, leftBiasB, rightBiasB;
    theta = normalizeAngle(Vector2D<int>::angle(final, initial));
    phiStar = finalSlope;
    phi = curSlope;
    float directionalAngle ;
    directionalAngle = (cos(atan2(final.y - initial.y, final.x - initial.x) - curSlope) >= 0) ? curSlope : normalizeAngle(curSlope - PI);
    dist = Vector2D<int>::dist(final, initial);  // Distance of next waypoint from the bot
    alpha = normalizeAngle(theta - phiStar);
    beta = (fabs((float)alpha) < fabs((float)atan2(clearance, dist))) ? (alpha) : SGN(alpha) * atan2(clearance, dist);
    thetaD = normalizeAngle(theta + beta);
    float originalThetaD = thetaD;
    thetaDD = thetaD;
    float rayCastAngle = normalizeAngle((thetaD/*/2 + directionalAngle/2*/));
    //////////////// New Code Begin
    AngleChooser ac;
    for(int i = 0; i < obstacles.size(); i++)
    {
      float obsDistanceSq = Vector2D<int>::distSq(initial, Vector2D<int>(obstacles[i].x, obstacles[i].y));
      if(obsDistanceSq > COLLISION_DIST * COLLISION_DIST)
        continue;
      float safeRadius = SAFE_RADIUS;
      if(obsDistanceSq < SAFE_RADIUS * SAFE_RADIUS)
      {
        safeRadius = sqrt((float)obsDistanceSq) * 0.8;
      }
      else
      {
        safeRadius = SAFE_RADIUS;
      }
      float obsDistanceF = sqrt((float)obsDistanceSq);
      float botObsAngle = atan2(obstacles[i].y - initial.y, obstacles[i].x - initial.x);
      float tempAngle = 0;
      if(obsDistanceF > safeRadius)
        tempAngle = asin(safeRadius / obsDistanceF);
      else
        tempAngle = 0;
      float tempAngle1 = normalizeAngle(botObsAngle + tempAngle);
      float tempAngle2 = normalizeAngle(botObsAngle - tempAngle);
      ac.setInvalid(tempAngle1, tempAngle2);
      //printf("::::::::::::::::::::::::Invalidated: %lf to %lf\n", tempAngle1, tempAngle2);
    }
    /// adding boundary code
    int boundaryRadius = BOT_RADIUS * 2; // radius to check for boundary
    for(unsigned int i = 0; i < boundaries.size(); i++)
    {
      Point2D<int> &p1 = boundaries[i].first;
      Point2D<int> &p2 = boundaries[i].second;
      if(Intersection::lineInCircle(p1.x, p1.y, p2.x, p2.y, initial.x, initial.y, boundaryRadius))
      {
        //Invalidating angle using close bouundaries
        //sorry not using these
        Point2D<int> &p1_close = closeBoundaries[i].first;
        Point2D<int> &p2_close = closeBoundaries[i].second;
//        printf("<<< Intersecting with boundary!!\n");
        std::pair<Point2D<int>, Point2D<int> > tempPair = Intersection::getLineCirclePoints(p1.x, p1.y, p2.x, p2.y, initial.x, initial.y, boundaryRadius);
        
        float tempAngle1 = Vector2D<int>::angle(tempPair.first, initial);
        float tempAngle2 = Vector2D<int>::angle(tempPair.second, initial);
        /// expanding angle as suggested by akshay, soumyadeep.
        float tempAngle3 = atan(BOT_RADIUS/(float)boundaryRadius);
        ac.setInvalid(tempAngle1, tempAngle1+tempAngle3);
        ac.setInvalid(tempAngle1-tempAngle3, tempAngle1);
        ac.setInvalid(tempAngle2, tempAngle2+tempAngle3);
        ac.setInvalid(tempAngle2-tempAngle3, tempAngle2);
        ac.setInvalid(tempAngle1, tempAngle2);
      }
    }
    /// end boundary code
    //printf("<<<<<<<<<<< Number of obstacles = %u\n", obstacles.size());

    if(ac.isInvalid(rayCastAngle)&&avoid_obstacle)
    {
      
      //printf(">>>>>>>>>>>>>>Original Angle Loaded!!!\n");
      std::pair<double, double> p = ac.getLimits(rayCastAngle);
      float tempAngle1 = p.first;
      float tempAngle2 = p.second;
      if(AngleChooser::angleDifference(tempAngle1, thetaD) < AngleChooser::angleDifference(thetaD, tempAngle2))
        thetaDD = tempAngle1;
      else
        thetaDD = tempAngle2;
    }
    else
    {
      //printf(">>>>>>> Angle is FREE!! angle = %lf\n", rayCastAngle);
      thetaDD = thetaD;
    }
    /////////////// New Code End

    ///////////// New Code
    thetaD = thetaDD;
    //////////// End of
   
    boundaryPoint.x = initial.x + COLLISION_DIST * cos(thetaD);
    boundaryPoint.y = initial.y + COLLISION_DIST * sin(thetaD);
    for(unsigned int i=0; i<boundaries.size(); i++)
    {
      Point2D<int> &p1 = boundaries[i].first;
      Point2D<int> &p2 = boundaries[i].second;
//      comm->addLine(p1.x, p1.y, p2.x, p2.y);
    }
    float delta = normalizeAngle(thetaD - phi);
    r = (sin(delta) * sin(delta)) * SGN(tan(delta));
    t = (cos(delta) * cos(delta)) * SGN(cos(delta));
    return true;

  }


}
