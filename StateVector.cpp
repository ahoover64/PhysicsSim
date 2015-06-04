#include "StateVector.h"
#include <vector>

class StateVector::Impl {
public:
  Impl() : sv(13, 0.0), size(13) {}
  Impl(int num) : sv(num, 0.0), size(num) {}
  Impl(const Impl &other) : sv(other.sv), size(other.size) {}
  Impl(const std::shared_ptr<Impl> &other) : sv(other->sv), size(other->size) {}
  Impl& operator=(const Impl &other) {
    if (this != &other) {
      sv = other.sv;
      size = other.size;
    }
    return *this;
  }
  std::vector<double> sv;
  int size;
};

StateVector::StateVector() : mImpl(new Impl) {}

StateVector::StateVector(int size) : mImpl(new Impl(size)) {}

StateVector::StateVector(const StateVector &other) : mImpl(new Impl(other.mImpl)) {};

StateVector& StateVector::operator=(const StateVector &other) {
  if (this != &other) {
    mImpl = other.mImpl;
    //mImpl->sv = other.mImpl->sv;
  }
  return *this;
}

StateVector::~StateVector() {}

int StateVector::size() const {
  return mImpl->sv.size();
}

double StateVector::operator[](int num) const {
  if (num < 0) {
    //return;
    //return this[num + mImpl->sv.size()]; // IDK IF THIS WORKS
  }
  else if (num > mImpl->size) {
    //return;
    //return this[num - mImpl->sv.size()]; // IDK IF THIS WORKS
  }
  return mImpl->sv[num];
}

double& StateVector::operator[](int num) {
  if (num < 0) {
    //return this[num + mImpl->sv.size()]; // IDK IF THIS WORKS
  }
  else if (num > mImpl->size) {
    //return this[num - mImpl->sv.size()]; // IDK IF THIS WORKS
  }
  return mImpl->sv[num];
}


// OPERATOR TIME
StateVector operator+(const StateVector& v1, const StateVector& v2) {
  int size = v1.size();
  if(size != v2.size()) {
    //return;
  }
  StateVector v = v1;
  for(int i = 0; i < size; i++) {
    v[i] += v2[i];
  }
  return v;
}

StateVector operator+(const StateVector& v, double c) {
  StateVector vnew = v;
  int size = v.size();
  for(int i = 0; i < size; i++) {
    vnew[i] = vnew[i] + c;
  }
  return vnew;
}

StateVector operator+(double c, const StateVector& v) {
  StateVector vnew = v;
  int size = v.size();
  for(int i = 0; i < size; i++) {
    vnew[i] = vnew[i] + c;
  }
  return vnew;
}

StateVector operator-(const StateVector& v1, const StateVector& v2) {
  int size = v1.size();
  if(size != v2.size()) {
    //return;
  }
  StateVector v = v1;
  for(int i = 0; i < size; i++) {
    v[i] -= v2[i];
  }
  return v;
}

StateVector operator-(const StateVector& v, double c) {
  StateVector vnew = v;
  int size = v.size();
  for(int i = 0; i < size; i++) {
    vnew[i] = vnew[i] - c;
  }
  return vnew;
}

StateVector operator-(double c, const StateVector& v) {
  StateVector vnew = v;
  int size = v.size();
  for(int i = 0; i < size; i++) {
    vnew[i] = vnew[i] - c;
  }
  return vnew;
}

StateVector operator*(const StateVector& v1, const StateVector& v2) {
  int size = v1.size();
  if(size != v2.size()) {
    //return;
  }
  StateVector v = v1;
  for(int i = 0; i < size; i++) {
    v[i] *= v2[i];
  }
  return v;
}

StateVector operator*(const StateVector& v, double c) {
  StateVector vnew = v;
  int size = v.size();
  for(int i = 0; i < size; i++) {
    vnew[i] = vnew[i] * c;
  }
  return vnew;
}

StateVector operator*(double c, const StateVector& v) {
  StateVector vnew = v;
  int size = v.size();
  for(int i = 0; i < size; i++) {
    vnew[i] = vnew[i] * c;
  }
  return vnew;
}

std::ostream& operator<< (std::ostream& os, const StateVector& v) {
  for(int i = 0; i < v.size(); i++) {
    os << v[i] << ", ";
  }
  return os;
}
