#include "SpringMesh.h"

class SpringMesh::Impl {
public:
  int n;
  float len;
  Plane p;
  std::vector<Strut*> struts;
  std::vector<Particle*> particles;
  void strut_calc(Strut &s) {
    double dist = (s.b->pos - s.a->pos).norm();
    Vector3d uab = (s.b->pos - s.a->pos) / dist;
    Vector3d fsab = (s.k * (dist - s.lo)) * uab;
    Vector3d fdab = s.d * ((s.b->vel - s.a->vel) * uab) * uab;
    s.a->force = s.a->force + fsab + fdab;
    s.b->force = s.b->force - fsab - fdab;
    /*if(s.a->force.norm() > 100.0) {
      s.a->force = s.a->force.normalize() * 100.0;
    }
    if(s.b->force.norm() > 100.0) {
      s.b->force = s.b->force.normalize() * 100.0;
    }*/
  }
};

SpringMesh::SpringMesh() : ParticleControl(), mImpl(new Impl) {}

SpringMesh::SpringMesh(int n, float len, float k, float d, Plane p) : ParticleControl(), mImpl(new Impl) {
  mImpl->n = n;
  mImpl->len = len;
  mImpl->p = p;
  float lpn = (float) len / n;
  for (int i=0; i<n; i++) {
    for (int j=0; j<n; j++) {
      Particle *pa = new Particle();
      pa->pos = Vector3d(len * (0.5 - (double) i / n), 0, len * (0.5 - (double) j / n));
      pa->mass = .1;
      mImpl->particles.push_back(pa);
      if(j > 0) {
        Strut *s = new Strut();
        s->a = pa; s->b = mImpl->particles[mImpl->particles.size()-2];
        s->k = k; s->d = d; s->lo = lpn;
        mImpl->struts.push_back(s);
      }
      if(i > 0) {
        Strut *s2 = new Strut();
        s2->a = pa; s2->b = mImpl->particles[mImpl->particles.size()-n-1];
        s2->k = k; s2->d = d; s2->lo = lpn;
        mImpl->struts.push_back(s2);
      }
      if(i > 0 && j < (n-1)) {
        Strut *s3 = new Strut();
        s3->a = pa; s3->b = mImpl->particles[mImpl->particles.size()-n];
        s3->k = k; s3->d = d; s3->lo = lpn * 1.414;
        mImpl->struts.push_back(s3);
      }
      if(i > 0 && j > 0) {
        Strut *s4 = new Strut();
        s4->a = pa; s4->b = mImpl->particles[mImpl->particles.size()-n-2];
        s4->k = k; s4->d = d; s4->lo = lpn * 1.414;
        mImpl->struts.push_back(s4);
      }
    }
  }
}

SpringMesh::~SpringMesh() {}

std::vector<Strut*>& SpringMesh::struts() {
  return mImpl->struts;
}

std::vector<Particle*>& SpringMesh::mesh_particles() {
  return mImpl->particles;
}

void SpringMesh::force_calc(Particle &p, float t) {
  p.force = p.force + ParticleControl::params().gravity * p.mass;
  //Vector3d wind_force = Vector3d(0.05,0,0.05);
  //p.force = p.force + wind_force * p.pos[1] * p.pos[1] * 0.01;
}

void SpringMesh::sim() {
  ParticleControl::sim();
  int n = mImpl->n;
  static Vector3d force_sum = Vector3d(0,0,0);
  for(int i = 0; i < mImpl->particles.size(); i++) {
    mImpl->particles[i]->force = Vector3d(0,0,0);
  }
  for(int i = 0; i < mImpl->struts.size(); i++) {
    Strut s = *mImpl->struts[i];
    mImpl->strut_calc(s);
    *mImpl->struts[i] = s;
  }
  for(int i = 0; i < mImpl->particles.size(); i++) {
    Particle p = *mImpl->particles[i];
    // RIGID CORNERS
    //if(i == 0 || i == n - 1 || i == (n*(n-1)) || i == n * n - 1) {
    //  p.force = Vector3d(0,0,0);
    //}
    // RIGID EDGES
    if(i % n == 0 || (i+1) % n == 0 || i > n*(n-1) || i < n) {
      p.force = Vector3d(0,0,0);
    }
    else {
      force_calc(p, 0);
    }
    force_sum = force_sum + p.force;
    ParticleControl::euler_integrator(p, 0); // SHOULD BE RK4
    *mImpl->particles[i] = p;
  }
  force_sum = Vector3d(0,0,0);
}
