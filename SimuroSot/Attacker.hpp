#pragma once
#include "skills.h"

namespace MyStrategy
{
  // Naive example for attacker
  void attacker(BeliefState *state,int botID)
  {
    print("Attacker\n");
	Vec2D fpoint(HALF_FIELD_MAXX,0);
	Vec2D ballPoint(state->ballPos.x, state->ballPos.y);
	// Go to ball if ball is far
	// Take ball to the goal
	if( Vec2D::distSq(state->homePos[botID],state->ballPos) < 2*BOT_BALL_THRESH)
		GoToPoint(botID,state,fpoint,PI/2,true,false);
	else 
		GoToPoint(botID,state,ballPoint,0,false, true);
  }
  
}