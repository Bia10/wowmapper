#pragma once

#include "camera_c.h"
#include "adt/adt_c.h"
#include "wdt/wdt_c.h"

typedef std::vector<void (*)(void)> DisplayCallbacks_t;

class Displayer_c {
 public:
  Displayer_c(int32_t width, int32_t height, const char *title);
  ~Displayer_c();
  //void Start(AdtList_t *adtList);

  DisplayCallbacks_t& display_callbacks() { return display_callbacks_; }

 private:
  static void RenderLoop();
  static void PassiveMouseCallback(int32_t x, int32_t y);
  static void MouseCallback(int32_t x, int32_t y);
  static void KeyboardCallback(uint8_t key, int32_t x, int32_t y);

  static DisplayCallbacks_t display_callbacks_;
  static Camera_c camera_;
  static glm::vec2 mouse_pos_;
  //static const AdtList_t *adt_list_;

  int32_t win_id_;
  int32_t width_;
  int32_t height_;

  static glm::vec3 *vertices_;
  static glm::vec3 *normales_;
};
