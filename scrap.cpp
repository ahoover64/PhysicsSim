
SpringMesh::SpringMesh(int n, float len, float k, float d, Plane p) : ParticleControl(), mImpl(new Impl) {
  mImpl->n = n;
  mImpl->len = len;
  mImpl->p = p;
  float lpn = len / n;
  for (double i=0; i<n; i++) {
    for (double j=0; j<n; j++) {
      Particle *pa = new Particle();
      Particle *pa2 = new Particle();
      pa->pos[0] = p.point[0] - 0.5*len + i*lpn;
      pa->pos[2] = p.point[2] - 0.5*len + j*lpn;
      pa2->pos[0] = p.point[0] - 0.5*len + i*lpn;
      pa2->pos[2] = p.point[2] - 0.5*len + (j+1)*lpn;
      ParticleControl::add_particle(*pa);
      ParticleControl::add_particle(*pa2);
      Strut *s = new Strut();
      s->a = pa; s->b = pa2;
      s->k = k; s->d = d; s->lo = lpn;
      mImpl->struts.push_back(s);
      pa = new Particle();
      pa2 = new Particle();
      pa->pos[0] = p.point[0] - 0.5*len + i*lpn;
      pa->pos[2] = p.point[2] - 0.5*len + j*lpn;
      pa2->pos[0] = p.point[0] - 0.5*len + (i+1)*lpn;
      pa2->pos[2] = p.point[2] - 0.5*len + j*lpn;
      ParticleControl::add_particle(*pa);
      ParticleControl::add_particle(*pa2);
      s = new Strut();
      s->a = pa; s->b = pa2;
      s->k = k; s->d = d; s->lo = lpn;
      mImpl->struts.push_back(s);
      if (j == n-1){
        pa = new Particle();
        pa2 = new Particle();
        pa->pos[0] = p.point[0] - 0.5*len + i*lpn;
        pa->pos[2] = p.point[2] - 0.5*len + (j+1)*lpn;
        pa2->pos[0] = p.point[0] - 0.5*len + (i+1)*lpn;
        pa2->pos[2] = p.point[2] - 0.5*len + (j+1)*lpn;
        ParticleControl::add_particle(*pa);
        ParticleControl::add_particle(*pa2);
        s = new Strut();
        s->a = pa; s->b = pa2;
        s->k = k; s->d = d; s->lo = lpn;
        mImpl->struts.push_back(s);
      }
      if (i == n-1){
        pa = new Particle();
        pa2 = new Particle();
        pa->pos[0] = p.point[0] - 0.5*len + (i+1)*lpn;
        pa->pos[2] = p.point[2] - 0.5*len + j*lpn;
        pa2->pos[0] = p.point[0] - 0.5*len + (i+1)*lpn;
        pa2->pos[2] = p.point[2] - 0.5*len + (j+1)*lpn;
        ParticleControl::add_particle(*pa);
        ParticleControl::add_particle(*pa2);
        s = new Strut();
        s->a = pa; s->b = pa2;
        s->k = k; s->d = d; s->lo = lpn;
        mImpl->struts.push_back(s);
      }
    }
  }
}
