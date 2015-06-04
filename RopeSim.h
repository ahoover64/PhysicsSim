#ifndef __ROPE_SIM__
#define __ROPE_SIM__

#include "StateVector.h"
#include <iostream>
#include <memory>
#include <vector>

class RopeSim {
public:
  RopeSim();
  RopeSim(int num_joints, double k, double d, double len);
  RopeSim(const RigidBodySim &);
  RopeSim& operator=(const RigidBodySim &);
  ~RopeSim();

  void force();
  void sim(double deltaT);

private:
  void ode(StateVector &sv, (void*) force_func);
  class Impl;
  std::shared_ptr<Impl> mImpl;
};

#endif //__ROPE_SIM__
