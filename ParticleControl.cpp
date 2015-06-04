#include "ParticleControl.h"

class ParticleControl::Impl {
public:
  std::vector<Particle> particles;
  SimParams params;
  std::vector<Plane> planes;

  void euler_integrator(Particle &p, const Vector3d &accel, float t, double frac) {
    p.vel = p.vel + accel * params.deltaT * frac;
    p.pos = p.pos + p.vel * params.deltaT * frac;
  }
};

ParticleControl::ParticleControl() : mImpl(new Impl) {}

ParticleControl::ParticleControl(const ParticleControl &other) : mImpl(new Impl) {
  mImpl->particles = other.mImpl->particles;
  mImpl->params = other.mImpl->params;
}

ParticleControl& ParticleControl::operator=(const ParticleControl &other) {
  //if (this != other) {
    mImpl->particles = other.mImpl->particles;
    mImpl->params = other.mImpl->params;
  //}
  return *this;
}

ParticleControl::~ParticleControl() {}

std::vector<Particle>& ParticleControl::particles() {
  return mImpl->particles;
}

std::vector<Plane>& ParticleControl::planes() {
  return mImpl->planes;
}

SimParams ParticleControl::params() {
  return mImpl->params;
}

void ParticleControl::add_particle(const Particle &p) {
  Particle part = p;
  mImpl->particles.push_back(part);
}

void ParticleControl::add_plane(const Plane &p) {
  Plane pl = p;
  mImpl->planes.push_back(pl);
}

void ParticleControl::set_params(const SimParams &sim) {
  mImpl->params = sim;
}

void ParticleControl::euler_integrator(Particle &p, float t) {
  Vector3d accel = p.force / p.mass;
  p.vel = p.vel + accel * mImpl->params.deltaT;
  p.pos = p.pos + p.vel * mImpl->params.deltaT;
}

void ParticleControl::rk4_integrator(Particle &p_o, void (*force_func)(Particle &,float), float t) {
  double deltaT = mImpl->params.deltaT;
  Particle p = p_o;
  force_func(p, t);
  Vector3d k1 = deltaT * p.force;
  p.pos = p_o.pos + deltaT * k1 * 0.5;
  force_func(p, t + deltaT * 0.5);
  Vector3d k2 = deltaT * p.force;
  p.pos = p_o.pos + deltaT * k2 * 0.5;
  force_func(p, t + deltaT * 0.5);
  Vector3d k3 = deltaT * p.force;
  p.pos = p_o.pos + deltaT * k3 * 0.5;
  force_func(p, t + deltaT * 0.5);
  Vector3d k4 = deltaT * p.force;
  p_o.vel = p.vel + deltaT  * (k1 + 2*k2 + 2*k3 + k4) / (6 * p.mass);
  p_o.pos = p.pos + deltaT  * p.vel; // NOT SURE
}

void ParticleControl::force_calc(Particle &p, float t) {
  p.force = mImpl->params.gravity * p.mass;
}

// Main Loop
void ParticleControl::sim() {
  // USED FOR COLLISIONS
  Particle p_old;
  Particle p_col;
  double frac;
  Plane pl;
  int col_cnt = 0;

  Vector3d accel;
  float t = 0.0;
  int particle_size = mImpl->particles.size();
  int plane_size = mImpl->planes.size();
  for(int i = 0; i < particle_size; i++) {
    p_old = mImpl->particles[i];
    mImpl->particles[i].force = 0;
    force_calc(mImpl->particles[i], t);
    accel =   mImpl->particles[i].force / mImpl->particles[i].mass;
    euler_integrator(mImpl->particles[i], t);
    // FOR PLANE COLLISION CHECKING
    for(int j = 0; j < plane_size; j++) {
      pl = mImpl->planes[j];
      frac = PlaneUtils::fraction_to_collision(p_old.pos, mImpl->particles[i].pos, pl, p_old.radius);
      if(frac >= 0.0) {
        if(col_cnt > 0) {
          p_old = p_col;
        }
        col_cnt++;
        p_col = p_old;
        mImpl->euler_integrator(p_col, accel, t, frac);
        p_col.vel = PlaneUtils::reflect(p_col.vel, pl);
        force_calc(p_col, t + frac * mImpl->params.deltaT);
        accel = p_col.force / p_col.mass;
        p_old = p_col;
        mImpl->euler_integrator(p_col, accel, t, 1.0); // Should be 1.0 - frac (Not accurate)
        mImpl->particles[i] = p_col;
      }
    }

  }
  static float cumulo = 0;
  cumulo += mImpl->params.deltaT;
  if(cumulo >= 1.0 / mImpl->params.fps) {
    cumulo = 0;
    //PerspDisplay(); // REPLACED BY THE DISPLAY FUNCTION
  }
}
