#include "../Skills/skills.h"

namespace MyStrategy
{
  // Example
  void goalkeeper(BeliefState *state)
  {
    print("GoalKeeper\n");
    Vec2D dpoint(OUR_GOAL_X+DBOX_HEIGHT,state->ballPos.y);
    if(dpoint.y > OUR_GOAL_MAXY)
      dpoint.y = OUR_GOAL_MAXY;
    if(dpoint.y < OUR_GOAL_MINY)
      dpoint.y = OUR_GOAL_MINY;
    GoToPoint(0,state,dpoint,PI/2,false);
  }
}