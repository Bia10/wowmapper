#include "glview_c.h"

Camera_c GlView_c::camera_(glm::vec3(19487.4f, 53.9951f, 26324.9f));
uint32_t GlView_c::indices_;

GlView_c::GlView_c(int32_t width, int32_t height, const char *title)
    : win_id_(0),
      width_(width),
      height_(height),
      title_(title) {
}

GlView_c::~GlView_c() {
  glutDestroyWindow(win_id_);
}

void GlView_c::Initialize(uint32_t indices) {
  indices_ = indices;

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
  glShadeModel(GL_SMOOTH);

  // Create light components
  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f, 0.5f };
  GLfloat diffuseLight[] = { 0.8f, 0.8f, 0.8, 0.5f };
  GLfloat specularLight[] = { 0.5f, 0.5f, 0.5f, 0.2f };
  GLfloat position[] = { 100.0f, 10000.0f, 0, 1.0f };

  // Assign created components to GL_LIGHT0
  glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  //glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);
  glLightfv(GL_LIGHT0, GL_POSITION, position);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);

  glEnable(GL_DEPTH_TEST);
  glDepthFunc(GL_LEQUAL);
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

  //GLfloat lpos[] = {pos.x, pos.y, pos.z, 1.0f};

  //glLightfv(GL_LIGHT0, GL_POSITION, lpos);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);

  glDrawElements(GL_TRIANGLES, indices_, GL_UNSIGNED_INT, NULL);

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
  glVertexPointer(3, GL_FLOAT, 0, NULL);

  uint32_t vbo_norm_id;
  glGenBuffersARB(1, &vbo_norm_id);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_norm_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, normals->size()*3*sizeof(float),
      normals->data(), GL_STATIC_DRAW_ARB);
  glNormalPointer(GL_FLOAT, 0, NULL);

  uint32_t vbo_idx_id;
  glGenBuffersARB(1, &vbo_idx_id);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_idx_id);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indices->size()*sizeof(uint32_t),
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
  static float speed = 10.0f;

  switch(key) {
    case 'w': {
      camera_.Walk(+speed);
      break; }
    case 's': {
      camera_.Walk(-speed);
      break; }
    case 'a': {
      camera_.Strafe(-speed);
      break; }
    case 'd': {
      camera_.Strafe(+speed);
      break; }
    case 'h': {
      speed = 1.0f;
      break; }
    case 'j': {
      speed = 10.0f;
      break; }
    case 'k': {
      speed = 50.0f;
      break; }

  }
}
