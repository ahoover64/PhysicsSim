#include "RopeSim.h"
#include "Strut.h"

class RopeSim::Impl {
public:
  std::vector<Strut*> struts;
  std::vector<Particle*> particles;
  int num_joints;
  double k;
  double d;
  double len;
  double mass_per_increment;

  void sv_to_rope(StateVector &sv) {

  }

  StateVector& rope_to_sv() {
    StateVector sv(num_joints * 6);
    for(int i = 0; i < num_joints; i++) {
      sv[3 * i] = *particles[i].pos[0];
      sv[3 * i + 1] = *particles[i].pos[1];
      sv[3 * i + 2] = *particles[i].pos[2];
      sv[3 * (i + num_joints)] = *particles[i].vel[0];
      sv[3 * (i + num_joints) + 1] = *particles[i].vel[1];
      sv[3 * (i + num_joints) + 2] = *particles[i].vel[2];
    }
    return sv;
  }
};

RopeSim::RopeSim() : mImpl(new Impl) {

}

RopeSim::RopeSim(int num_joints, double k, double d, double len) : mImpl(new Impl) {
  mImpl->num_joints = num_joints;
  mImpl->k = k;
  mImpl->d = d;
  mImpl->len = len;
  mImpl->mass_per_increment = 0.1;
  double lpn = len / num_joints;
  for (int i = 0; i < num_joints; i++) {
    Particle *pa = new Particle();
    pa->pos = Vector3d(0, 10 - i * (lpn), 0);
    pa->mass = 0.1;
    mImpl->particles.push_back(pa);
    if(i > 0) {
      Strut *s = new Strut();
      s->a
    *mImpl->struts[i] = s;= pa; s->b = mImpl->particles[mImpl->particles.size()-num_joints-1];
      s->k = k; s->d = d; s->lo = lpn;
      mImpl->struts.push_back(s);
    }
  }
}

RopeSim::RopeSim(const RigidBodySim &other) : mImpl(new Impl) {
  mImpl = other.mImpl;
}

RopeSim& RopeSim::operator=(const RigidBodySim &other) {
  if (this != &other) {
    mImpl = other.mImpl;
  }
  return *this;
}

RopeSim::~RopeSim() {}

void RBSim::force() {
}

void RBSim::ode(StateVector &sv, (void*) force_func) {
  // NEED RK4
}

void sim(double deltaT) {
  int n = mImpl->num_joints;
  for(int i = 0; i < n; i++) {
    mImpl->particles[i]->force = Vector3d(0,0,0);
  }
  for(int i = 0; i < mImpl->struts.size(); i++) {
    Strut s = *mImpl->struts[i];
    double dist = (s.b->pos - s.a->pos).norm();
    Vector3d uab = (s.b->pos - s.a->pos) / dist;
    Vector3d fsab = s.k * (dist - s.lo) * uab;
    Vector3d fdab = s.d * ((s.b->vel - s.a->vel) * uab) * uab;
    s.a->force = s.a->force + fsab + fdab;
    s.b->force = s.b->force - fsab - fdab;
    if(s.a->force.norm() > 150.0) {
      s.a->force = s.a->force.normalize() * 150.0;
    }
    if(s.b->force.norm() > 150.0) {
      s.b->force = s.b->force.normalize() * 150.0;
    }
    *mImpl->struts[i] = s;
  }
  for(int i = 0; i < n; i++) {
    *mImpl->particles[i].force = *mImpl->particles[i].force
    + mImpl->mass_per_increment * (n - i) * Vector3d(0,-9.8,0);
  }
  *mImpl->particles[0].force = Vector3d(0,0,0);
  StateVector sv = rope_to_sv();
  ode(sv, force);
  sv_to_rope(sv);
}
