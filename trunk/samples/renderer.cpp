#include "renderer.h"

//------------------------------------------------------------------------------
Renderer::Renderer( uint32_t width, uint32_t height, const std::string &title )
    : _width( width ),
      _height( height ),
      _title( title ),
      _surface( NULL ),
      _d3dDev9( NULL ),
      _camera( glm::vec3( 0, 100, 0 ), glm::vec3( 0, 0, 0 ) ),
      _lastMouseX( 0 ),
      _lastMouseY( 0 ),
      _lightCounter( 0 ) {
}

//------------------------------------------------------------------------------
bool Renderer::initialize() {
  return initSDL() && initDirect3D();
}

//------------------------------------------------------------------------------
void Renderer::initScene() {
  // set world matrix
  D3DXMATRIX mat_world;
  D3DXMatrixTranslation( &mat_world, 1066.67f, -134.377f, -8000.0f );
  _d3dDev9->SetTransform( D3DTS_WORLD, &mat_world );

  // create view matrix
  const glm::vec3 &pos = _camera.position();
  const glm::vec3 &look_at = _camera.lookAt();
  D3DXVECTOR3 eye_vec( pos.x, pos.y, pos.z );
  D3DXVECTOR3 la_vec( look_at.x, look_at.y, look_at.z );
  D3DXVECTOR3 up_vec( 0.0f, 1.0f, 0.0f );

  D3DXMATRIXA16 mat_view;
  D3DXMatrixLookAtRH( &mat_view, &eye_vec, &la_vec, &up_vec );
  _d3dDev9->SetTransform( D3DTS_VIEW, &mat_view );

  // and finaly the projection matrix
  D3DXMATRIX mat_proj;
  D3DXMatrixPerspectiveFovRH( &mat_proj, D3DX_PI/4, 1.0f, 1.0f, 100000.0f );
  _d3dDev9->SetTransform( D3DTS_PROJECTION, &mat_proj );
}

//------------------------------------------------------------------------------
void Renderer::begin() {
  sdlPollEvents();
  _d3dDev9->Clear( 0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER, 0, 1.0f, 0 );
  _d3dDev9->BeginScene();
}

//------------------------------------------------------------------------------
void Renderer::end() {
  _d3dDev9->EndScene();
  _d3dDev9->Present( NULL, NULL, NULL, NULL );
}

//------------------------------------------------------------------------------
IDirect3DDevice9* Renderer::getDevice() const {
  return _d3dDev9;
}

//------------------------------------------------------------------------------
bool Renderer::initSDL() {
  //const SDL_VideoInfo *info = SDL_GetVideoInfo();
  _surface = SDL_SetVideoMode( _width, _height, 16, 0 );
  if ( !_surface ) return false;

  if ( SDL_Init( SDL_INIT_VIDEO ) < 0 ) return false;
  if ( SDL_EnableKeyRepeat( 100, 10 ) < 0 ) return false;

  SDL_WM_SetCaption( _title.c_str(), NULL );

  return true;
}

