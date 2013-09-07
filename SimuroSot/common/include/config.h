#pragma once
#ifndef CONFIG_H
#define CONFIG_H
#include <string>

#define SELECT(a,b) b

#define SGN(x) (((x)>0)?1:(((x)<0)?(-1):0))
extern bool FIELD_IS_INVERTED;
template<class T> T ForwardX(T distance)
{
  return FIELD_IS_INVERTED ? (-distance) : distance;
}
#define SELECTX(a, b) ForwardX(SELECT(a, b))

static const int BOT_MAX  = SELECT(5, 5);

extern const int GOAL_DEPTH       ;
extern const float MOVING_BALL_VELOCITY     ;
extern const float MIN_DIST_FROM_TARGET     ;
extern const int CENTER_X                   ;
extern const int CENTER_Y                   ;
extern const int HALF_FIELD_MAXX            ;
extern const int HALF_FIELD_MAXY            ;

#define OUR_GOAL_X    ForwardX(-HALF_FIELD_MAXX)
#define OPP_GOAL_X    ForwardX(HALF_FIELD_MAXX)

extern const int OUR_GOAL_MAXY                ;
extern const int OUR_GOAL_MINY                ;
extern const int OPP_GOAL_MAXY                ;
extern const int OPP_GOAL_MINY                ;
extern const int OUR_GOAL_WIDTH              ;
extern const int OPP_GOAL_WIDTH              ;
extern const int CENTER_CIRCLE_DIAMETER      ;
extern const int DBOX_WIDTH                  ;
extern const int DBOX_HEIGHT                  ;
extern const int DBOX_DEPTH                   ;
extern const int BALL_AT_CORNER_THRESH        ; 
/* Bot Parameteres configuration */
extern const float ROTATION_FACTOR            ;
extern const float RFACTOR                    ;
extern const float RFACTOR_SMALL              ;

extern const int CLEARANCE_PATH_PLANNER       ;             
extern const int MID_FIELD_THRESH             ;                       
extern const float BOT_RADIUS                 ;                       
extern const float BALL_RADIUS                ;                       
extern const float SAFE_RADIUS                ; 
extern const float COLLISION_DIST             ;

extern const int DRIBBLER_BALL_THRESH         ;            


extern const int BOT_BALL_THRESH              ;                  
extern const int BOT_BALL_THRESH_FOR_PR       ;                  
extern const int BOT_POINT_THRESH             ;                     

extern const int STRIP_WIDTH_X              ;
extern const int STRIP_WIDTH_Y               ;
extern const int MAX_FIELD_DIST             ;                
extern const float MAX_WHEEL_SPEED            ;                 
extern const float MAX_BOT_LINEAR_ACC         ;               
extern const float MAX_BOT_LINEAR_VEL_CHANGE  ;
extern const float MAX_BOT_SPEED              ;                 
extern const float MIN_BOT_SPEED        ;                    
extern const float MAX_BOT_OMEGA           ;                    
extern const float MIN_BOT_OMEGA             ;                         
extern const float DRIBBLER_BALL_ANGLE_RANGE  ;                
extern const float SATISFIABLE_THETA         ;                
extern const float SATISFIABLE_THETA_SHARP    ;              
extern const float MAX_BALL_STEAL_DIST      ;
/* If the velocity of a bot is below this value, then the bot has effectively zero velocity */
extern const float ZERO_VELOCITY_THRESHOLD ;
extern const float ZERO_VELOCITY_THRESHOLD_SQ;
extern const float LOW_BALL_VELOCITY_THRES ;
extern const float LOW_BALL_VELOCITY_THRES_SQ;
// Parameters useful for camera's data transformation.
extern const int OUR_GOAL_Y ;
extern const int OPP_GOAL_Y;
extern const float NETWORK_DELAY;  // Network Delay in miliseconds
//Distance Hysteresis factor for switching of roles
extern const int HYSTERESIS;

namespace Simulator
{
  // THESE ARE THE RATES FOR UPDATE ETC
  static const int VIEW_REFRESH_RATE= 30;
  static const int UPDATE_RATE=30;
  static const int SEND_RATE=60;
  
  // THESE ARE THE NETWORK PROPERTIES
  static const int   VISION_PORT= 10002;
  static const int   COMMAND_PORT=20012;
  const char ADDRESS[] = "127.0.0.1";
  const char INTER_FACE[]  = "";
  static const int BALL_AREA = 200;
  static const int BALL_PIXEL_X= 30;
  static const int BALL_PIXEL_Y = 30;
  static const int BOT_PIXEL_X = 60;
  static const int BOT_PIXEL_Y = 60;
}
#endif  // INCL_CONFIG_H 
