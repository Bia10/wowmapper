#pragma once

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "common.h"
#include "camera_c.h"

class GlView_c {
 public:
  GlView_c(int32_t width, int32_t height, const char *title);
  ~GlView_c();

  void Initialize();
  void SetBuffers(const Points_t *vertices, const Points_t *normals, const Indices32_t *indices);
  void Go() const { glutMainLoop(); }

 private:
  void SetLight();
  void SetCallbacks();
  void SetGlCapabilities();

  static void Render();
  static void MouseUpCallback(int32_t x, int32_t y);
  static void MouseDownCallback(int32_t x, int32_t y);
  static void KeyboardCallback(uint8_t key, int32_t x, int32_t y);

  int32_t win_id_;
  int32_t width_;
  int32_t height_;
  const char *title_;

  static Camera_c camera_;
};
