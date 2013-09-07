#include <cstdlib>
using namespace std;
namespace Util {
  class RandGenerator {
  public:
    static void reseed() {
      srand(time(0));
    }
    static float get_rand_float(float mean, float max_deviation) {
      return mean + rand_direction()*max_deviation*(rand_fraction());
    }
    static int rand_direction() {
      return (rand()&1)?(-1):1;
    }
    static float rand_fraction() {
      return rand()/(float)RAND_MAX;
    }
  };
}