# pragma once
#include "../mainheaders.hpp"
#include "../common/include/thread.h"
#include "../common/include/cs.hpp"
#include "../winDebugger/Client.h"
using namespace std;
using namespace MyStrategy;

class Tester : public Util::Thread
{
public:
  BeliefState &state;
  
  Comm*    comm;
 
  Tester(BeliefState &state_):
	  state(state_)
  {
  }
  
  void run()
  {
    print("bot0 pos: %d %d\n",state.homePos[0].x,state.homePos[0].y);

		attacker(&state);
    defender(&state);
    goalkeeper(&state);
    game(&state);

    return;
  }
};