#ifndef __PARTICLE_CONTROL__
#define __PARTICLE_CONTROL__

#include "Vector.h"
#include "SimParams.h"
#include <memory>
#include <vector>

struct Particle {
  Vector3d pos = Vector3d(0,0,0);
  Vector3d vel = Vector3d(0,0,0);
  Vector3d force = Vector3d(0,0,0);
  float mass = 0.5;
  float radius = .1;
  float charge = 0.0;
  Vector4d color = Vector4d(1.0,0,0,1.0);
};

#include "Utils.h"

class ParticleControl {
public:
  ParticleControl();
  ParticleControl(const ParticleControl &);
  ParticleControl& operator=(const ParticleControl &);
  virtual ~ParticleControl();

  std::vector<Particle>& particles();
  std::vector<Plane>& planes();
  SimParams params();

  void add_particle(const Particle &p);
  void add_plane(const Plane &p);
  void set_params(const SimParams &);

  virtual void sim();
  void euler_integrator(Particle &, float t);
  void rk4_integrator(Particle &, void (*force_func)(Particle &,float), float t);
  virtual void force_calc(Particle &, float t);


private:
  class Impl;
  std::shared_ptr<Impl> mImpl;
};

#endif // __PARTICLE_CONTROL__
