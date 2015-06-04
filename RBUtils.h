#include "Quaternion.h"
#include "Matrix.h"
Quaternion matrix_to_quaternion(const Matrix3x3 &m) {
  Quaternion q;
  double tr, s;

  tr = m[0][0] + m[1][1] + m[2][2];

  if (tr >= 0) {
    s = sqrt(tr + 1);
    q.q.w = 0.5 * s;
    s = 0.5/s;
    q.q.x = (m[2][1] - m[1][2]) * s;
    q.q.y = (m[0][2] - m[2][0]) * s;
    q.q.z = (m[1][0] - m[0][1]) * s;
  }
  else {
    int i = 0;

    if (m[1][1] > m[0][0])
      i = 1;
    if (m[2][2] > m[i][i])
      i = 2;

    switch(i) {
      case 0:
        s = sqrt((m[0][0] - (m[1][1] + m[2][2])) + 1);
        q.q.x = 0.5 * s;
        s = 0.5/s;
        q.q.y = (m[0][1] + m[1][0]) * s;
        q.q.z = (m[2][0] + m[0][2]) * s;
        q.q.w = (m[2][1] - m[1][2]) * s;
        break;
      case 1:
        s = sqrt((m[1][1] - (m[2][2] + m[0][0])) + 1);
        q.q.y = 0.5 * s;
        s = 0.5/s;
        q.q.z = (m[1][2] + m[2][1]) * s;
        q.q.x = (m[0][1] + m[1][0]) * s;
        q.q.w = (m[0][2] - m[2][0]) * s;
        break;
      case 2:
        s = sqrt((m[2][2] - (m[0][0] + m[1][1])) + 1);
        q.q.z = 0.5 * s;
        s = 0.5/s;
        q.q.w = (m[2][0] + m[0][2]) * s;
        q.q.y = (m[1][2] + m[2][1]) * s;
        q.q.w = (m[1][0] - m[0][1]) * s;
        break;
    }
  }
  return q;
}

Matrix3x3 quaternion_to_matrix(Quaternion q) {
return Matrix3x3(1 - (2 * q.q.y * q.q.y) - (2 * q.q.z * q.q.z),
  (2*q.q.x * q.q.y) - (2 * q.q.w * q.q.z),
  (2 * q.q.x * q.q.z) + (2*q.q.w * q.q.y),
  (2*q.q.x * q.q.y) + (2*q.q.w*q.q.z),
  (1)-(2*q.q.x*q.q.x)-(2*q.q.z*q.q.z),
  (2*q.q.y*q.q.z) - (2*q.q.w*q.q.x),
  (2* q.q.x*q.q.z) - (2*q.q.w*q.q.y),
  (2*q.q.y*q.q.z) + (2*q.q.w*q.q.x),
  (1) - (2*q.q.x*q.q.x) - (2*q.q.y*q.q.y));
}

Matrix3x3 Star(Vector3d a) {
  return Matrix3x3(0.0, -a.z, a.x,
       a.z, 0.0, -a.x,
      -a.y, a.x, 0.0);
}
