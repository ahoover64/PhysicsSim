#ifndef __SIM_PARAMS__
#define __SIM_PARAMS__

#include "Vector.h"

struct SimParams {
  Vector3d gravity = Vector3d(0,-10.0,0);
  float deltaT = 0.005;
  int fps = 60;
};

#endif // __SIMPARAMS
