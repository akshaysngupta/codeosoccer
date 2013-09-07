#pragma once
#ifndef COMM_H
#define COMM_H

#include <list>
#include "../Strategy.h"
// Forward Declarations

namespace Util
{
  class CS;
}

namespace MyStrategy
{
  class BeliefState;
}
class Comm
{
  private:
   
	Comm();	
	static Comm* singletonInstance;
  protected:
    static Util::CS commCS;
	
  public:
	   Environment *envi;
	static Comm* getInstance();
	virtual ~Comm();
    Util::CS*                   debug_cs;
    /* This function must be implemented in the derived classes.
     * Remember to guard the entire function body with the CS object commCS
     * otherwise the result will be undefined and communication protocols
     * may break down
     */
     void sendCommand(int   botID,           // valid bot ID
                             float v_l,             // in mm/sec
                             float v_r             // in mm/sec
                             ) ;    // on(true) or off(false)
 }; // class Comm
 // namespace HAL

#endif // COMM_H
