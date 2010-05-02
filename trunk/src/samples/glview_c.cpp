#include "glview_c.h"

Camera_c GlView_c::camera_(glm::vec3(0, 0, 0));
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
  // Create light components
  GLfloat ambientLight[] = { 0.2f, 0.2f, 0.2f };
  GLfloat diffuseLight[] = { 0.5f, 0.5f, 0.5f };
  GLfloat specularLight[] = { 1.0f, 1.0f, 1.0f };

  // Assign created components to GL_LIGHT0
  /*glLightfv(GL_LIGHT0, GL_AMBIENT, ambientLight);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuseLight);
  glLightfv(GL_LIGHT0, GL_SPECULAR, specularLight);

  glEnable(GL_LIGHTING);
  glEnable(GL_LIGHT0);*/

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
  glEnable(GL_COLOR_MATERIAL);
  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glEnable(GL_BLEND);
  glEnable(GL_NORMALIZE);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glClearColor(0, 0, 0.1, 1.0f);
}

void GlView_c::Render() {
  // buffer clear + pre-render setup
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluPerspective(60, 4/3.0, 1.0, 100000.0);

  const glm::vec3 &pos = camera_.position();
  const glm::vec3 &look = camera_.lookat();
  gluLookAt(pos.x, pos.y, pos.z, look.x, look.y, look.z, 0, 1, 0);

  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glEnableClientState(GL_COLOR_ARRAY);
  //glEnableClientState(GL_INDEX_ARRAY);

  glDrawElements(GL_TRIANGLES, indices_, GL_UNSIGNED_INT, NULL);

  glDisableClientState(GL_VERTEX_ARRAY);
  glDisableClientState(GL_NORMAL_ARRAY);
  glDisableClientState(GL_COLOR_ARRAY);
  //glDisableClientState(GL_INDEX_ARRAY);

  glutSwapBuffers();
}

void GlView_c::SetBuffers(const Indices32_t *indices,
                          const Vertices_t *vertices,
                          const Normals_t *normals,
                          const Colors_t *colors) {
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

  uint32_t vbo_col_id;
  glGenBuffersARB(1, &vbo_col_id);
  glBindBufferARB(GL_ARRAY_BUFFER_ARB, vbo_col_id);
  glBufferDataARB(GL_ARRAY_BUFFER_ARB, vertices->size()*sizeof(int),
        colors->data(), GL_STATIC_DRAW_ARB);
  glColorPointer(4, GL_UNSIGNED_BYTE, 0, NULL);

  uint32_t vbo_idx_id;
  glGenBuffersARB(1, &vbo_idx_id);
  glBindBufferARB(GL_ELEMENT_ARRAY_BUFFER_ARB, vbo_idx_id);
  glBufferDataARB(GL_ELEMENT_ARRAY_BUFFER_ARB, indices->size()*sizeof(uint32_t),
      indices->data(), GL_STATIC_DRAW_ARB);
  glIndexPointer(GL_UNSIGNED_INT, 0, NULL);
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
    case 'q': {
      camera_.SwimUp(-speed);
      break; }
    case 'e': {
      camera_.SwimUp(+speed);
      break; }
    case 'h': {
      speed = 0.5f;
      break; }
    case 'j': {
      speed = 10.0f;
      break; }
    case 'k': {
      speed = 50.0f;
      break; }

  }
}
