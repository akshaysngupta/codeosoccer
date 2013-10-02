#pragma once 
#include "stdafx.h"
#include <ctime>
#include "beliefState.h"
#include "../common/include/config.h"
#include "../common/include/thread.h"
#include "../common//include/cs.hpp"
#include "../common/include/config.h"
#include "../skills.h"
#include <Windows.h>

using namespace std;
using namespace Util;
using namespace HAL;
using namespace Simulator;

namespace MyStrategy
{

  BeliefState::BeliefState() :
    prevFrameNum(-1),
    currFrameNum(-1)
  {
    for(int botID = 0; botID < HomeTeam::SIZE; botID++)
    {
      homeAngle[botID] = 0;
    }

    for(int botID = 0; botID < AwayTeam::SIZE; botID++)
    {
      awayAngle[botID] = 0;
    }
  }

  BeliefState::~BeliefState()
  {

  }

  bool BeliefState::update()
  {
	print_values();
    computeBallLocation();
    computeBallInDBox();
    computeBallOurSide();
    computeBallOppSide();
    computeBallInStrips();
    return true;
  } // update
  /* Function definitions for all the predicates...
     * */

  void BeliefState::computeBallLocation()
  {
    int homeNearbotID = -1, awayNearbotID = -1;
    float minDist, distSq, closestBot ;
    minDist = 1e20;
    for(int i = 0; i < HomeTeam::SIZE ; i++)
    {
      float th = Vector2D<int>::angle(ballPos, homePos[i]);
      float theta = normalizeAngle(th - homeAngle[i]);
      distSq =  Vector2D<int>::distSq(ballPos , homePos[i]);

      if(distSq < minDist)
      {
        minDist = distSq;
        closestBot = i;
      }
      this->ourBotNearestToBall = closestBot;
      if(distSq < (BOT_BALL_THRESH * BOT_BALL_THRESH) && fabs((float)theta) < DRIBBLER_BALL_ANGLE_RANGE)
      {
        homeNearbotID = i;
        break ;
      }
    }
    minDist = 1e20;
    for(int i = 0; i < AwayTeam::SIZE ; i++)
    {
      float theta = normalizeAngle(Vector2D<int>::angle(ballPos, homePos[i]) - awayAngle[i]);
      distSq = Vector2D<int>::distSq(ballPos , awayPos[i]);
      if(distSq < minDist)
      {
        minDist = distSq;
        closestBot = i;
      }
      this->oppBotNearestToBall = closestBot;
      if(distSq < (BOT_BALL_THRESH * BOT_BALL_THRESH) && fabs((float)theta) < DRIBBLER_BALL_ANGLE_RANGE)
      {
        awayNearbotID = i;
        break ;
      }
    }
    //printf("OurBotnearest, OppBotNearest : %d %d\n",ourBotNearestToBall,oppBotNearestToBall);
    if(awayNearbotID == -1 && homeNearbotID == -1)
    {
      pr_looseBall = true;
      pr_oppBall = false;
      pr_ourBall = false;

    }
    else if(awayNearbotID == -1 && homeNearbotID != -1)
    {
      pr_looseBall  =  false;
      pr_oppBall    =  false;
      pr_ourBall    =  true;
    }
    else if(awayNearbotID != -1 && homeNearbotID == -1)
    {
      pr_looseBall  =  false;
      pr_oppBall    =  true;
      pr_ourBall    =  false;
    }
    else if(awayNearbotID != -1 && homeNearbotID != -1)
    {
      pr_looseBall  =  false;
      pr_oppBall    =  true;
      pr_ourBall    =  false;

      //TODO: Incorporate bot orientation to decide if homeBot is pointing in the right direction
    }

  }

  void BeliefState::computeBallInDBox()
  {
    pr_ball_in_opp_dbox = false;
    pr_ball_in_our_dbox = false;

    if(ballPos.x < (-HALF_FIELD_MAXX + DBOX_WIDTH) &&
        ballPos.y < OUR_GOAL_MAXY &&
        ballPos.y > OUR_GOAL_MINY)
      pr_ball_in_our_dbox = true;
    else if(ballPos.x > (HALF_FIELD_MAXX - DBOX_WIDTH) &&
            ballPos.y < OUR_GOAL_MAXY &&
            ballPos.y > OUR_GOAL_MINY)
      pr_ball_in_opp_dbox = true;
  }

  //The following three assume that 0,0 is at center of frame (for checking ball parameters)

