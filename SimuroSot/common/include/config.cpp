#pragma once
#include <iostream>
#include "stdafx.h"
#include "config.h"
using namespace std;


 const int GOAL_DEPTH                   = SELECT(3.9373, 393.73);
 const float MOVING_BALL_VELOCITY       =	  SELECT(40, 30);
 const float MIN_DIST_FROM_TARGET       =	  SELECT(30.0, 25.0);
 const int CENTER_X                     = SELECT(0, 000);
 const int CENTER_Y                     = SELECT(0, 0);
 const int HALF_FIELD_MAXX              = SELECT(47.24435, 4724.435); //actual 225 (rugged surface at end)
 const int HALF_FIELD_MAXY              = SELECT(35.29235, 3529.235);

 const int OUR_GOAL_MAXY                = SELECT(7.87405,787.405);
 const int OUR_GOAL_MINY                = SELECT(-7.87405,-787.7405);
 const int OPP_GOAL_MAXY                = SELECT(7.87405, 787.405);
 const int OPP_GOAL_MINY                = SELECT(-7.87405,-787.7405);
 const int OUR_GOAL_WIDTH               = OUR_GOAL_MAXY - OUR_GOAL_MINY;
 const int OPP_GOAL_WIDTH               = OPP_GOAL_MAXY - OPP_GOAL_MINY;
 const int CENTER_CIRCLE_DIAMETER       = SELECT(1000,1000);
 const int DBOX_WIDTH                   = SELECT(12,1200);       //Along X direction
 const int DBOX_HEIGHT                  = SELECT(30,500);       //Along Y direction(half height in each y direction)
const int DBOX_DEPTH                   = SELECT(10, 10);
const int BALL_AT_CORNER_THRESH        = SELECT(20,20); 
/* Bot Parameteres configuration */
const float ROTATION_FACTOR            = SELECT(0.05, 0.15);                //

const float RFACTOR                    = SELECT(3,   0.3);
const float RFACTOR_SMALL              = SELECT(0.6, 0.15);

const int CLEARANCE_PATH_PLANNER       = SELECT(500, 700);               //mm
const int MID_FIELD_THRESH             = SELECT(10, 10);                       // mm
const float BOT_RADIUS                 = SELECT(70, 300);                       // mm
const float BALL_RADIUS                = SELECT(40, 40);                       // mm
const float SAFE_RADIUS                = (BOT_RADIUS * 2);
const float COLLISION_DIST             = (BOT_RADIUS * 7);
const int DRIBBLER_BALL_THRESH         = SELECT(110, 500);            // mm


const int BOT_BALL_THRESH              = SELECT(105, 300);                  // mm
const int BOT_BALL_THRESH_FOR_PR       = SELECT(105, 300);                  // mm
const int BOT_POINT_THRESH             = SELECT(105, 300);                     // mm

const int STRIP_WIDTH_X                = ForwardX(BOT_RADIUS*1.5);
const int STRIP_WIDTH_Y                = BOT_RADIUS*1.5;
const int MAX_FIELD_DIST               = SELECT(1000, 3500);                // mm
const float MAX_WHEEL_SPEED            = SELECT(2000, 100);                 //mm/s
const float MAX_BOT_LINEAR_ACC         = SELECT(1000, 100);                // mm/s/s
const float MAX_BOT_LINEAR_VEL_CHANGE  = SELECT(10, 3);
const float MAX_BOT_SPEED              = SELECT(1800, 120.0);                 // mm
const float MIN_BOT_SPEED              = SELECT(5, 30);                     // mm/s
const float MAX_BOT_OMEGA              = SELECT(7, 40);                     // rad/s//2
const float MIN_BOT_OMEGA              = SELECT(0.15, 0.15);                     // rad/s
const float DRIBBLER_BALL_ANGLE_RANGE  = SELECT(0.2f, 0.1f);                // rad
const float SATISFIABLE_THETA          = SELECT(0.08f, 0.1f);                // rad
const float SATISFIABLE_THETA_SHARP    = SELECT(0.01f, 0.01f);              // rad
const float MAX_BALL_STEAL_DIST        = SELECT(800, 800);
/* If the velocity of a bot is below this value, then the bot has effectively zero velocity */
const float ZERO_VELOCITY_THRESHOLD    = SELECT(10, 10);
const float ZERO_VELOCITY_THRESHOLD_SQ = (ZERO_VELOCITY_THRESHOLD * ZERO_VELOCITY_THRESHOLD);
const float LOW_BALL_VELOCITY_THRES    = SELECT(50, 50);
const float LOW_BALL_VELOCITY_THRES_SQ = (LOW_BALL_VELOCITY_THRES*LOW_BALL_VELOCITY_THRES);
// Parameters useful for camera's data transformation.
const int OUR_GOAL_Y = 500;
const int OPP_GOAL_Y = 0;
const float NETWORK_DELAY = SELECT(0.05, 0.05);  // Network Delay in miliseconds
//Distance Hysteresis factor for switching of roles
const int HYSTERESIS = SELECT(20000, 300);