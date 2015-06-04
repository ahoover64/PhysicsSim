#ifndef __STATE_VECTOR__
#define __STATE_VECTOR__

#include <memory>
#include <iostream>

class StateVector {
public:
  StateVector();
  StateVector(int size);
  StateVector(const StateVector &);
  StateVector& operator=(const StateVector &);
  ~StateVector();

  int size() const;

  double operator[](int pos) const;
  double& operator[](int pos);

private:
  class Impl;
  std::shared_ptr<Impl> mImpl;
};

// OPERATORS
StateVector operator+(const StateVector& v1, const StateVector& v2);
StateVector operator+(const StateVector& v, double c);
StateVector operator+(double c, const StateVector& v);
StateVector operator-(const StateVector& v1, const StateVector& v2);
StateVector operator-(const StateVector& v, double c);
StateVector operator-(double c, const StateVector& v);
StateVector operator*(const StateVector& v1, const StateVector& v2);
StateVector operator*(const StateVector& v, double c);
StateVector operator*(double c, const StateVector& v);

// Print Operator
std::ostream& operator<< (std::ostream& os, const StateVector& v);


#endif // __STATE_VECTOR__
