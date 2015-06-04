#ifndef __EMITTER__
#define __EMITTER__
#include <memory>
#include "ParticleControl.h"

class Emitter : public ParticleControl {
public:
  Emitter();
  Emitter(const Emitter &);
  ~Emitter();

  // Initiallizes the emitter with 500 Particles.
  void init();

  virtual void force_calc(Particle &, float t);
  virtual void sim();
  std::vector<Particle>& live_particles();
  std::vector<Particle>& dead_particles();

private:
  class Impl;
  std::shared_ptr<Impl> mImpl;
};

#endif // __EMITTER__
