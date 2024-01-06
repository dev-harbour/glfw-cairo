/*
 *
 */

#include "glfwcairo.h"

int main()
{
   App *app = CreateWindow( 830, 450, "Test window" );

   Background( app, 0xEAEAEA );

   while( ! MainLoop( app ) )
   {
      BEGINDRAWING( app );

      ENDDRAWING( app );
      WaitEvents();
   }

   CloseAll( app );
}
