#include "Utils.h"

PlaneUtils::PlaneUtils() {}

double PlaneUtils::fraction_to_collision(const Vector3d &p, const Vector3d &p_new, const Plane &plane, float radius) {
  double d = abs((p_new - p) * plane.norm);
  double h = abs((plane.point - p) * plane.norm) - radius;
  if(h < d) {
    return h / d;
  }
  if (d < 0.0005 && h < 0.0005) {
    return 0.00;
  }
  return -1;
}

Vector3d PlaneUtils::reflect(const Vector3d &v_col, const Plane &plane) {
  Vector3d vp = plane.norm * (v_col * plane.norm);
  Vector3d vt = v_col - vp;
  return -1*plane.coef_restitution*vp + (1 - plane.coef_friction) * vt;
}