  void BeliefState::computeBallOppSide()
  {
    if((ballPos.x) > 0)
    {
      //MID_FIELD_THRESH is the distance from midfield line into which when ball goes ball's side changes
      if((ballPos.x > fieldCentre.x + MID_FIELD_THRESH) && (pr_ourBall == true))
        pr_ballOppSide = true ;
      //check ball VELOCITY
      else if((ballVel.x) * (opp_goalpoints[2].x - fieldCentre.x) > 0) //opp_goalpoints[2] is mid point of goal
        pr_ballOppSide = true ;
      else
        pr_ballOppSide = false;
    }
  }

  void BeliefState::computeBallOurSide()
  {
    if(ballPos.x < fieldCentre.x)
    {
      if((ballPos.x < fieldCentre.x - MID_FIELD_THRESH) && (pr_oppBall == true)) //MID_FIELD_THRESH is the distance from midfield line into which when ball goes ball's side changes
        pr_ballOurSide = true ;
      else if((ballVel.x) * (opp_goalpoints[2].x - fieldCentre.x) < 0)  //opp_goalpoints[2] is mid point of goal
        pr_ballOurSide = true ;
    }
  }

  void BeliefState::computeBallMidfield()
  {

    if((pr_ballOppSide != true) && (pr_ballOurSide != true))
      pr_ballMidField = true ;
  }

  bool BeliefState::pr_nOpponentsOurSide(int n)
  {
    int ctr = 0;
    for(int i = 0; i < AwayTeam::SIZE ; i++)
    {
      if(awayPos[i].x < fieldCentre.x)
        ctr ++ ;
    }
    if(ctr == n)
      return true ;
    else
      return false ;
  }

  bool BeliefState::pr_ballXGt(int x)
  {
    if(ballPos.x > x)
      return true ;
    else
      return false ;
  }

  bool BeliefState::pr_ballXLt(int x)
  {
    if(ballPos.x  < x)
      return true ;
    else
      return false ;
  }

  bool BeliefState::pr_ballAbsYGt(int y)
  {
    int absy = ballPos.y > 0 ? ballPos.y : (-(ballPos.y)) ;
    if(absy > y)
      return true ;
    else
      return false ;
  }

  bool BeliefState::pr_ballAbsYLt(int y)
  {
    int absy = ballPos.y > 0 ? ballPos.y : (-(ballPos.y)) ;
    if(absy < y)
      return true ;
    else
      return false ;
  }

  void BeliefState::computeBallInStrips()
  {
    pr_ballInFrontStrip = (ballPos.x > HALF_FIELD_MAXX - STRIP_WIDTH_X || ballPos.x < -HALF_FIELD_MAXX + STRIP_WIDTH_X);
    pr_ballInSideStrip  = (ballPos.y > HALF_FIELD_MAXY - STRIP_WIDTH_Y || ballPos.y < -HALF_FIELD_MAXY + STRIP_WIDTH_Y);
  }

 
  int BeliefState::ourBotNearestToPoint(const int x, const int y) {
    float minDis  = 1e20;
    int   botId   = -1;
    for(int i = 0; i < HomeTeam::SIZE; i++) {
      float dnew = Vector2D<int>::dist(homePos[i], x, y);
      if(dnew < minDis) {
        minDis = dnew;
        botId  = i;
      }
    }
    return botId;
  }
  
  int BeliefState::oppBotNearestToPoint(const int x, const int y) {
    float minDis  = 1e20;
    int   botId   = -1;
    for(int i = 0; i < AwayTeam::SIZE; i++) {
      float dnew = Vector2D<int>::dist(awayPos[i], x, y);
      if(dnew < minDis) {
        minDis = dnew;
        botId  = i;
      }
    }
    return botId;
  }

  void BeliefState::print_values()
  {
	  print("@@@%d\t%d*%d\t%d*%d\t%d*%d\t%d*%d\t%d@%.2f\t%.2f*%.2f\t%.2f*%.2f\t%.2f*%.2f\t%.2f*%.2f\t%.2f@%.2f*%.2f*%.2f*%.2f@",ballPos.x,ballPos.y,homePos[0].x,homePos[0].y,homePos[1].x,homePos[1].y,homePos[2].x,homePos[2].y,homePos[3].x,homePos[3].y
		  ,ballVel.x,ballVel.y,homeVel[0].x,homeVel[0].y,homeVel[1].x,homeVel[1].y,homeVel[2].x,homeVel[2].y,homeVel[3].x,homeVel[3].y
		  ,(180/PI)*homeAngle[0],(180/PI)*homeAngle[1],(180/PI)*homeAngle[2],(180/PI)*homeAngle[3] );
  }
  
} // namespace MyStrategy