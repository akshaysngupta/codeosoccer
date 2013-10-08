#include "stdafx.h"
#include <cassert>
#include "comm.h"
#include "../common/include/cs.hpp"
#include "../Strategy.h"
#include <cmath>
#define PI 3.141592653589793
using namespace Util;

  CS   Comm::commCS;  
Comm* Comm::singletonInstance=0;
  Comm::Comm() {}

  Comm* Comm::getInstance()
  {
	  if(singletonInstance==0)
	  {
		  singletonInstance = new Comm();
	  }
	  return singletonInstance;

  }
  Comm::~Comm()
  { }  
  
     
  void Comm::sendCommand(int   botID,           // valid bot ID
                             float v_l,             // in mm/sec
                             float v_r             // in mm/sec
							 )  // on(true) or off(false)

	 {
		// assert (fabs(v_l)>125 ||fabs(v_r)>125);
		 if(botID <=2 && botID >=0)
		 {
			 envi->home[botID].velocityLeft = v_l;
			 envi->home[botID].velocityRight = v_r;
		 }
	 }
 // namespace HAL
