/*

===== GlPainter.cpp ========================================================

*/

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <mx.h>

#include "shared.h"
#include "server.h"
#include "client.h"
#include "SDWindow.h"
#include "RCWindow.h"

#include "action.h"

#ifdef _SOFTGPIO
#ifdef __cplusplus
extern "C"
{
    namespace {
#endif
extern void delay(unsigned int howLong);
#ifdef __cplusplus
    } //namespace
}
#endif
#else
#include <wiringPi.h>
#endif //_SOFTGPIO

#include "GlPainter.h"

int glpainter_main(){
	//
	// make sure, we start in the right directory
	//

    for(; !flag_server_ready || !flag_client_ready;)
        delay(BAUD_RATE);

//	mx_setcwd (mx::getApplicationPath ());      //fix me

	//mx::setDisplayMode (0, 0, 0);
	mx::init (0, 0);

    g_SDWindow = new SDWindow ();
	g_SDWindow->setMenuBar (g_SDWindow->getMenuBar ());
	g_SDWindow->setBounds (20, 20, 640, 540);
	g_SDWindow->setVisible (true);

    g_RCWindow = new RCWindow ();
//	g_RCWindow->setMenuBar (g_RCWindow->getMenuBar ());
	g_RCWindow->setBounds (120, 20, 640, 540);
	g_RCWindow->setVisible (true);

	int ret = mx::run ();

	mx::cleanup ();

	return ret;

}

