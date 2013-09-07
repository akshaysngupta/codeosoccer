#include "stdafx.h"
#include "intersection.hpp"

namespace Intersection
{
int findDet(Point2D<int> p1, Point2D<int> p2, Point2D<int> p3)
{
  return (p1.x*(p2.y-p3.y)+p2.x*(p3.y-p1.y)+p3.x*(p1.y-p2.y));
}
// Does not work!!!!
bool lineIntersection(Point2D<int> p1, Point2D<int> p2, Point2D<int> p3, Point2D<int> p4)
{
  if(findDet(p1,p2,p3)*findDet(p1,p2,p4)<0 && findDet(p3,p4,p1)*findDet(p3,p4,p2)<0)
    return true;
  else
    return false;
}
// Works
bool lineIntersection2(Point2D<int> p1, Point2D<int> p2, Point2D<int> p3, Point2D<int> p4)
{
// Store the values for fast access and easy
// equations-to-code conversion
  float x1 = p1.x, x2 = p2.x, x3 = p3.x, x4 = p4.x;
  float y1 = p1.y, y2 = p2.y, y3 = p3.y, y4 = p4.y;

  float d = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);
// If d is zero, there is no intersection
  if (d == 0) return false;
// Get the x and y
  float pre = (x1*y2 - y1*x2), post = (x3*y4 - y3*x4);
  float x = ( pre * (x3 - x4) - (x1 - x2) * post ) / d;
  float y = ( pre * (y3 - y4) - (y1 - y2) * post ) / d;

// Check if the x and y coordinates are within both lines
  if ( x < min(x1, x2) || x > max(x1, x2) ||
       x < min(x3, x4) || x > max(x3, x4) ) return false;
  if ( y < min(y1, y2) || y > max(y1, y2) ||
       y < min(y3, y4) || y > max(y3, y4) ) return false;

// Return the point of intersection
  
  return true;
}
bool pointInCircle(double x, double y, double cx, double cy, double r)
{
  return (Vector2D<double>::distSq(Vector2D<double>(x,y), Vector2D<double>(cx, cy)) < r*r);
}
bool lineInCircle(double ax, double ay, double bx, double by, double cx, double cy, double cr)
{
  double vx = bx - ax;
  double vy = by - ay;
  double xdiff = ax - cx;
  double ydiff = ay - cy;
  double a = pow(vx, 2) + pow(vy, 2);
  double b = 2 * ((vx * xdiff) + (vy * ydiff));
  double c = pow(xdiff, 2) + pow(ydiff, 2) - pow(cr, 2);
  double quad = pow(b, 2) - (4 * a * c);
  if (quad >= 0)
  {
    // An infinite collision is happening, but let's not stop here
    float quadsqrt=sqrt(quad);
    for(int i = -1; i <= 1; i += 2)
    {
      // Returns the two coordinates of the intersection points
      float t = (i * -b + quadsqrt) / (2 * a);
      float x = ax + (i * vx * t);
      float y = ay + (i * vy * t);
      // If one of them is in the boundaries of the segment, it collides
      if (x >= min(ax, bx) && x <= max(ax, bx) && y >= min(ay, by) && y <= max(ay, by)) return true;
      //return true;
    }
    if(pointInCircle(ax, ay, cx, cy, cr) && pointInCircle(bx, by, cx, cy, cr))
      return true;
  }
  return false;
}
std::pair<Point2D<int>, Point2D<int> > getLineCirclePoints(double ax, double ay, double bx, double by, double cx, double cy, double cr)
{
  double vx = bx - ax;
  double vy = by - ay;
  double xdiff = ax - cx;
  double ydiff = ay - cy;
  double a = pow(vx, 2) + pow(vy, 2);
  double b = 2 * ((vx * xdiff) + (vy * ydiff));
  double c = pow(xdiff, 2) + pow(ydiff, 2) - pow(cr, 2);
  double quad = pow(b, 2) - (4 * a * c);
  std::pair<Point2D<int>, Point2D<int> > ret;
  if (quad >= 0)
  {
    // An infinite collision is happening, but let's not stop here
    float quadsqrt=sqrt(quad);    
    for(int i = -1; i <= 1; i += 2)
    {
      // Returns the two coordinates of the intersection points
      float t = (i * -b + quadsqrt) / (2 * a);
      float x = ax + (i * vx * t);
      float y = ay + (i * vy * t);
      if(i == -1)
      {
        if (x >= min(ax, bx) && x <= max(ax, bx) && y >= min(ay, by) && y <= max(ay, by)) 
        {
          ret.first.x = x;
          ret.first.y = y;
        }
        else
        {
          if(Vector2D<int>::distSq(Vector2D<int>(ax, ay), Vector2D<int>(x, y)) < Vector2D<int>::distSq(Vector2D<int>(bx, by), Vector2D<int>(x, y)))
          {
            ret.first.x = ax;
            ret.first.y = ay;
          }
          else
          {
            ret.first.x = bx;
            ret.first.y = by;
          }
        }
      }
      else
      {
        if (x >= min(ax, bx) && x <= max(ax, bx) && y >= min(ay, by) && y <= max(ay, by)) 
        {
          ret.second.x = x;
          ret.second.y = y;
        }
        else
        {
          if(Vector2D<int>::distSq(Vector2D<int>(ax, ay), Vector2D<int>(x, y)) < Vector2D<int>::distSq(Vector2D<int>(bx, by), Vector2D<int>(x, y)))
          {
            ret.second.x = ax;
            ret.second.y = ay;
          }
          else
          {
            ret.second.x = bx;
            ret.second.y = by;
          }
        }
      }
      // If one of them is in the boundaries of the segment, it collides
      //if (x >= min(ax, bx) && x <= max(ax, bx) && y >= min(ay, by) && y <= max(ay, by)) return true;
    }
    return ret;
  }
  assert(0); // Collision points requested even though no collision btw line and circle
  return ret;
}
}
bool AngleInterval::operator<(const AngleInterval& ai) const
{
  if(start < ai.start)
    return true;
  if(start > ai.start)
    return false;
  return end < ai.end;
}

