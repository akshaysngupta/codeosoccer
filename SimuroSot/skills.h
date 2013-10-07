// It is through the skill layer that the MyStrategy layer talks to the Simulator/Actual robots
#pragma once
#ifndef SKILLS_H
#define SKILLS_H


#include <cassert>
#include <string>
#include <vector>

#include "Utils/comdef.h"
#include "Utils/pathPlanners.h"
#include "Core/beliefState.h"
#include "winDebugger/Client.h"
#include "common/include/config.h"
#include "HAL/comm.h"
#include "common/include/geometry.hpp"

// Forward Declarations
namespace MyStrategy
{
  class LocalAvoidance;
  class BeliefState;
}

template <class T> class Vector2D;
typedef Vector2D<int> Vec2D;
// Use Vec2D to define a point with x and y as coordinate



namespace MyStrategy
{  
    /****************************************************List of Skills******************************************************/
    // Go to a point with obstacle avoidance.
    void GoToPoint(int botID,BeliefState *state,Vector2D<int> dpoint, float finalslope, bool increaseSpeed, bool shouldAlign);
    
    // Go to point without obstacle avoidance.
    void GoToPointStraight(int botID,BeliefState *state,Vector2D<int>dpoint,float finalslope, bool increaseSpeed, bool shouldAlign);
    
    // Go to ball: If shouldAlign is true, then bot will align with the line joining the ball and the goal else will go straightaway.
    void GoToBall(int botID,BeliefState *state,bool shouldAlign);
    
    // Angle in radian
    void TurnToAngle(int botID,BeliefState *state,float angle);
    
    // Control the bot directly.
    void Velocity(int botID,int vl,int vr);

    // Spin the bot. Positive angular speed for clockwise rotation
    void Spin(int botID,float angularSpeed);// spped in radians
    
    // Stop the bot.
    void Stop(int botID);

    /*
     * For those who want to know more may checkout the function definiton in skills.cpp
     * And you can also add new skills depending upon your need here.
    */
    /*********************************************************************************************************************/
    

    bool pointyInField(Vector2D<int> final);
    void maingotopoint(int botID,BeliefState *state, Vector2D<int> dpoint, float finalvel, float finalslope, float clearance = CLEARANCE_PATH_PLANNER,bool increaseSpeed=0,bool avoid_obstacle = true);
    static void clearDebugData()
    {
      #ifdef FIRASSL_COMM
        comm->clearDebugData();
      #endif
    }
} // namespace MyStrategy
#endif // SKILLS_H
