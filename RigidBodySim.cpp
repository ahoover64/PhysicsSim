#include "RBSim.h"

class RBSim::Impl {
public:
  double mass = 5;
  Matrix3x3 Ibody, IbodyInv;
  std::vector<Vector3d> verticies;
  Vector3d x;
  Quaternion q;
  Vector3d p = Vector3d(0,0,0);
  Vector3d L = Vector3d(0,0,0);
  // Derived Variables
  Matrix3x3 R;
  Vector3d v = Vector3d(0,0,0);
  Vector3d omega = Vector3d(0,0,0);
  Vector3d F = Vector3d(0,0,0);
  Vector3d tau = Vector3d(0,0,0);
  void sv_to_body(StateVector &sv) {

  }
  StateVector& body_to_sv() {
    StateVector sv(13);
    sv[0] = v[0]; sv[1] = v[1]; sv[2] = v[2];
    q = matrix_to_quaternion(R);
    Quaternion qdot = 0.5 * omega * q;
    sv[3] = q.w; sv[4] = q.x; sv[5] = q.y; sv[6] = q.z;
    sv[7] = F[0]; sv[8] = F[1]; sv[9] = F[2];
    sv[10] = tau[0]; sv[11] = tau[1]; sv[12] = tau[2];
    return sv;
  }
};

RBSim::RBSim() : mImpl(new Impl) {
  mImpl->x = Vector3d(0, 5, 5);
  // Ibody for Cube of lide lengths 2
  mImpl->Ibody.set(mImpl->mass*(2*2+2*2), 0, 0
                   0, mImpl->mass*(2*2+2*2), 0
                   0, 0, mImpl->mass*(2*2+2*2));
  mImpl->IbodyInv = Ibody.inv();
  mImpl->R.set(1, 0, 0,
               0, 1, 0,
               0, 0, 1);
}

RBSim::RBSim(const RBSim &other) : mImpl(new Impl) {
  mImpl = other.mImpl;
}

RBSim& RBSim::operator=(const RBSim &other) {
  if (this != &other) {
    mImpl = other.mImpl;
  }
  return *this;
}

RBSim::~RBSim() {}

Vector3d RBSim::force() {
  Vector3d gravity(0,-9.8,0);
  return gravity * mImpl->mass;
}

void RBSim::ode(StateVector &sv, (void*) force_func) {
  // NEED RK4
}

void RBSim::sim(double deltaT) {
  StateVector sv = mImpl->body_to_sv();
  ode(sv, force);
  sv_to_body(sv);
}
