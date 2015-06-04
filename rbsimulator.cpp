//
// particleBox.cpp
//
// Hello Sphere
//
// Kacey Coley September 2012
// Modified February 2015
//
#include "Camera.h"
#include "Vector.h"


#include <string.h>
#include <cstdlib>
#include <cstdio>

#include <memory>
#include <vector>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

// EDITS BY ALEX
#include "ParticleControl.h"
#include "Emitter.h"
#include "SpringMesh.h"
#include "Utils.h"

using namespace std;

#define HDWIDTH 1920
#define HDHEIGHT 1080
#define WIDTH (HDWIDTH/2)
#define HEIGHT (HDHEIGHT/2)
#define STARTFRAME 0

#define AMBIENT_FRACTION 0.1
#define DIFFUSE_FRACTION 0.4
#define SPECULAR_FRACTION 0.4


// colors used for lights, and materials for coordinate axes

const float WHITE[] = {1, 1, 1, 1};


enum COLOR{MAT_WHITE, MAT_RED, MAT_ORANGE, MAT_YELLOW, MAT_GREEN, MAT_BLUE, MAT_VIOLET};

enum SHADEMODE{AMBIENT, DIFFUSE, SPEC_BROAD, SPEC_TIGHT, ALL};

// colors indexed for cycling material colors
float hues[][3] = {
{1, 1, 1},    // white
{1, 0, 0},    // red
{1, 0.5, 0},  // orange
{1, 1, 0},    // yellow
{0, 1, 0},    // green
{0, 0, 1},    // blue
{0.5, 0, 1}   // violet
};

static int Color;
static int ShadeMode;


int persp_win;
bool blend = true;
bool lighting= true;
bool printParams = false;
bool resting = false;
bool paused = true;

Camera *camera = nullptr;
SpringMesh *pc = nullptr;

bool showGrid = false;

static int TimerDelay;
int TIMERMSECS;

// draws a simple grid
void makeGrid() {
  glColor4f(0.0, 0.0, 0.0, 1.0);
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, WHITE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, WHITE);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, WHITE);

  glLineWidth(1.0);

  for (double i=-12; i<12; i++) {
    for (double j=-12; j<12; j++) {
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i, 0, j+1);
      glEnd();
      glBegin(GL_LINES);
      glVertex3f(i, 0, j);
      glVertex3f(i+1, 0, j);
      glEnd();

      if (j == 11){
        glBegin(GL_LINES);
        glVertex3f(i, 0, j+1);
        glVertex3f(i+1, 0, j+1);
        glEnd();
      }
      if (i == 11){
        glBegin(GL_LINES);
        glVertex3f(i+1, 0, j);
        glVertex3f(i+1, 0, j+1);
        glEnd();
      }
    }
  }

  glLineWidth(2.0);
  glBegin(GL_LINES);
  glVertex3f(-12, 0, 0);
  glVertex3f(12, 0, 0);
  glEnd();
  glBegin(GL_LINES);
  glVertex3f(0, 0, -12);
  glVertex3f(0, 0, 12);
  glEnd();
  glLineWidth(1.0);
}

void init() {
  glEnable(GL_LINE_SMOOTH);
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  ShadeMode = DIFFUSE;
  Color = MAT_WHITE;
  glShadeModel(GL_SMOOTH);
  // local lighting, consider position of light in specular shading
  glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

  // position light and turn it on
  glEnable(GL_LIGHT0);

  // set up camera
  // parameters are eye point, aim point, up vector
  camera = new Camera(Vector3d(0, 5, 30), Vector3d(0, 5, 0), Vector3d(0, 1, 0));
  Plane pl;
  pl.point = Vector3d(0,0,0);
  pl.norm = Vector3d(0,1,0);
  pc = new SpringMesh(10, 10.0, 10.0, 20, pl);

  // ADDING STUFF
  Particle p;
  p.vel = Vector3d(0, 0, 0);
  p.radius = 3.5;
  p.mass = 5.0;
  p.pos = Vector3d(0.0, 15.0, 0.0);
  pc->add_particle(p);

  if (lighting) {
    glEnable (GL_LIGHTING);
    glClearColor(0, 0, 0, 0);


    // local lighting, consider position of light in specular shading
    glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

    // position light and turn it on
    glEnable(GL_LIGHT0);


  }
  else {
    // grey background for window
    glClearColor(0.2, 0.2, 0.2, 0.0);
    glDepthRange(0.0, 1.0);
  }

  glDepthFunc(GL_LESS);
  glEnable(GL_NORMALIZE);

  glEnable(GL_DEPTH_TEST);

  float deltaT = pc->params().deltaT;
  TIMERMSECS = 1000 * deltaT;
  TimerDelay = int(0.5 * deltaT * 1000);



}

