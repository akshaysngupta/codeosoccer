// This file contains all the skillSet functons
#include "stdafx.h"
#include "skills.h"

using namespace HAL;
using namespace std;
namespace MyStrategy
{
  

  /***************************GoToPoint with pathplanner***************************************************/
  void GoToPoint(int botID,BeliefState *state,Vector2D<int> dpoint, float finalslope,bool increaseSpeed)
  {
    maingotopoint(botID,state,dpoint,0,finalslope,0,increaseSpeed,true);
  }
  void GoToPointStraight(int botID,BeliefState *state,Vector2D<int> dpoint, float finalslope,bool increaseSpeed)
  {
    maingotopoint(botID,state,dpoint,0,finalslope,0,increaseSpeed,false);
  }
  void maingotopoint(int botID,BeliefState *state, Vector2D<int> dpoint, float finalvel, float finalslope, float clearance,bool increaseSpeed,bool avoid_obstacle)
  {
    int prevVel = 0;
    static LocalAvoidance*    pathPlanner;
    pathPlanner = new LocalAvoidance();
    Comm* comm  = Comm::getInstance();
    std::vector<obstacle> obs;
    obstacle obsTemp;
    for (int i = 0; i < HomeTeam::SIZE ; ++i)
    {
      /// Adding Condition to REMOVE all obstacles that are sufficiently CLOSE to BALL
      if( i != botID && Vector2D<int>::distSq(state->homePos[botID], state->homePos[i]) < COLLISION_DIST * COLLISION_DIST && Vector2D<int>::distSq(state->ballPos, state->homePos[i]) > BOT_RADIUS * BOT_RADIUS * 9)
      {
        /// If destination is close to bot, don't add it as obstacle
        if(Vector2D<int>::distSq(dpoint, state->homePos[i]) > BOT_RADIUS * BOT_RADIUS * 1)
        {
          obsTemp.x = state->homePos[i].x;
          obsTemp.y = state->homePos[i].y;
          obsTemp.radius =  BOT_RADIUS;
          obs.push_back(obsTemp);
        }
      }
    }

    for (int i = 0; i < AwayTeam::SIZE; ++i)
    {
      if(Vector2D<int>::distSq(state->homePos[botID], state->awayPos[i]) < COLLISION_DIST * COLLISION_DIST && Vector2D<int>::distSq(state->ballPos, state->awayPos[i]) > BOT_RADIUS * BOT_RADIUS * 9)
      {
        /// If destination is close to bot, don't add it as obstacle
        if(Vector2D<int>::distSq(dpoint, state->homePos[i]) > BOT_RADIUS * BOT_RADIUS * 1)
        {
          obsTemp.x = state->awayPos[i].x;
          obsTemp.y = state->awayPos[i].y;
          obsTemp.radius =  BOT_RADIUS;
          obs.push_back(obsTemp);
        }
      }
    }
    Vector2D<int> point, nextWP, nextNWP;
    float r = 0, t = 0, dist = 0;
    dist = Vector2D<int>::dist(dpoint, state->homePos[botID]);  // Distance of next waypoint from the bot
    if(dist < BOT_POINT_THRESH )
    {
      float angle = fabs((float)firaNormalizeAngle(state->homeAngle[botID] - finalslope));
      if(angle > DRIBBLER_BALL_ANGLE_RANGE)
      {
        TurnToAngle(botID,state,finalslope);
        return;
      }
     
      comm->sendCommand(botID, finalvel, finalvel);
      return;
      // This part of the function is just for safety.
      // The tactic should actually prevent this call.
      // The bot should be aligned properly before this condition is reached.
    }

    pathPlanner->plan(state->homePos[botID],
                      state->homeVel[botID],
                      dpoint,
                      obs,
                      botID,
                      true,
                      state->homeAngle[botID],
                      finalslope,
                      t,
                      r,
                      comm,
                      clearance,
                      avoid_obstacle);

    float fTheta = asin(sqrt(fabs((float)r)));
    fTheta = 1 - fTheta/(PI/2);
    fTheta = pow(fTheta,2.2) ;
    float fDistance = (dist > BOT_POINT_THRESH*3) ? 1 : dist / ((float) BOT_POINT_THRESH *3);
    float fTot = fDistance * fTheta;
    fTot = 0.2 + fTot*(1-0.2);

    float profileFactor = MAX_BOT_SPEED*fTot;
    float max_bot_linear_vel_change = 10;

    if(increaseSpeed==1&&r<0.11)
    if(fabs((float)r)<0.11)
      profileFactor*=2;
    {
      if(fabs((float)profileFactor-prevVel)>max_bot_linear_vel_change)
      {
        if(profileFactor>prevVel)profileFactor=prevVel+max_bot_linear_vel_change;
        else profileFactor=prevVel-max_bot_linear_vel_change;
      }
      prevVel=profileFactor;
    }
    if(profileFactor>1.5*MAX_BOT_SPEED)
      profileFactor = 1.5*MAX_BOT_SPEED;
    else if(profileFactor <-1.5*MAX_BOT_SPEED)
      profileFactor = -1.5*MAX_BOT_SPEED;
    prevVel=profileFactor;
    r *= profileFactor;
    t *= profileFactor;
    #if FIRA_COMM || FIRASSL_COMM
    comm->sendCommand(botID, (t - r), (t + r));
    #else
    comm->sendCommand(botID, (t - r), (t + r));
    #endif
  }
  /*******************************************************************************/



