#pragma once
#ifndef PATH_PLANNER_H
#define PATH_PLANNER_H

#include <vector>
#include <list>
#include "..\common\include\geometry.hpp"
#include "..\common\include\config.h"
#include "..\HAL\comm.h"
#define MAX_OBSTACLES (HomeTeam::SIZE + AwayTeam::SIZE + 1)

// Forward Declarations
namespace MyStrategy
{
  template <class T> class KDTree;
}
/*TODO: Find Out if forward declaration of FIRAComm is actually needed or not. 
 * Technically, pathPlanning should be unaware of communication implementations.
 * Also see if header includes and CommType is really required orf not (should not be required waise)
 * <Arpit>: comm is basically used for adding debugging figures from plan() of LocalAvoidance (eg. comm->addLine, addCircle). 
 * So we only really need the base class here.
 */ 

typedef Comm CommType;

namespace MyStrategy
{
  typedef struct
  {
    float x, y;
    float radius;
  } obstacle;

  class LocalAvoidance
  {
  private:
    float phiStar;
    float theta;
    float phi;
    float dist;
    float alpha, beta;
    float thetaD, thetaDD, delta;
    float r, t;
    float profileFactor, velMod; 
    void _addLineToBoundaries(std::vector<int> xi, std::vector<int> yi);
    inline void addLineToBoundaries(int x1, int y1, int x2, int y2)
    {
      boundaries.push_back(std::pair<Point2D<int>, Point2D<int> >(Point2D<int>(x1, y1), Point2D<int>(x2, y2)));
    }
    // Field Boundary lines    
  public:
    static std::vector<std::pair<Point2D<int>, Point2D<int> > > boundaries;
    static std::vector<std::pair<Point2D<int>, Point2D<int> > > closeBoundaries; // inside main boundaries, for padded checking
    static bool isInTriangle(int ax, int ay, int bx, int by, int dx, int dy, int x, int y);
    static bool isPointInField(Point2D<int> pt); // Checks if point Strictly lies on field (within bondaries defined  here) (uses closeBoundaries)
  public:
    LocalAvoidance();
    ~LocalAvoidance();
    bool plan(Vector2D<int> initial, Vector2D<float> velocity, Vector2D<int> final, std::vector<obstacle> obstacles, int current_id, bool teamBlue, float curSlope, float finalSlope, float &t, float &r, CommType *comm = NULL, int clearance = CLEARANCE_PATH_PLANNER,bool avoid_obstacle = true);
  private:
    bool rayCasting(Vector2D<int> p1, Vector2D<int> p2, obstacle *obs, int obsRadius);
  }; //Class LocalAvoidance
 
}

#endif // PATH_PLANNER_H