void doLighting() {
  float ambient_color[4];
  float diffuse_color[4];
  float specular_color[4];
  int shininess = 0;

  // distant light source, parallel rays coming from front upper right
  const float light_position1[] = {1, 1, 1, 0};

  glLightfv(GL_LIGHT0, GL_POSITION, light_position1);
  glLightfv(GL_LIGHT0, GL_AMBIENT, WHITE);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, WHITE);
  glLightfv(GL_LIGHT0, GL_SPECULAR, WHITE);

  for(int i = 0; i < 3; i++) {
    ambient_color[i] = diffuse_color[i] = specular_color[i] = 0;
  }

  ambient_color[3] = diffuse_color[3] = specular_color[3] = 1;
  shininess = 1;



  switch(ShadeMode){
    case AMBIENT:
    for(int i = 0; i < 3; i++) {
      ambient_color[i] = AMBIENT_FRACTION * 3 * hues[Color][i];
    }
    break;

    case DIFFUSE:
    for(int i = 0; i < 3; i++){
      ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
      diffuse_color[i] = DIFFUSE_FRACTION * 2 * hues[Color][i];
    }
    break;

    case SPEC_BROAD:
    for(int i = 0; i < 3; i++){
      ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
      specular_color[i] = SPECULAR_FRACTION * 2 * hues[MAT_WHITE][i];
      shininess = 10;
    }
    break;

    case SPEC_TIGHT:
    for(int i = 0; i < 3; i++){
      ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
      specular_color[i] = SPECULAR_FRACTION * 2 *hues[MAT_WHITE][i];
      shininess = 60;
    }
    break;

    case ALL:
    for(int i = 0; i < 3; i++){
      ambient_color[i] = AMBIENT_FRACTION * hues[Color][i];
      diffuse_color[i] = DIFFUSE_FRACTION * hues[Color][i];
      specular_color[i] = SPECULAR_FRACTION * hues[MAT_WHITE][i];
      shininess = 60;
    }
    break;

  }
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ambient_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse_color);
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular_color);
  glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, shininess);
}

void draw_plane(Plane &plane) {
  Vector3d tx = (plane.norm % plane.point).normalize();
  Vector3d ty = (plane.norm % tx).normalize();
  Vector3d new_norm = tx % ty;
  Vector3d p = plane.point;
  Vector3d fu = tx * plane.width / 2.0;
  Vector3d fv = ty * plane.height / 2.0;
  Vector3d v[4];
  v[0] = p + fu - fv;
  v[1] = p - fu - fv;
  v[2] = p - fu + fv;
  v[3] = p + fu + fv;

  //glColor4f(1.0f,1.0f,0.0f,1.0f);
  // DRAWING
  glBegin(GL_TRIANGLES);
  glNormal3f(new_norm[0], new_norm[1], new_norm[2]);

  glVertex3f(v[2][0], v[2][1], v[2][2]);
  glVertex3f(v[1][0], v[1][1], v[1][2]);
  glVertex3f(v[0][0], v[0][1], v[0][2]);

  glVertex3f(v[3][0], v[3][1], v[3][2]);
  glVertex3f(v[2][0], v[2][1], v[2][2]);
  glVertex3f(v[0][0], v[0][1], v[0][2]);

  glEnd();

}


void PerspDisplay() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glLoadIdentity();

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  camera->PerspectiveDisplay(WIDTH, HEIGHT);


  if (showGrid) {
    makeGrid();
  }

  if (lighting) {
    glEnable(GL_LIGHTING);
    doLighting();
  }
  else {
    glDisable(GL_LIGHTING);
  }
  // push matrix onto transformation stack
  glPushMatrix();
  glTranslatef(0,0,0);
  // Shade the particles cyan in non-lighting mode
  glColor4f(0.0f, 1.0f, 1.0f,1.0f);

  std::vector<Particle> particles = pc->particles();
  /*  for(int i = 0; i < particles.size(); i++) {
  // glColor4f(particles[i].color[0], particles[i].color[1], particles[i].color[2],particles[i].color[3]);
  glPushMatrix();
  glTranslatef(particles[i].pos[0],particles[i].pos[1],particles[i].pos[2]);
  glutSolidSphere(particles[i].radius, 64, 64);
  glPopMatrix();
  //glTranslatef(-1*particles[i].pos[0],-1*particles[i].pos[1],-1*particles[i].pos[2]);
}*/
glPushMatrix();
glTranslatef(particles[particles.size()-1].pos[0],particles[particles.size()-1].pos[1],particles[particles.size()-1].pos[2]);
glutSolidSphere(particles[particles.size()-1].radius, 64, 64);
glPopMatrix();

