/*
 * Copyright 2020 - 2024 Rafał Jopek ( rafaljopek at hotmail com )
 */

#ifndef GLFWCAIRO_H_
#define GLFWCAIRO_H_

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <cairo/cairo.h>

#if defined( GLFW_EXPOSE_NATIVE_WIN32 )
   #include <cairo/cairo-win32.h>
#endif
#if defined( GLFW_EXPOSE_NATIVE_X11 )
   #include <cairo/cairo-xlib.h>
#endif
#if defined( GLFW_EXPOSE_NATIVE_COCOA )
   #include <cairo/cairo-quartz.h>
#endif

#include <GLFW/glfw3.h>
#include <GLFW/glfw3native.h>

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// types
typedef enum bool
{
   F = 0,
   T = ( ! 0 )
} bool;

typedef struct _App App;

struct _App
{
   GLFWwindow  *window;
   int          width;
   int          height;
   const char  *title;
   bool         closeFlag;
   double       cursorX;
   double       cursorY;
   int          keyCode;
   int          keyScancode;
   int          keyAction;
   int          keyMods;
   int          mouseButton;
   int          mouseAction;
   int          mouseMods;
   int          winMaximized;
   unsigned int background;
   double       previousTime;
   int          tmpWidth;
   int          tmpHeight;

   cairo_surface_t     *sf;
   cairo_t             *cr;
#if defined( GLFW_EXPOSE_NATIVE_WIN32 )
   HDC dc;
#endif
#if defined( GLFW_EXPOSE_NATIVE_X11 )
   Display *xd;
#endif
};

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// API functions
App  *CreateWindow( int width, int height, const char *title );
bool  MainLoop( App *pApp );
void  ExitApp( App *pApp );
void  BeginDrawing( App *pApp );
void  EndDrawing( const App *pApp );
int   CloseAll( App *pApp );
void  Background( App *pApp, unsigned int color );
void  PollEvents();
void  WaitEvents();
void  WaitEventsTimeout( double timeout );
void  SetTargetFPS( App *pApp, int targetFPS );

/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// internal
void set_color_from_hex( cairo_t *cr, unsigned int hexColor );
/* =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= */
// macros
#define MAX( a, b ) ( ( a ) < ( b ) ? ( b ) : ( a ) )
#define MIN( a, b ) ( ( a ) < ( b ) ? ( a ) : ( b ) )
#define UNUSED( n ) ( ( void )( n ) )
#define LEN( n ) ( sizeof( n ) / sizeof( n )[ 0 ] )

#define BEGINDRAWING( pApp ) do { BeginDrawing( pApp )
#define ENDDRAWING( pApp ) EndDrawing( pApp ); } while( 0 )

#endif /* End GLFWCAIRO_H_ */
