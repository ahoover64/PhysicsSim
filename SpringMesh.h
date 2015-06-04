#ifndef __SPRINGMESH_H__
#define __SPRINGMESH_H__
#include <memory>
#include "ParticleControl.h"
#include "Strut.h"
#include "Utils.h"

class SpringMesh : public ParticleControl{
public:
  SpringMesh();
  SpringMesh(int n, float len, float k, float d, Plane p);
  ~SpringMesh();

  std::vector<Strut*>& struts();
  std::vector<Particle*>& mesh_particles();
  void sim();
  void force_calc(Particle &, float t);
private:
  class Impl;
  std::shared_ptr<Impl> mImpl;
};

#endif // __SPRINGMESH_H__
