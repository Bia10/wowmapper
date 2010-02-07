#pragma once

#include <vector>
#include <stdint.h>
#include "camera_c.h"
#include "adt/adt_c.h"

typedef std::vector<void (*)(void)> DisplayCallbacks_t;

class Displayer_c {
 public:
  Displayer_c(int32_t width, int32_t height, const char *title);
  ~Displayer_c();
  void Start(const glm::vec3 *mapPatches, const glm::vec3 *mapNormals);

  DisplayCallbacks_t& display_callbacks() { return display_callbacks_; }

 private:
  static void RenderLoop();
  static void PassiveMouseCallback(int32_t x, int32_t y);
  static void MouseCallback(int32_t x, int32_t y);
  static void KeyboardCallback(uint8_t key, int32_t x, int32_t y);

  static DisplayCallbacks_t display_callbacks_;
  static Camera_c camera_;
  static const glm::vec3 *map_patches_;
  static const glm::vec3 *map_normals_;
  static glm::vec2 mouse_pos_;

  int32_t win_id_;
  int32_t width_;
  int32_t height_;
};
