/*  wowmapper - World of Warcraft MAP ParsER
    Copyright (C) 2010 PferdOne aka Flowerew

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>. */

#pragma once

#include <SDL.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../src/common.h"
#include "camera.h"

struct CustomVertex_s {
  D3DXVECTOR3 pos;
  D3DXVECTOR3 normal;
};
typedef std::vector<CustomVertex_s> CustomVertices_t;

/** Little helper class to ease rendering. **/
class Renderer {
 public:
  Renderer( uint32_t width, uint32_t height, const std::string &title );
  /** Initializes SDL and D3D . **/
  bool initialize();
  /** Called everytime after begin() to setup the scene. **/
  void initScene();
  /** Place render calls in between begin and end. **/
  void begin();
  void end();
  /** Create index and vertex buffer by the size given in the parameters. **/
  void createBuffers( size_t ib_size, size_t vb_size, DWORD format,
                      IDirect3DIndexBuffer9 **ib, IDirect3DVertexBuffer9 **vb ) const;
  /** Return IDirect3DDevice9. **/
  IDirect3DDevice9* getDevice() const;
  /** Returns camera. **/
  Camera& getCamera();

 private:
  bool initSDL();
  bool initDirect3D();
  void sdlPollEvents();
  void sdlHandleInputEvents( SDL_Event &event );
  /** Create directional lights with a direction and a diffuse color. **/
  void createDirectionalLight( const glm::vec3 &dir, const glm::vec3 &diff );

  uint32_t _width, _height;
  std::string _title;
  SDL_Surface *_surface;
  IDirect3DDevice9 *_d3dDev9;
  Camera _camera;
  int _lastMouseX, _lastMouseY;
  int _lightCounter;
};