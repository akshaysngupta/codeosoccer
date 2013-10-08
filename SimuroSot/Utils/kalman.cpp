#pragma once
#include "stdafx.h"
#include "comdef.h"
#include "kalman.h"
#include "../common/include/cs.hpp"
#include "../Core/beliefState.h"
#include "../common/include/config.h"
#include "../common/include/timer.h"
#include < time.h >
#include "../winDebugger/Client.h"
//#include <stdafx.h>
#include "../Strategy.h"
#include <limits>
#define DEBUG_KALMAN 1
const int botcenterbluecenterdist =75; //Ankit: for botCentreTransform
inline void linearTransform(int &x, int &y, float &newangle,bool invert_field)
{
  int tempx, tempy;
  tempx = SGN(-invert_field+0.5)*(5000-x);
  tempy = SGN(-invert_field+0.5)*(4200-y);
  x = tempx;
  y = tempy;
  newangle = !invert_field?normalizeAngle(newangle+PI):newangle;
  //y=tempy-800;
}
inline void linearTransform_new(int &x, int &y, float &newangle,bool invert_field)
{
  int tempx, tempy;
  tempx = SGN(-!invert_field+0.5)*(5000-x);
  tempy = SGN(-!invert_field+0.5)*(4200-y);
  x = tempx;
  y = tempy;
  newangle = invert_field?normalizeAngle(newangle+PI):newangle;
  //y=tempy-800;
}

bool isIndeterminate(const float pV)
{
    return (pV != pV);
} 

bool isInfinite(const float pV)
{
    return (fabs((float)pV) == std::numeric_limits<float>::infinity());
}
void checkValid(float &x, float resetVal) {
  if(isIndeterminate(x) || isInfinite(x)) {
    x = 2*HALF_FIELD_MAXX;
  }
}
void checkValidX(float &x, float &slope, float resetVal) {
  if(x>=-2*HALF_FIELD_MAXX && x<=2*HALF_FIELD_MAXX)return;
  x = resetVal;
  slope = 0;
}
void checkValidY(float &y, float &slope, float resetVal) {
  if(y>=-2*HALF_FIELD_MAXY && y<=2*HALF_FIELD_MAXY)return;
  y = resetVal;
  slope = 0;
}
void checkValidA(float &a, float &slope, float resetVal) {
  if(a>=-2*PI && a<=2*PI)return;
  a = resetVal;
  slope = 0;
}
namespace MyStrategy
{
  Kalman::Kalman()
  {
    kalmanlog = fopen("kalman.log", "w");
    t.start();
    mutex = new Util::CS();
    for(int id = 0; id < HomeTeam::SIZE; ++id)
    {
      homePose[id].x     = 2*HALF_FIELD_MAXX + BOT_RADIUS*4;
      homePose[id].y     = 2*HALF_FIELD_MAXY + BOT_RADIUS*4;
      homeVelocity[id].x = 0;
      homeVelocity[id].y = 0;
      homePosK[id].x     = 1;
      homePosK[id].y     = 1;
      homeAngle[id]      = 0;
      homeOmega[id]      = 0;
      homeAngleK[id]     = 1;
    }
    for(int id = 0; id < AwayTeam::SIZE; id++)
    {
      awayPose[id].x     = 2*HALF_FIELD_MAXX + BOT_RADIUS*4;
      awayPose[id].y     = 2*HALF_FIELD_MAXY + BOT_RADIUS*4;
      awayVelocity[id].x = 0;
      awayVelocity[id].y = 0;
      awayPosK[id].x     = 1;
      awayPosK[id].y     = 1;
      awayAngle[id]      = 0;
      awayOmega[id]      = 0;
      awayAngleK[id]     = 1;
    }
    ballPose.x     = 0;
    ballPose.y     = 0;
    ballVelocity.x = 0;
    ballVelocity.y = 0;
    ballPosK.x     = 1;
    ballPosK.y     = 1;
  }

