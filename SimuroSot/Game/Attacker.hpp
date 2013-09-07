#include "../Skills/skills.h"

namespace MyStrategy
{
  // Example
  void attacker(BeliefState *state)
  {
    print("Attacker\n");
    
    Vec2D dpoint(HALF_FIELD_MAXX/2,HALF_FIELD_MAXY/2);
    GoToBall(1,state,true);
  }
}