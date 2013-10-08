#pragma once
#ifndef BELIEF_STATE_H
#define BELIEF_STATE_H

#include "..\common\include\geometry.hpp"
#include "..\Utils\comdef.h"

static char debug[500];
#define print(...) {sprintf(debug,__VA_ARGS__); Client::debugClient->SendMessages(debug);}

// Forward Declarations
namespace HAL
{
  class VisionInfo;
  class RefBoxCmd;
}

namespace MyStrategy
{
  class VisionThread;
}

namespace Util
{
  class CS;
}

namespace MyStrategy
{
  class BeliefState
  {
  private:
    int              prevFrameNum;
    // stateSh is the shared belief state. It will be shared by the home robots.
  public:
    BeliefState();

    ~BeliefState();

    // It tries to update the belief state. If it does not change, the function returns false else true
	
    bool      update();
    int       currFrameNum;
	void print_values();

   //*********************------- Set of Predicates -------***********************//
   //*****************************************************************************// 
    int ourBotNearestToBall;
    int oppBotNearestToBall;
    
    
    
    // Predicates for ball possession
    bool pr_oppBall;
    bool pr_ourBall;
    bool pr_looseBall;

    // Predicates for ball position in field
    bool pr_ball_in_opp_dbox;
    bool pr_ballInOppCorner;
    bool pr_ballOppSide;
    bool pr_ballMidField;
    bool pr_ballOurSide;
    bool pr_balInOurCorner;
    bool pr_ball_in_our_dbox;   

 
    // Predicates that take an input parameter
    bool pr_nOpponentsOurSide(int n);
    bool pr_ballXGt(int x);
    bool pr_ballXLt(int x);
    bool pr_ballAbsYGt(int y);
    bool pr_ballAbsYLt(int y);
    bool pr_ballInSideStrip;
    bool pr_ballInFrontStrip;
    /************************************************************************************/




    /************************************************************************************/
    Point2D<int>    homePos[HomeTeam::SIZE];
    Point2D<int>    fieldCentre;
    Point2D<int>    home_goalpoints[5];
    Point2D<int>    opp_goalpoints[5];
    Vector2D<float> homeVel[HomeTeam::SIZE];
    Vector2D<float> homeAcc[HomeTeam::SIZE];
    float           homeAngle[HomeTeam::SIZE];
    float           homeOmega[HomeTeam::SIZE];
    float           homeAngAcc[HomeTeam::SIZE];
    
    Point2D<int>    awayPos[HomeTeam::SIZE];
    Vector2D<float> awayVel[HomeTeam::SIZE];
    Vector2D<float> awayAcc[HomeTeam::SIZE];
    float           awayAngle[HomeTeam::SIZE];
    float           awayOmega[HomeTeam::SIZE];
    float           awayAngAcc[HomeTeam::SIZE];
    
    Point2D<int>    ballPos;
    Vector2D<float> ballVel;
    Vector2D<float> ballAcc;
    /**********************************************************************************/


    // Prototypes for functions that compute the values of the predicates
    void computeBallInDBox();
    void computeOffense();
    void computeDefense();
    
    void computeBallLocation();
    void computeBallOppSide();
    void computeBallOurSide();
    void computeBallMidfield();

    void computeBallInOurCorner();
    void computeBallInOppCorner();

    void computeOurKickOff();
    void computeOppKickOff();

    void computeOurFreeKick();
    void computeOppFreeKick();

    void computeOurPenalty();
    void computeOppPenalty();
    
    void computeBallInStrips();
    
    // Functional Predicates
    int ourBotNearestToPoint(const int x, const int y);
    int oppBotNearestToPoint(const int x, const int y);
  };   // class BeliefState
} // namespace MyStrategy

#endif // BELIEF_STATE_H