  Kalman::~Kalman()
  {
    fclose(kalmanlog);
  }
  static float nearestAngle(float angle, float reference)
  {
    float del = fabs((float)angle-reference);
    float res = angle;
    if(del > fabs((float)angle-reference+2*PI))
    {
      del = fabs((float)angle-reference+2*PI);
      res = angle+2*PI;
    }
    if(del > fabs((float)angle-reference-2*PI))
    {
      res = angle-2*PI;
    }
    return res;
  }
  void Kalman::addInfo(Environment* env,bool invert_field)
  {
    static double minDelTime = 999999;
    //printf("addinfo......................\n");
    static double lastTime = 0;
    mutex->enter();
    //double timeCapture           = detection.t_capture();
    int ballsNum                 = 1;
    int blueNum                  =5;
    int yellowNum                = 5;


	
    //fprintf(kalmanlog, ">>>>>>>>>>>>>>>>Time: %f\n", timeCapture - lastTime);
    //double timeCapture      = detection.t_capture()/1000.0; //This is only valid if both the other side also uses the same reference time
    // neglecting the time for network delay. and wait time in race conditions 
    struct timeval tv;
	assert(Util::Timer::gettimeofday(&tv, NULL) == 0);
    double timeCapture      = tv.tv_sec+tv.tv_usec/1000000.0;
    
//    fprintf(kalmanlog, ">>>>>>>>>>>>>TimeDiff: %lld\n", microseconds);
    lastTime = timeCapture;
//    printf(">>>>>>>>>>>>>>> new iter in kalman.. <<<<<\n");
    {
      // Blue robot info
      
      for (int i = 0; i < blueNum; ++i)
      {
        int                id    = i;
        
		int newx = env->home[i].pos.x*100 - CENTER_X;
        int newy =  env->home[i].pos.y*100 - CENTER_Y;
		float newangle = env->home[i].rotation*PI/180;       
     linearTransform(newx, newy, newangle,invert_field);
        double           delTime =1/60.0;// timeCapture - homeLastUpdateTime[id];

        static float lastnx = 0;
        homePosSigmaSqK[id].x    = homePosSigmaSqK[id].x * ( 1 - homePosK[id].x) + SIGMA_SQ_NOISE_POS * delTime;
      //  assert(homePosSigmaSqK[id].x >= 0);
        homePosK[id].x           = homePosSigmaSqK[id].x / (homePosSigmaSqK[id].x + SIGMA_SQ_OBSVN_POS);
        float  predictedPoseX    = homePose[id].x + homeVelocity[id].x * (delTime);
        float  lastPoseX         = homePose[id].x;
        homePose[id].x            = predictedPoseX + homePosK[id].x * (newx - predictedPoseX);
        float lastVelocityx      = homeVelocity[id].x;
        homeVelocity[id].x       = (homePose[id].x - lastPoseX) / delTime;
        homeAcc[id].x            = (homeVelocity[id].x - lastVelocityx) / delTime;
        if(delTime < minDelTime)
          minDelTime = delTime;
//        assert(delTime > 0.00000001);
        //printf("minDelTime = %lf\n", minDelTime);
        homePosSigmaSqK[id].y    = homePosSigmaSqK[id].y * ( 1 - homePosK[id].y) + SIGMA_SQ_NOISE_POS * delTime;
      //  assert(homePosSigmaSqK[id].y >= 0);
        homePosK[id].y           = homePosSigmaSqK[id].y / (homePosSigmaSqK[id].y + SIGMA_SQ_OBSVN_POS);
        float  predictedPoseY    = homePose[id].y + homeVelocity[id].y * (delTime);
        float  lastPoseY         = homePose[id].y;
        homePose[id].y           = predictedPoseY + homePosK[id].y * (newy - predictedPoseY);
        float lastVelocityy      = homeVelocity[id].y;
        homeVelocity[id].y       = (homePose[id].y - lastPoseY) / delTime;
        homeAcc[id].y            = (homeVelocity[id].y - lastVelocityy) / delTime;
                                
        homeAngleSigmaSqK[id]    = homeAngleSigmaSqK[id] * ( 1 - homeAngleK[id]) + SIGMA_SQ_NOISE_ANGLE * delTime;
        homeAngleK[id]           = homeAngleSigmaSqK[id] / (homeAngleSigmaSqK[id] + SIGMA_SQ_OBSVN_ANGLE);
        float  predictedAngle    = homeAngle[id] + homeOmega[id] * (delTime);
        float  lastAngle         = homeAngle[id];
        homeAngle[id]            = predictedAngle + homeAngleK[id] * (nearestAngle(newangle, homeAngle[id]) - predictedAngle);
        float lastAngularV       = homeOmega[id];
        homeOmega[id]            = (homeAngle[id] - lastAngle) / delTime;
        homeAngularAcc[id]       = (homeOmega[id] - lastAngularV) / delTime;
        if(homeAngle[id] > PI)  homeAngle[id] -= 2*PI;
        else if(homeAngle[id] <=-PI) homeAngle[id] += 2*PI;
        homeLastUpdateTime[id]   = timeCapture;
        
//        if(id == 1)
//          printf("%f %f %f %f\n", homePose[1].x, homePosK[1].x, homePosSigmaSqK[1].x, homeVelocity[1].x);
        checkValidX(homePose[id].x, homeVelocity[id].x, newx);
        checkValidY(homePose[id].y, homeVelocity[id].y, newy);
        checkValidA(homeAngle[id], homeOmega[id], newangle);
        //printf("homePos from kalman: id: %d, xy: %.0f %.0f\n", id, homePose[id].x, homePose[id].y);
      }
      // Yellow robot info
      for (int i = 0; i < yellowNum; ++i)
      {
        
      int                id    = i;
        
		int newx = env->opponent[i].pos.x*100 - CENTER_X;
        int newy =  env->opponent[i].pos.y*100 - CENTER_Y;
		float newangle = env->opponent[i].rotation*PI/180;  
        double           delTime =1/60.0;// timeCapture - awayLastUpdateTime[id];
		if(invert_field)
			linearTransform_new(newx, newy, newangle,invert_field);
		else
			linearTransform(newx, newy, newangle,invert_field);
		if(id == 1 && invert_field)
		{
			newx = - newx;
			newy = -newy;
		}
        awayPosSigmaSqK[id].x    = awayPosSigmaSqK[id].x * ( 1 - awayPosK[id].x) + SIGMA_SQ_NOISE_POS * delTime;
        awayPosK[id].x           = awayPosSigmaSqK[id].x / (awayPosSigmaSqK[id].x + SIGMA_SQ_OBSVN_POS);
        float  predictedPoseX    = awayPose[id].x + awayVelocity[id].x * (delTime);
        float  lastPoseX         = awayPose[id].x;
        awayPose[id].x           = /*newx;*/predictedPoseX + awayPosK[id].x * (newx - predictedPoseX);
        float lastVelocityx      = awayVelocity[id].x;
        awayVelocity[id].x       = (awayPose[id].x - lastPoseX) / delTime;
        awayAcc[id].x            = (awayVelocity[id].x - lastVelocityx) / delTime;
                                
        awayPosSigmaSqK[id].y    = awayPosSigmaSqK[id].y * ( 1 - awayPosK[id].y) + SIGMA_SQ_NOISE_POS * delTime;
        awayPosK[id].y           = awayPosSigmaSqK[id].y / (awayPosSigmaSqK[id].y + SIGMA_SQ_OBSVN_POS);
        float  predictedPoseY    = /*newy;*/awayPose[id].y + awayVelocity[id].y * (delTime);
        float  lastPoseY         = awayPose[id].y;
        awayPose[id].y           = predictedPoseY + awayPosK[id].y * (newy - predictedPoseY);
        float lastVelocityy      = awayVelocity[id].y;
        awayVelocity[id].y       = (awayPose[id].y - lastPoseY) / delTime;
        awayAcc[id].y            = (awayVelocity[id].y - lastVelocityy) / delTime;
                                
        awayAngleSigmaSqK[id]    = awayAngleSigmaSqK[id] * ( 1 - awayAngleK[id]) + SIGMA_SQ_NOISE_ANGLE * delTime;
        awayAngleK[id]           = awayAngleSigmaSqK[id] / (awayAngleSigmaSqK[id] + SIGMA_SQ_OBSVN_ANGLE);
        float  predictedAngle    = awayAngle[id] + awayOmega[id] * (delTime);
        float  lastAngle         = awayAngle[id];
        awayAngle[id]            = /*nearestAngle(newangle, awayAngle[id]);*/predictedAngle + awayAngleK[id] * (nearestAngle(newangle, awayAngle[id]) - predictedAngle);
        float lastAngularV       = awayOmega[id];
        awayOmega[id]            = (awayAngle[id] - lastAngle) / delTime;
        awayAngularAcc[id]       = (awayOmega[id] - lastAngularV) / delTime;
        if(awayAngle[id] > PI)        awayAngle[id] -= 2*PI;
        else if(awayAngle[id] <=-PI)  awayAngle[id] += 2*PI;
        awayLastUpdateTime[id]   = timeCapture;
        checkValidX(awayPose[id].x, awayVelocity[id].x, newx);
        checkValidY(awayPose[id].y, awayVelocity[id].y, newy);
        checkValidA(awayAngle[id], awayOmega[id], newangle);
        //printf("Awaybots %d: %f %f %f %f %lf\n", id, awayPose[id].x, awayPose[id].y, predictedPoseX, awayPosK[id].y, delTime);
      }
    }
   
    
    // Ball info
    if (ballsNum > 0) {
		int newx =env->currentBall.pos.x*100 - CENTER_X;
      int newy = env->currentBall.pos.y *100- CENTER_Y;
      double delTime =1/60.0;// timeCapture - ballLastUpdateTime;
      float garbage;

      linearTransform(newx, newy, garbage,invert_field);
     
      ballPosSigmaSqK.x          = ballPosSigmaSqK.x * ( 1 - ballPosK.x) + SIGMA_SQ_NOISE_POS * delTime;
      ballPosK.x                 = ballPosSigmaSqK.x / (ballPosSigmaSqK.x + SIGMA_SQ_OBSVN_POS);
      float  predictedPoseX      = ballPose.x + ballVelocity.x * (delTime);
      float  lastPoseX           = ballPose.x;
      ballPose.x                 = predictedPoseX + ballPosK.x * (newx - predictedPoseX);
      float lastVelocityx        = ballVelocity.x;
      ballVelocity.x             = (ballPose.x - lastPoseX) / delTime;
      ballAcceleration.x         = (ballVelocity.x - lastVelocityx) / delTime;

      ballPosSigmaSqK.y          = ballPosSigmaSqK.y * ( 1 - ballPosK.y) + SIGMA_SQ_NOISE_POS * delTime;
      ballPosK.y                 = ballPosSigmaSqK.y / (ballPosSigmaSqK.y + SIGMA_SQ_OBSVN_POS);
      float  predictedPoseY      = ballPose.y + ballVelocity.y * (delTime);
      float  lastPoseY           = ballPose.y;
      ballPose.y                 = predictedPoseY + ballPosK.y * (newy - predictedPoseY);
      float lastVelocityy        = ballVelocity.y;
      ballVelocity.y             = (ballPose.y - lastPoseY) / delTime;
      ballAcceleration.y         = (ballVelocity.y - lastVelocityy) / delTime;
      //printf("Ball: %f %f %f %f %lf\n", ballPose.x, ballPose.y, ballVelocity.x, ballVelocity.y, delTime);
      
      checkValidX(ballPose.x, ballVelocity.x, newx);
      checkValidY(ballPose.y, ballVelocity.y, newy);
      ballLastUpdateTime         = timeCapture;
    }
    mutex->leave();
  }

