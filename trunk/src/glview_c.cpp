#include "glview_c.h"

Camera_c GlView_c::camera_(glm::vec3(0.0f, 0.0f, 100.0f));

GlView_c::GlView_c(int32_t width, int32_t height, const char *title)
    : win_id_(0),
      width_(width),
      height_(height),
      title_(title) {
}

GlView_c::~GlView_c() {
  glutDestroyWindow(win_id_);
}

void GlView_c::Initialize() {
  // initialize glut and display mode
  glutInit(&win_id_, NULL);
  glutInitDisplayMode(GLUT_DEPTH | GLUT_RGB | GLUT_DOUBLE);

  glutInitWindowPosition(100, 100);
  glutInitWindowSize(width_, height_);

  // create window
  win_id_ = glutCreateWindow(title_);

  if (glewInit() != GLEW_OK) {
    std::cout << "GLEW error." << std::endl;
  }

  SetCallbacks();
  SetLight();
  SetGlCapabilities();
}

void GlView_c::SetLight() {
  GLfloat LightAmbient[]= { 0.4f, 0.4f, 0.2f, 0.5f };
  GLfloat LightDiffuse[]= { 0.5f, 0.5f, 0.5f, 0.5f };
  GLfloat LightPosition[]= { 1000.0f, 1000.0f, -1000.0f, 0.1f };

  glLightfv(GL_LIGHT1, GL_AMBIENT, LightAmbient);
  glLightfv(GL_LIGHT1, GL_DIFFUSE, LightDiffuse);
  glLightfv(GL_LIGHT1, GL_POSITION, LightPosition);
}

void GlView_c::SetCallbacks() {
  // set render callbacks
  glutDisplayFunc(GlView_c::Render);
  glutIdleFunc(GlView_c::Render);

  // set input callbacks
  glutPassiveMotionFunc(GlView_c::MouseUpCallback);
  glutMotionFunc(GlView_c::MouseDownCallback);
  glutKeyboardFunc(GlView_c::KeyboardCallback);
}

void GlView_c::SetGlCapabilities() {
  glShadeModel(GL_SMOOTH);

  glEnable(GL_LIGHT1);
  glEnable(GL_LIGHTING);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
  glClearColor(0, 0, 0.1, 1.0f);
}

void GlView_c::Render() {
  // buffer clear + pre-render setup
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluPerspective(60, 4/3.0f, 1.0, 100000.0);

  const glm::vec3 &pos = camera_.position();
  const glm::vec3 &look = camera_.lookat();
  gluLookAt(pos.x, pos.y, pos.z, look.x, look.y, look.z, 0, 1, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glVertexPointer(3, GL_FLOAT, 0, NULL);
  glNormalPointer(GL_FLOAT, 0, NULL);
  glDrawElements(GL_TRIANGLES, 196608, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);

  glutSwapBuffers();
}

void GlView_c::SetBuffers(const Points_t *vertices,
                          const Points_t *normals,
                          const Indices32_t *indices) {
  uint32_t vbo_vtx_id;
  // generate and bind buffer
  glGenBuffersARB(1, &vbo_vtx_id);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_vtx_id);

  // copy buffer data to video memory
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices->size()*3*sizeof(float),
      vertices->data(), GL_STATIC_DRAW_ARB);

  uint32_t vbo_norm_id;
  glGenBuffersARB(1, &vbo_norm_id);
  glBindBufferARB(GL_NORMAL_ARRAY_BUFFER_BINDING_ARB, vbo_norm_id);

  glBufferDataARB(GL_NORMAL_ARRAY_BUFFER_BINDING_ARB, normals->size()*3*sizeof(float),
      normals->data(), GL_STATIC_DRAW_ARB);

  uint32_t vbo_idx_id;
  glGenBuffersARB(1, &vbo_idx_id);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_idx_id);

  // copy buffer data to video memory
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indices->size()*sizeof(uint16_t),
      indices->data(), GL_STATIC_DRAW_ARB);
}

void GlView_c::MouseUpCallback(int32_t x, int32_t y) {
  camera_.SetMousePos(x, y);
}

void GlView_c::MouseDownCallback(int32_t x, int32_t y) {
  camera_.Rotate(x, y);
  camera_.SetMousePos(x, y);
}

void GlView_c::KeyboardCallback(uint8_t key, int x, int y) {
  switch(key) {
    case 'w': {
      camera_.Walk(+1.0f);
      break; }
    case 's': {
      camera_.Walk(-1.0f);
      break; }
    case 'a': {
      camera_.Strafe(-1.0f);
      break; }
    case 'd': {
      camera_.Strafe(+1.0f);
      break; }
  }
}