  /************************************TurnToAngle*********************************/
  void TurnToAngle(int botID,BeliefState *state,float angle)
  {
    Comm *comm = Comm::getInstance();
    float vl,vr;
    float finalSlope = angle;
    float turnAngleLeft = normalizeAngle(finalSlope - state->homeAngle[botID]); // Angle left to turn
    
    if(turnAngleLeft>PI/2||turnAngleLeft<-PI/2)
    {
      if(turnAngleLeft>PI/2)
        turnAngleLeft=turnAngleLeft-PI;
      else
        turnAngleLeft=turnAngleLeft+PI;
    }

    float factor = (turnAngleLeft+(turnAngleLeft))/(PI/2);
    vr = -0.4*MAX_BOT_OMEGA*(factor)/(PI/2);
    
    if(fabs((float)turnAngleLeft) > DRIBBLER_BALL_ANGLE_RANGE/2)
    {
      #if FIRA_COMM || FIRASSL_COMM 
      comm->sendCommand(botID, vl, vr);        
      #else
      comm->sendCommand(botID, vr, vl); 
      #endif
    }
    else 
    {
      comm->sendCommand(botID, 0, 0);
    }
  }
  /*********************************************************************************/

  /*************************************GoToBall************************************/
  void GoToBall(int botID,BeliefState *state,bool align)
  {
    Comm *comm = Comm::getInstance();
		Vector2D<int> ballInitialpos,ballFinalpos;
		ballInitialpos = state->ballPos;
		ballFinalpos.x = state->ballPos.x+(0.7*state->ballVel.x);
		ballFinalpos.y = state->ballPos.y+(0.7*state->ballVel.y);
		
		float botballdist = Vector2D<int>::dist(ballInitialpos,state->homePos[botID]);
		float botballVeldist = Vector2D<int>::dist(ballFinalpos,state->homePos[botID]);
		
		float theta=Vector2D<int>::angle(ballInitialpos,ballFinalpos);
		
		if (align==true)
		{
			if((Vector2D<int>::dist(ballInitialpos,state->homePos[botID]))>=BOT_BALL_THRESH)
			{
					if(fabs((float)tan(Vector2D<int>::angle(ballInitialpos,ballFinalpos))-tan(Vector2D<int>::angle(state->homePos[botID],ballFinalpos)))<1)
            maingotopoint(botID,state,ballInitialpos,0,theta,50.0);
          else
            maingotopoint(botID,state,ballFinalpos,0,theta,50.0);
			}
			else 
			{
          comm->sendCommand(botID,0,0);
			}
		}
		else
		{
			if((Vector2D<int>::dist(ballInitialpos,state->homePos[botID]))>=BOT_BALL_THRESH)
			{
					maingotopoint(botID,state,ballInitialpos,0,theta,00.0);
      }
			else 
			{
				comm->sendCommand(botID,0,0);
			}
		}
  }
  /**********************************************************************************************/

  void Stop(int botID)
  {
    Comm *comm = Comm::getInstance();
    comm->sendCommand(botID,0,0);
  } // stop

  void Velocity(int botID,int vl,int vr)
  {
    Comm *comm = Comm::getInstance();
    comm->sendCommand(botID,vl,vr);
  }

  void Spin(int botID,float angularSpeed)// speed in radians
  {
    Comm *comm = Comm::getInstance();
    float omega,vl,vr ; 
    vr = (angularSpeed * (MAX_BOT_SPEED))/MAX_BOT_OMEGA ;
    vl = -vr;
    printf("%f %f\n", vl, vr);
    
    #if FIRA_COMM || FIRASSL_COMM
      comm->sendCommand(botID, vl, vr);        
    #else
      comm->sendCommand(botID, vr, vl);
    #endif
  }

  bool pointyInField(Vector2D<int> final)
  {
    if(final.y < -HALF_FIELD_MAXY + BALL_AT_CORNER_THRESH || final.y > HALF_FIELD_MAXY - BALL_AT_CORNER_THRESH)
      return false;
    else return true;
  }

} // namespace MyStrategy
