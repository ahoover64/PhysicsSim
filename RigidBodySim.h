#ifndef __RIGID_BODY__
#define __RIGID_BODY__

#include "RBUtils.h"
#include "StateVector.h"
#include <iostream>
#include <memory>
#include <vector>

class RBSim {
public:
  RBSim();
  RBSim(const RBSim &);
  RBSim& operator=(const RBSim &);
  ~RBSim();

  Vector3d force();
  void sim(double deltaT);

private:
  void ode(StateVector &sv, (void*) force_func);
  class Impl;
  std::shared_ptr<Impl> mImpl;
};


#endif // __RIGID_BODY__