  void Kalman::update(BeliefState& state)
  {
    mutex->enter();
    // TODO : Add delTime here and update the state state with respect to delTime
    struct timeval tv2;
    Util::Timer::gettimeofday(&tv2, NULL);
    double timeNow      = tv2.tv_sec+tv2.tv_usec/1000000.0;
    state.currFrameNum++;
    for (int botID = 0; botID < 5; ++botID)
    {
      double delTime = 0;
      state.homePos[botID]   = Vector2D<int>(homePose[botID].x, homePose[botID].y);/*Vector2D<int>(homePose[botID].x+homeVelocity[botID].x*delTime, 
      homePose[botID].y+homeVelocity[botID].y*delTime);*/
      state.homeAngle[botID] = homeAngle[botID];// + homeOmega[botID]*delTime;
      state.homeVel[botID]   = homeVelocity[botID];
      state.homeOmega[botID] = homeOmega[botID];
      state.homeAcc[botID]   = homeAcc[botID];
      state.homeAngAcc[botID]= homeAngularAcc[botID];
    }
    //printf("updating %d %d\n", state.homePos[1].x, state.homePos[1].y);
    for (int botID = 0; botID < 5; ++botID)
    {
      double delTime = 0;
      state.awayPos[botID]   = Vector2D<int>(awayPose[botID].x, awayPose[botID].y);
      state.awayAngle[botID] = awayAngle[botID];
      state.awayVel[botID]   = awayVelocity[botID];
      state.awayOmega[botID] = awayOmega[botID];
      state.awayAcc[botID]   = awayAcc[botID];
      state.awayAngAcc[botID]= awayAngularAcc[botID];
    }
      double delTime = 0;
      state.ballPos = Vector2D<int>(ballPose.x, ballPose.y);
      state.ballVel = ballVelocity;
      state.ballAcc = ballAcceleration;

    
    state.computeBallLocation();
    state.computeBallInDBox();
    //state.computeBallSide();
    state.computeBallInStrips();
    mutex->leave();
  }
}