AngleChooser::AngleChooser()
{
  invalidSet.clear();
}

bool AngleChooser::insideInterval(double angle1, double angle2, double angle) //[angle1, angle2] in ordered direction (angle1->angle2 CCW)
{
  if(angle1 < angle2)
  {
    if(angle >= angle1 && angle2 >=  angle)
      return true;
    return false;
  }
  if(angle >= angle1 || angle <= angle2)
    return true;
  return false;
}
bool AngleChooser::isInvalid(double angle)
{
  for(std::set<AngleInterval>::iterator it = invalidSet.begin(); it!=invalidSet.end(); ++it)
  {
    const AngleInterval &ai = *it;
    if(insideInterval(ai.start, ai.end, angle))
      return true;
  }
  return false;
}
void AngleChooser::setInvalid(double angle1, double angle2)//invalidates SMALLER sector between 2 angles
{
  if(angle1 > angle2)
    swap(angle1, angle2);
  if(angle2 - angle1 > PI)
    swap(angle1, angle2);
  invalidSet.insert(AngleInterval(angle1, angle2));
  int size = invalidSet.size();
  if(size <= 1)
    return;
  std::set<AngleInterval>::iterator it = invalidSet.begin(), prevIt = it++;
  for(; it!=invalidSet.end(); prevIt=it, ++it)
  {
    const AngleInterval &ai1 = *prevIt;
    const AngleInterval &ai2 = *it;
    if(insideInterval(ai1.start, ai1.end, ai2.start))
    {
      //ai1.end = max(ai2.end, ai1.end);
      ai1.end = (angleDifference(ai1.start,ai1.end) > angleDifference(ai1.start,ai2.end))? ai1.end : ai2.end;
      invalidSet.erase(it);//erasing ai2
      it = prevIt;
    }
    else if(insideInterval(ai2.start, ai2.end, ai1.start))
    {
      //ai2.end = max(ai2.end, ai1.end);
      ai2.end = (angleDifference(ai2.start,ai1.end) > angleDifference(ai2.start,ai2.end))? ai1.end : ai2.end;
      invalidSet.erase(prevIt);//erasing ai1
      prevIt = it;
    }
  }
  size =invalidSet.size();
  if(size <= 1)
    return;
  it = invalidSet.begin();
  const AngleInterval &ai1 = *prevIt;
  const AngleInterval &ai2 = *it;
  if(insideInterval(ai1.start, ai1.end, ai2.start))
  {
    //ai1.end = max(ai2.end, ai1.end);
    ai1.end = (angleDifference(ai1.start,ai1.end) > angleDifference(ai1.start,ai2.end))? ai1.end : ai2.end;
    invalidSet.erase(it);//erasing ai2
    it = prevIt;
  }
  else if(insideInterval(ai2.start, ai2.end, ai1.start))
  {
    //ai2.end = max(ai2.end, ai1.end);
    ai2.end = (angleDifference(ai2.start,ai1.end) > angleDifference(ai2.start,ai2.end))? ai1.end : ai2.end;
    invalidSet.erase(prevIt);//erasing ai1
    prevIt = it;
  }
}
std::pair<double, double> AngleChooser::getLimits(double angle)
{
  if(!isInvalid(angle))
  {
    assert(0);//Error: limits requested for angle which is not invalid!!
  }
  for(std::set<AngleInterval>::iterator it = invalidSet.begin(); it!=invalidSet.end(); ++it)
  {
    const AngleInterval &ai = *it;
    if(insideInterval(ai.start, ai.end, angle))
      return pair<double,double>(ai.start, ai.end);
  }
  assert(0);//Error: shouldn't have reached this line of code.
  return std::pair<double,double>(10,10);
}
void AngleChooser :: printInvalid()
{
  for(std::set<AngleInterval>::iterator it = invalidSet.begin(); it!=invalidSet.end(); ++it)
  {
    printf("(%f , %f)\t",(*it).start,(*it).end);
  }
  printf("\n");
}