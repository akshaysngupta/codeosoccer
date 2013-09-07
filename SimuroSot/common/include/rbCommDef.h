/*
 * This file contains the definition for the following datastructures :
 *
 * 1. SSLPacket (struct) : Low Level(PWM) Command Protocol for sending command to the robots
 * 2. Circle(class) : Parameters of a circle
 * 3. Line(class) : Parameters of a line
 * 4. Text(class) : Parameters of Text
 * 5. Element(union) : Contains Circle,Line,Text datatypes
 * 6. CanvasColor(enum) : Enumerates Colours for the canvas
 * 7. ElementType : Enumerates the type of element - Circle,Line,Text
 * 8. CanvasItem(class) : Containd Canvas Info
 * 9. BotCommand(class) : High Level Command Protocol for sending command to the robots
 * 10. SimPacket(class) : Contains BotCommand and CanvasItem as its members
 * 11. VisionInfo(class) : Comprises of the bots and the ball coordinates and other information derived from Computer Vision
 * 12. RefBoxCmd(class) : Protocols for Referee Box Commands
 */

#pragma once
#ifndef RB_COMM_DEF_H
#define RB_COMM_DEF_H

#ifndef WIN32
#include <stdint.h>
#endif // !WIN32
#include <string>
#include "comdef.h"
#include "geometry.hpp"

namespace HAL
{
  //----- Protocol for sending commands to SSL bots -----//
  typedef struct SSLPacket
  {
    uint8_t preamble  : 8;                // Preamble for the data packet        B0.0 - B0.7
    uint8_t           : 0;
    uint8_t teamColor : 1;                // Team Color                          B1.0 - B1.0
    uint8_t botID     : 3;                // Bot ID                              B1.1 - B1.3
    uint8_t           : 0;
    uint8_t dirW0     : 1;                // Direction for Wheel 0               B2.0 - B2.0
    uint8_t dirW2     : 1;                // Direction for Wheel 2               B2.1 - B2.1
    uint8_t dirW1     : 1;                // Direction for Wheel 1               B2.2 - B2.2
    uint8_t dribble   : 1;                // Dribbler on/off                     B2.4 - B2.4
    uint8_t kickPower : 3;                // Kick Power                          B2.5 - B2.7
    uint8_t           : 0;
    uint8_t pwmW0     : 8;                // PWM for Wheel 0                     B3.0 - B3.7
    uint8_t           : 0;
    uint8_t pwmW2     : 8;                // PWM for Wheel 2                     B4.0 - B4.7
    uint8_t           : 0;
    uint8_t pwmW1     : 8;                // PWM for Wheel 1                     B5.0 - B5.7
    uint8_t           : 0;
  } SSLPacket;
  
   //FIRA Packet:
  typedef struct FIRAPacket
  {
    uint8_t preamble	  : 8;					// Preamble for the data packet        B0.0 - B0.7
    uint8_t teamColor	  : 1;					// Team Color                          B1.0 - B1.0
    uint8_t botID     	: 3;          // Bot ID                              B1.1 - B1.3
    uint8_t dirWR     	: 1;          // Direction for Right Wheel           B1.4 - B1.4
    uint8_t dirWL     	: 1;          // Direction for Left Wheel            B1.5 - B1.5
    uint8_t pwmWL     	: 8;          // PWM for Wheel Left                  B2.0 - B2.7
    uint8_t pwmWR     	: 8;          // PWM for Wheel Right                 B3.0 - B3.7
  } FIRAPacket;
  //----- Protocols for sending commands to the Robocup SSL Simulator -----//
  static const int   MAX_DEBUG_ITEMS  = 20;

#ifdef WIN32 // When compiled in Windows, the shared memory is identified by aa unique string
  static const char* SHM_BLUE_ALIAS   = "ShmemBlue";
  static const char* SEM_BLUE_ALIAS   = "SemBlue";
  static const char* SHM_YELLOW_ALIAS = "ShmemYellow";
  static const char* SEM_YELLOW_ALIAS = "SemYellow";
#else // When compiled in linux, the shared memory is identified by an unique integer
  const int SHM_BLUE_ALIAS   = 5678; // Some arbitrary number
  const int SHM_YELLOW_ALIAS = 1234; // Some arbitrary number
#endif // !WIN32

