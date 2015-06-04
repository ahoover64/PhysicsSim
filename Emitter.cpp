#include "Emitter.h"
#include "Gauss.h"

class Emitter::Impl {
public:
  std::vector<Particle> alive;
  std::vector<Particle> dead;
  Gauss random = Gauss(0.0,1.0,0);
  Vector3d vortex1;
  Vector3d vortex2;
  void operators(Particle &p, float t) {
    //p.force = Vector3d(0,0,0);
    Vector3d sig = (vortex1 - vortex2).normalize() % (p.pos - vortex1);
    double inverse_r = 1 / ((p.pos - vortex1) - ((p.pos - vortex1) * (vortex1 - vortex2) * (vortex1 - vortex2).normalize())).norm();
    double omega = 5.0 * inverse_r;
    p.vel = omega * sig;
  }
};

Emitter::Emitter() : ParticleControl(), mImpl(new Impl) {
  mImpl->random = Gauss(0.0, 1.0, 100);
  mImpl->vortex1 = Vector3d(0,0,0);
  mImpl->vortex2 = Vector3d(0,10.0,0);
}

void Emitter::init() {
  Particle p;
  for(int i = 0; i < 500; i++) {
    mImpl->dead.push_back(p);
  }
}

void Emitter::force_calc(Particle &p, float t) {
  float v = 1.0; // VORTEX SPEED
  float y = p.pos[1];
  float r = .1 * y * y + 0.01;
  float a = v * v / r;
  p.force = -1 * p.mass * a * Vector3d(p.pos[0], 0, p.pos[2]).normalize();
}

void Emitter::sim() {
  // USED FOR COLLISIONS
  Particle p_old;
  Particle p_col;
  //double frac;
  Plane pl;
  //int col_cnt = 0;
  double r = mImpl->random();
  if(r < -1.0 && mImpl->dead.size() > 100) {
    Particle p = mImpl->dead.back();
    mImpl->dead.pop_back();
    p.pos = Vector3d(1.0,1.1,0.1);
    p.vel = Vector3d(0.0,0.75,2.5);
    //p.pos = Vector3d(mImpl->random() * 1.5,mImpl->random()* 2.0 + 5,mImpl->random() * 1.5);
    //p.vel = Vector3d(mImpl->random() * 1.5,0,mImpl->random() * 1.5);
    mImpl->alive.push_back(p);
  }
  //if(r < -1.5 && mImpl->alive.size() > 0) {
  //  mImpl->dead.push_back(mImpl->alive.back());
  //  mImpl->alive.pop_back();
  //}
  Vector3d accel;
  float t = 0.0;
  int particle_size = mImpl->alive.size();
  for(int i = 0; i < particle_size; i++) {
    p_old = mImpl->alive[i];
    mImpl->alive[i].force = Vector3d(0,0,0);
    force_calc(mImpl->alive[i], t);
    accel = mImpl->alive[i].force / mImpl->alive[i].mass;
    //mImpl->operators(mImpl->alive[i], t);
    ParticleControl::euler_integrator(mImpl->alive[i], t);
  }
  static float cumulo = 0;
  cumulo += ParticleControl::params().deltaT;
  if(cumulo >= 1.0 / ParticleControl::params().fps) {
    cumulo = 0;
    //PerspDisplay(); // REPLACED BY THE DISPLAY FUNCTION
  }
}

std::vector<Particle>& Emitter::live_particles() {
  return mImpl->alive;
}

std::vector<Particle>& Emitter::dead_particles() {
  return mImpl->dead;
}

Emitter::~Emitter() {}
