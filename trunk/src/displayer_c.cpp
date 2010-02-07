#include "displayer_c.h"
#include <GL/freeglut.h>
#include <iostream>

DisplayCallbacks_t Displayer_c::display_callbacks_;
Camera_c Displayer_c::camera_;
glm::vec2 Displayer_c::mouse_pos_(0.0f, 0.0f);
const vertex3f *Displayer_c::map_patches_ = NULL;
const vertex3f *Displayer_c::map_normals_ = NULL;

Displayer_c::Displayer_c(int32_t width, int32_t height, const char *title)
    : win_id_(0),
      width_(width),
      height_(height) {

  glutInit(&win_id_, NULL);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(480, 360);

  win_id_ = glutCreateWindow(title);
  glutDisplayFunc(Displayer_c::RenderLoop);
  glutIdleFunc(Displayer_c::RenderLoop);

  /* set keyboard and mouse callbacks */
  glutPassiveMotionFunc(Displayer_c::PassiveMouseCallback);
  glutMotionFunc(Displayer_c::MouseCallback);
  glutKeyboardFunc(Displayer_c::KeyboardCallback);

  //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  glShadeModel(GL_SMOOTH);
  //glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

  GLfloat LightAmbient[]= { 0.5f, 0.5f, 0.5f, 1.0f };
  GLfloat LightDiffuse[]= { 1.0f, 1.0f, 1.0f, 1.0f };
  GLfloat LightPosition[]= { 0, 200.0f, 0, 1.0f };
  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);
  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  //glEnable(GL_NORMALIZE);
}

Displayer_c::~Displayer_c() {
  glutDestroyWindow(win_id_);
}

void Displayer_c::Start(const vertex3f *mapPatches, const vertex3f *mapNormals) {
  map_patches_ = mapPatches;
  map_normals_ = mapNormals;

  glutMainLoop();
}

void Displayer_c::RenderLoop() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluPerspective(60, 4/3.0f, 1.0, 1000.0);

  /* set camera position and rotation */
  glm::vec3 &cam_pos = camera_.position();
  glm::vec2 &cam_rot = camera_.rotation();

  glTranslatef(cam_pos.x, cam_pos.y, cam_pos.z);
  glRotatef(cam_rot.y, 1, 0, 0);
  glRotatef(cam_rot.x, 0, 1, 0);

  glBegin(GL_TRIANGLES);
    for(int i = 0; i < 64*4*256; i++) {
      glNormal3f(map_normals_[i].x, map_normals_[i].y, map_normals_[i].z);
      glVertex3f(map_patches_[i*3].x, map_patches_[i*3].y, map_patches_[i*3].z);
      glVertex3f(map_patches_[i*3+1].x, map_patches_[i*3+1].y, map_patches_[i*3+1].z);
      glVertex3f(map_patches_[i*3+2].x, map_patches_[i*3+2].y, map_patches_[i*3+2].z);
    }
  glEnd();

  glutSwapBuffers();
}

void Displayer_c::PassiveMouseCallback(int32_t x, int32_t y) {
  mouse_pos_ = glm::vec2(static_cast<float>(x), static_cast<float>(y));
}

void Displayer_c::MouseCallback(int32_t x, int32_t y) {
  glm::vec2 new_pos(static_cast<float>(x), static_cast<float>(y));
  glm::vec2 distance = new_pos - mouse_pos_;
  camera_.rotation() += (distance * 0.5f);
  mouse_pos_ = new_pos;
}

void Displayer_c::KeyboardCallback(uint8_t key, int x, int y) {
  const float speed = 0.5f;

  /* key control */
  switch(key) {
    case 'q': {
      camera_.position().y -= speed;
      break; }
    case 'e': {
      camera_.position().y += speed;
      break; }
    case 'a': {
      camera_.position().x += speed;
      break; }
    case 'd': {
      camera_.position().x -= speed;
      break; }
    case 'w': {
      camera_.position().z += speed;
      break; }
    case 's': {
      camera_.position().z -= speed;
      break; }
    case 'l': {
      glEnable(GL_LIGHTING);
      break; }
  }
}
