#include "displayer_c.h"
#include <GL/freeglut.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/type_ptr.hpp>

DisplayCallbacks_t Displayer_c::display_callbacks_;
Camera_c Displayer_c::camera_;
glm::vec2 Displayer_c::mouse_pos_(0.0f, 0.0f);
const glm::vec3 *Displayer_c::map_patches_ = NULL;
const glm::vec3 *Displayer_c::map_normals_ = NULL;

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
  GLfloat LightPosition[]= { 0, 500.0f, 0, 1.0f };
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

void Displayer_c::Start(const glm::vec3 *mapPatches, const glm::vec3 *mapNormals) {
  map_patches_ = mapPatches;
  map_normals_ = mapNormals;

  glutMainLoop();
}

void Displayer_c::RenderLoop() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluPerspective(60, 4/3.0f, 1.0, 1000.0);

  glm::vec3 curr_pos = camera_.position();
  glm::vec3 look_at = curr_pos + camera_.direction();
  gluLookAt(curr_pos.x, curr_pos.y, curr_pos.z,
            look_at.x, look_at.y, look_at.z,
            0.0f, 1.0f, 0.0f);


  glBegin(GL_TRIANGLES);
    for(int i = 0; i < 3*4*64*256; i++) {
      glNormal3f(map_normals_[i].x, map_normals_[i].y, map_normals_[i].z);
      glVertex3f(map_patches_[i].x, map_patches_[i].y, map_patches_[i].z);
      //glVertex3f(map_patches_[i*3+1].x, map_patches_[i*3+1].y, map_patches_[i*3+1].z);
      //glVertex3f(map_patches_[i*3+2].x, map_patches_[i*3+2].y, map_patches_[i*3+2].z);
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

  glm::mat4 rot4;
  rot4 = glm::rotate(rot4, camera_.rotation().y, glm::vec3(1, 0, 0));
  rot4 = glm::rotate(rot4, camera_.rotation().x, glm::vec3(0, 1, 0));
  glm::mat3 rot(rot4);

  camera_.direction() = (glm::vec3(0, 0, 1) * rot);

  mouse_pos_ = new_pos;
}

void Displayer_c::KeyboardCallback(uint8_t key, int x, int y) {
  glm::vec3 &cam_dir = camera_.direction();
  glm::vec3 &cam_pos = camera_.position();
  const float speed = 5.0f;

  /* key control */
  switch(key) {
    case 'q': {
      camera_.position().y += speed;
      break; }
    case 'e': {
      camera_.position().y -= speed;
      break; }
    case 'a': {
      cam_pos -= glm::vec3(-cam_dir.z, 0, cam_dir.x) * speed;
      break; }
    case 'd': {
      cam_pos += glm::vec3(-cam_dir.z, 0, cam_dir.x) * speed;
      break; }
    case 'w': {
      cam_pos += cam_dir * speed;
      break; }
    case 's': {
      cam_pos -= cam_dir * speed;
      break; }
  }
}