glColor4f(1.0f, 0.0f, 1.0f,0.5f);
glLineWidth(1.5);
std::vector<Strut*> struts = pc->struts();
for(int i = 0; i < struts.size(); i++) {
  //std::cout << struts[i].a->pos[0] << struts[i].a->pos[1] << struts[i].a->pos[2] << std::endl;
  glColor4f(1.0,0.0,1.0, 1.0);
  glBegin(GL_LINES);
  glVertex3f(struts[i]->a->pos[0], struts[i]->a->pos[1], struts[i]->a->pos[2]);
  glVertex3f(struts[i]->b->pos[0], struts[i]->b->pos[1], struts[i]->b->pos[2]);
  glEnd();
}
//glLoadIdentity();
glutSwapBuffers();
}

// PROXY TORUN PARTICLE SIMULATOR THROUGH
void disp_sim() {
  if(!paused) {
    pc->sim();
  }
  PerspDisplay();
}


void resizeWindow(int w, int h) {
  if (h == 0) {
    h = 1;
  }
  float ratio = 1.0 * w/h;

  glMatrixMode(GL_PROJECTION);

  glLoadIdentity();
  glViewport(0,0,w,h);
  gluPerspective(45, ratio, 1, 1000);
  glMatrixMode(GL_MODELVIEW);

}
void mouseEventHandler(int button, int state, int x, int y) {
  // let the camera handle some specific mouse events (similar to maya)
  camera->HandleMouseEvent(button, state, x, y);
  glutPostRedisplay();
}

void motionEventHandler(int x, int y) {
  // let the camera handle some mouse motions if the camera is to be moved
  camera->HandleMouseMotion(x, y);
  glutPostRedisplay();
}

void arrowKeys(int key, int x, int y) {
  switch(key) {
    case GLUT_KEY_LEFT:
    break;
    case GLUT_KEY_RIGHT:
    break;
    case GLUT_KEY_UP:
    break;
    case GLUT_KEY_DOWN:
    break;
  }
}
void keyboardEventHandler(unsigned char key, int x, int y) {
  switch (key) {
    case 'r': case 'R':
    // reset the camera to its initial position
    camera->Reset();
    break;
    case 'f': case 'F':
    camera->SetCenterOfFocus(Vector3d(0, 0, 0));
    break;
    case 'g': case 'G':
    showGrid = !showGrid;
    break;

    case 'b': case 'B':
    blend = !blend;
    if (!blend) {
      glEnable(GL_BLEND);
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    }
    else {
      glDisable(GL_BLEND);
    }
    break;


    case 'h':			// H -- cycle through shading modes
    case 'H':
    ShadeMode = (ShadeMode == ALL? AMBIENT: ShadeMode + 1);
    switch(ShadeMode){
      case AMBIENT:
      cout << "Ambient" << endl;
      break;
      case DIFFUSE:
      cout << "Diffuse" << endl;
      break;
      case SPEC_BROAD:
      cout << "Specular - broad" << endl;
      break;
      case SPEC_TIGHT:
      cout << "Specular - tight" << endl;
      break;
      case ALL:
      cout << "A + D + S" << endl;
      break;
    }
    glutPostRedisplay();
    break;

    case 'm':			// M -- cycle through material colors
    case 'M':
    Color = (Color == MAT_VIOLET? MAT_WHITE: Color + 1);
    glutPostRedisplay();
    break;


    case 'l': case 'L':
    if (lighting) {
      lighting = false;
      glDisable(GL_LIGHTING);
      glDisable(GL_LIGHT0);
      glClearColor(0.62, 0.62, 0.62, 0.0);
    }
    else {
      lighting = true;
      glEnable(GL_LIGHTING);
      glClearColor(0, 0, 0, 0);

      // local lighting, consider position of light in specular shading
      glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, GL_TRUE);

      // position light and turn it on
      glEnable(GL_LIGHT0);

    }
    break;
    case 'p': case 'P':
    paused = !(paused);
    break;
    case 'q': case 'Q':	// q or esc - quit
    case 27:		// esc
    exit(0);
  }

  glutPostRedisplay();
}


void idle(int value) {
  glutTimerFunc(TIMERMSECS, idle, 0);
  float time;
  time = glutGet(GLUT_ELAPSED_TIME)/1000;
  disp_sim();
}

int main(int argc, char *argv[]) {
  // set up opengl window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DEPTH | GLUT_DOUBLE);
  glutInitWindowSize(WIDTH, HEIGHT);
  glutInitWindowPosition(50, 50);
  persp_win = glutCreateWindow("Particle Simulation");


  init();

  glutTimerFunc(TimerDelay, idle, 0);

  // set up opengl callback functions
  glutDisplayFunc(disp_sim);
  glutReshapeFunc(resizeWindow);
  glutMouseFunc(mouseEventHandler);
  glutMotionFunc(motionEventHandler);
  glutKeyboardFunc(keyboardEventHandler);
  glutSpecialFunc(arrowKeys);

  glutMainLoop();
  delete camera;
  delete pc;
  return(0);
}