//------------------------------------------------------------------------------
bool Renderer::initDirect3D() {
  IDirect3D9 *d3d9 = Direct3DCreate9( D3D_SDK_VERSION );
  
  // create present params
  D3DPRESENT_PARAMETERS present_params;
  ZeroMemory( &present_params, sizeof( D3DPRESENT_PARAMETERS ) );

  HWND active_window = GetActiveWindow();
  // minimal setup
  present_params.Windowed = true;
  present_params.hDeviceWindow = active_window;
  present_params.BackBufferFormat = D3DFMT_UNKNOWN;
  present_params.BackBufferCount = 1;
  present_params.SwapEffect = D3DSWAPEFFECT_DISCARD;
  present_params.EnableAutoDepthStencil = true;
  present_params.AutoDepthStencilFormat = D3DFMT_D16;
  //present_params.FullScreen_RefreshRateInHz = D3DPRESENT_INTERVAL_IMMEDIATE;

  d3d9->CreateDevice( D3DADAPTER_DEFAULT, D3DDEVTYPE_HAL, active_window,
                      D3DCREATE_HARDWARE_VERTEXPROCESSING,
                      &present_params, &_d3dDev9 );

  d3d9->Release();

  if ( !_d3dDev9 ) return false;

  // create a plain white material
  D3DMATERIAL9 material;
  ZeroMemory( &material, sizeof( D3DMATERIAL9 ) );
  material.Diffuse.r = material.Ambient.r = 1.0f;
  material.Diffuse.g = material.Ambient.g = 1.0f;
  material.Diffuse.b = material.Ambient.b = 1.0f;
  material.Diffuse.a = material.Ambient.a = 1.0f;
  _d3dDev9->SetMaterial( &material );

  // create directional lights
  createDirectionalLight( glm::vec3( -1, -2, 1 ), glm::vec3( 0.5f, 0.5f, 0.6f ) ); 
  createDirectionalLight( glm::vec3( 1, 2, 1 ), glm::vec3( 0.3f, 0.3f, 0.4f ) );

  // set render states
  _d3dDev9->SetRenderState( D3DRS_CULLMODE, D3DCULL_NONE );
  _d3dDev9->SetRenderState( D3DRS_LIGHTING, TRUE );
  _d3dDev9->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
  _d3dDev9->SetRenderState( D3DRS_AMBIENT, D3DCOLOR_XRGB( 32, 32, 32 ) );

  return true;
}

//------------------------------------------------------------------------------
void Renderer::createDirectionalLight( const glm::vec3 &dir, const glm::vec3 &diff ) {
  D3DLIGHT9 light;
  ZeroMemory( &light, sizeof( D3DLIGHT9 ) );
  
  light.Type = D3DLIGHT_DIRECTIONAL;
  light.Direction.x = dir.x;
  light.Direction.y = dir.y;
  light.Direction.z = dir.z;
  light.Diffuse.r = diff.r;
  light.Diffuse.g = diff.g;
  light.Diffuse.b = diff.b;

  _d3dDev9->SetLight( _lightCounter, &light );
  _d3dDev9->LightEnable( _lightCounter, TRUE );

  _lightCounter++;
}


//------------------------------------------------------------------------------
void Renderer::sdlPollEvents() {
  SDL_Event event;

  while( SDL_PollEvent( &event ) ) {
    switch( event.type ) {
      case SDL_QUIT: {
        exit( 1 );
        break; }
      case SDL_KEYDOWN:
      case SDL_MOUSEBUTTONDOWN:
      case SDL_MOUSEMOTION: {
        sdlHandleInputEvents( event );
        break; }
    }
  }
}

//------------------------------------------------------------------------------
void Renderer::sdlHandleInputEvents( SDL_Event &event ) {
  if ( event.button.button == SDL_BUTTON_LEFT ) {
    int temp_x, temp_y;
    SDL_GetMouseState( &temp_x, &temp_y );
    _camera.rotate( (_lastMouseX - temp_x), -(_lastMouseY - temp_y) );
  }

  static float speed = 100.0f;

  switch( event.key.keysym.sym ) {
    case SDLK_w: {
      _camera.move( speed );
      break; }
    case SDLK_s: {
      _camera.move( -speed );
      break; }
    case SDLK_r: {
      speed = 100.0f;
      break; }
    case SDLK_f: {
      speed = 50.0f;
      break; }
    case SDLK_v: {
      speed = 10.0f;
      break; }
  }

  SDL_GetMouseState( &_lastMouseX, &_lastMouseY );
}

//------------------------------------------------------------------------------
void Renderer::createBuffers( size_t ib_size, size_t vb_size, DWORD format,
                              IDirect3DIndexBuffer9 **ib, IDirect3DVertexBuffer9 **vb ) const {
  _d3dDev9->CreateIndexBuffer( ib_size, 0, D3DFMT_INDEX32, D3DPOOL_DEFAULT, ib, NULL ); 
  _d3dDev9->CreateVertexBuffer( vb_size, 0, format, D3DPOOL_DEFAULT, vb, NULL );
}

//------------------------------------------------------------------------------
Camera& Renderer::getCamera() {
  return _camera;
}