#pragma once
#include "skills.h"

namespace MyStrategy
{
  // Naive example for defender
  void defender(BeliefState *state,int botID)
  {
    //print("Defender\n");
	//It follows the ball
	Vec2D dpoint;
	dpoint.x = -HALF_FIELD_MAXX/2;
	dpoint.y = state->ballPos.y;
    GoToPoint(botID,state,dpoint,PI/2,false,false);
  }
}