  typedef struct
  {
    int x, y;
    int radius;
  } Circle;

  typedef struct
  {
    int x1, y1;
    int x2, y2;
  } Line;

  typedef struct
  {
    char str[12];
    int x, y;
  } Text;

  typedef union
  {
    Circle circle;
    Line   line;
    Text   text;
  } Element;

  enum CanvasColor
  {
    RED,
    BLUE,
    BLACK,
    WHITE
  };

  enum ElementType
  {
    CIRCLE,
    LINE,
    TEXT,
    NONE
  };

  typedef struct
  {
    ElementType type;
    Element     element;
    CanvasColor color;
  } CanvasItem;

  typedef struct
  {
    float v_x;
    float v_y;
    float v_t;
    float kickPower;  // Must be within [0..1]
    bool  dribble;
  } BotCommand;

  typedef struct
  {
    BotCommand bot[MyStrategy::HomeTeam::SIZE];
    int        numItems;                // Must be within [0..MAX_DEBUG_ITEMS]
    CanvasItem items[MAX_DEBUG_ITEMS];
  } SimPacket;


  //------ Protocols for receiving Vision information ------//

#if FIRA_COMM || FIRASSL_COMM

  /* Index of this array represents the ID of the bot markers and the value corresponding
   * to the index represents the ID assigned to that bot in the MyStrategy module
   */
  static int BlueMarkerMap[]   = {0, 1, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1};
  static int YellowMarkerMap[] = {0, 1, 2, 3, 4, -1, -1, -1, -1, -1, -1, -1};

#elif GR_SIM_COMM || SIM_COMM

  /* When using the simulator, the SSLVision is skipped entirely and so the bot ids
   * obtained are used without any mapping
   */
  static const int BlueMarkerMap[]   = {0, 1, 2, 3, 4};
  static const int YellowMarkerMap[] = {0, 1, 2, 3, 4};

#endif // SSL_COMM

  class VisionInfo
  {
  public:
    int             frameNum;

    Point2D<int>    ballPos;
    Vector2D<float> ballVel;

    Point2D<int>    bluePos[Simulator::BlueTeam::SIZE];
    Vector2D<float> blueVel[Simulator::BlueTeam::SIZE];
    float           blueAngle[Simulator::BlueTeam::SIZE];
    float           blueAngVel[Simulator::BlueTeam::SIZE];

    Point2D<int>    yellowPos[Simulator::YellowTeam::SIZE];
    Vector2D<float> yellowVel[Simulator::YellowTeam::SIZE];
    float           yellowAngle[Simulator::YellowTeam::SIZE];
    float           yellowAngVel[Simulator::YellowTeam::SIZE];

    inline VisionInfo() :
      frameNum(-1)
    { }
  };


  // TODO : Put these settings in DataBox
  static std::string      REF_BOX_MULTICAST_ADDR = "224.5.23.1";
  static const unsigned short REF_BOX_MULTICAST_PORT = 10001;
  
  class RefBoxCmd
  {
  public:
    char           cmd;            // current referee command
    unsigned char  cmdCounter;     // increments each time new command is set
    unsigned char  goalsBlue;      // current score for blue team
    unsigned char  goalsYellow;    // current score for yellow team
    unsigned short timeRemaining;  // seconds remaining for current game stage (network byte order)

    RefBoxCmd() :
      cmd(0),
      cmdCounter(-1),
      goalsBlue(0),
      goalsYellow(0),
      timeRemaining(0)
    { }
  };
} // namespace HAL

#endif // RB_COMM_DEF_H
