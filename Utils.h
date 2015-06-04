#ifndef __UTILS__
#define __UTILS__

#include "Vector.h"

struct Plane {
  Vector3d point;
  Vector3d norm;
  float coef_friction;
  float coef_restitution;
  float width;
  float height;
  Vector4d color;
};

class PlaneUtils {
public:
  // Returns -1 if there is no collision
  static double fraction_to_collision(const Vector3d &p, const Vector3d &p_new,
                                      const Plane &plane, float radius);
  static Vector3d reflect(const Vector3d &v_col, const Plane &plane);
private:
  PlaneUtils();
};

#endif // __UTILS__
