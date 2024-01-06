/*
 * Copyright 2020 - 2023 Rafał Jopek ( rafaljopek at hotmail com )
 */

#include "glfwcairo.h"

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// static
static void cursor_position_callback( GLFWwindow *window, double x, double y )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->cursorX = x;
   pApp->cursorY = y;
}

static void key_callback( GLFWwindow *window, int key, int scancode, int action, int mods )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->keyCode     = key;
   pApp->keyScancode = scancode;
   pApp->keyAction   = action;
   pApp->keyMods     = mods;
}

static void mouse_button_callback( GLFWwindow *window, int button, int action, int mods )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->mouseButton = button;
   pApp->mouseAction = action;
   pApp->mouseMods   = mods;
}

static void window_maximize_callback( GLFWwindow *window, int maximized )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->winMaximized = maximized;
}

static void window_size_callback( GLFWwindow *window, int width, int height )
{
   App *pApp = glfwGetWindowUserPointer( window );

   pApp->width  = width;
   pApp->height = height;
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void set_color_from_hex( cairo_t *cr, unsigned int hexColor )
{
   if( ( hexColor <= 0xffffff ) )
   {
      double r, g, b;
      r = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      b = (   hexColor         & 0xFF ) / 255.0;
      cairo_set_source_rgba( cr, r, g, b, 1.0 );
   }
   else if( ( hexColor <= 0xffffffff ) )
   {
      float r, g, b, a;
      r = ( ( hexColor >> 24 ) & 0xFF ) / 255.0;
      g = ( ( hexColor >> 16 ) & 0xFF ) / 255.0;
      b = ( ( hexColor >> 8 )  & 0xFF ) / 255.0;
      a = (   hexColor         & 0xFF ) / 255.0;
      cairo_set_source_rgba( cr, r, g, b, a );
   }
   else
   {
      fprintf( stderr, "An invalid hexadecimal color value was passed." );
   }
}

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
App *CreateWindow( int width, int height, const char *title )
{
   if( ! glfwInit() )
   {
      exit( EXIT_FAILURE );
   }

   App *pApp = malloc( sizeof( App ) );
   if( ! pApp )
   {
      fprintf( stderr, "Memory allocation failed for App structure.\n" );
      return NULL;
   }

   memset( pApp, 0, sizeof( App ) );

   pApp->width  = width;
   pApp->height = height;
   pApp->title  = title;
   pApp->background = 0x323232;

   glfwWindowHint( GLFW_CLIENT_API, GLFW_NO_API );

   pApp->window = glfwCreateWindow( pApp->width, pApp->height, pApp->title, NULL, NULL );
   if( ! pApp->window )
   {
      glfwTerminate();
      return NULL;
   }

   glfwMakeContextCurrent( pApp->window );
   glfwSetWindowUserPointer( pApp->window, pApp );

   glfwSetCursorPosCallback( pApp->window, cursor_position_callback );
   glfwSetKeyCallback( pApp->window, key_callback );
   glfwSetMouseButtonCallback( pApp->window, mouse_button_callback );
   glfwSetWindowMaximizeCallback( pApp->window, window_maximize_callback );
   glfwSetWindowSizeCallback( pApp->window, window_size_callback );

   glfwSetWindowSizeLimits( pApp->window, pApp->width, pApp->height, GLFW_DONT_CARE , GLFW_DONT_CARE );
   glfwSetCursorPos( pApp->window, pApp->width / 2, pApp->height / 2 );

   #if defined( GLFW_EXPOSE_NATIVE_WIN32 )
      pApp->dc = GetDC( glfwGetWin32Window( pApp->window ) );
      pApp->sf = cairo_win32_surface_create_with_format( pApp->dc, CAIRO_FORMAT_ARGB32 );
      pApp->cr = cairo_create( pApp->sf );
   #endif
   #if defined( GLFW_EXPOSE_NATIVE_X11 )
      pApp->xd = glfwGetX11Display();
      pApp->sf = cairo_xlib_surface_create( pApp->xd, glfwGetX11Window( pApp->window ), DefaultVisual( pApp->xd, DefaultScreen( pApp->xd ) ), pApp->width, pApp->height );
      pApp->cr = cairo_create( pApp->sf );
   #endif

   return pApp;
}

bool MainLoop( App *pApp )
{
   bool state;

   pApp->closeFlag = glfwWindowShouldClose( pApp->window );
   glfwSetWindowShouldClose( pApp->window, GLFW_FALSE );

   state = glfwGetWindowAttrib( pApp->window, GLFW_ICONIFIED );
   if( state == T )
   {
      while( state == T )
      {
         state = glfwGetWindowAttrib( pApp->window, GLFW_ICONIFIED );

         glfwWaitEventsTimeout( 0.5 );
      }
   }
   return pApp->closeFlag;
}

void ExitApp( App *pApp )
{
   if( pApp )
   {
      glfwSetWindowShouldClose( pApp->window, GLFW_TRUE );
   }
}

void BeginDrawing( App *pApp )
{
   glfwGetFramebufferSize( pApp->window, &pApp->width, &pApp->height );

   #if defined( GLFW_EXPOSE_NATIVE_WIN32 )
      if( pApp->tmpWidth != pApp->width || pApp->tmpHeight != pApp->height )
      {
         cairo_surface_destroy( pApp->sf );
         pApp->sf = cairo_win32_surface_create_with_format( pApp->dc, CAIRO_FORMAT_ARGB32 );
         cairo_destroy( pApp->cr );
         pApp->cr = cairo_create( pApp->sf );
         pApp->tmpWidth  = pApp->width;
         pApp->tmpHeight = pApp->height;
      }
   #endif
   #if defined( GLFW_EXPOSE_NATIVE_X11 )
      cairo_xlib_surface_set_size( pApp->sf, pApp->width, pApp->height );
   #endif

   cairo_push_group( pApp->cr );

   set_color_from_hex( pApp->cr, pApp->background );
   cairo_paint( pApp->cr );
}

void EndDrawing( const App *pApp )
{
   cairo_pop_group_to_source( pApp->cr );
   cairo_paint( pApp->cr );
   cairo_surface_flush( pApp->sf );
}

void Background( App *pApp, unsigned int color )
{
   pApp->background = color;
}

int CloseAll( App *pApp )
{
   if( pApp )
   {
      cairo_surface_destroy( pApp->sf );
      cairo_destroy( pApp->cr );
      glfwDestroyWindow( pApp->window );
      free( pApp );
      glfwTerminate();
      exit( EXIT_SUCCESS );
   }
   else
   {
      fprintf( stderr, "Error: CloseWindow called with NULL App pointer.\n" );
      exit( EXIT_FAILURE );
   }
}

void PollEvents()
{
   glfwPollEvents();
}

void WaitEvents()
{
   glfwWaitEvents();
}

void WaitEventsTimeout( double timeout )
{
   glfwWaitEventsTimeout( timeout );
}

void SetTargetFPS( App *pApp, int targetFPS )
{
   const double targetFrameTime = 1.0 / targetFPS;

   double currentTime = glfwGetTime();
   double frameTime = currentTime - pApp->previousTime;

   if( frameTime < targetFrameTime )
   {
      double sleepTime = targetFrameTime - frameTime;
      glfwWaitEventsTimeout( sleepTime );
   }

   pApp->previousTime = currentTime;
}
