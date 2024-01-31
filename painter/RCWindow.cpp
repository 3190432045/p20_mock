/*
//
//
//
// file: RCWindow.cpp
// last modified:
// copyright:
// version:
//
// email:
// web:
//
*/

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <mx.h>
#include <gl.h>
#include "GlWindow.h"
//#include "CmdLine.h"
//#include "shared.h"
//#include "server.h"

#include "RCWindow.h"

/*
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
*/

RCWindow *g_RCWindow = 0;
//char g_appTitle[] = "Sender Window";

RCWindow :: RCWindow() : mxWindow( 0, 0, 0, 0, 0, "Receiver Window", mxWindow::Normal )
{
	// create menu stuff
	mb = new mxMenuBar (this);
	/*
	mxMenu *menuFile = new mxMenu ();
	mxMenu *menuAction = new mxMenu ();
//	mxMenu *menuTool = new mxMenu ();
	mxMenu *menuHelp = new mxMenu ();

	mb->addMenu ("File", menuFile);
	mb->addMenu ("Action", menuAction);
//	mb->addMenu ("Tools", menuTool);
	mb->addMenu ("Help", menuHelp);

//	menuFile->addSeparator ();
	menuFile->add ("Exit", IDC_FILE_EXIT);

//	menuAction->add ("Connect...", IDC_ACTION_CONNECT);
//	menuAction->add ("Disconnect...", IDC_ACTION_DISCONNECT);
	menuAction->add ("Display Information", IDC_ACTION_INFO);
	menuAction->addSeparator ();
	menuAction->add ("Undo", IDC_ACTION_UNDO);
//	menuAction->add ("Redo", IDC_ACTION_REDO);
	menuAction->addSeparator ();
	menuAction->add ("Clear Sceen", IDC_ACTION_CLS);
//	menuAction->add ("Refresh", IDC_ACTION_REFRESH);

	menuHelp->add ("About...", IDC_HELP_ABOUT);
    */
	// create the OpenGL window
	d_GlWindow = new GlWindow (this, 0, 0, 0, 0, "", mxWindow::Normal);
#ifdef WIN32
	SetWindowLong ((HWND) d_GlWindow->getHandle (), GWL_EXSTYLE, WS_EX_CLIENTEDGE);
#endif

	g_GlWindow = d_GlWindow;

//	line_t emptyline = {0,0,0,0};
//	cmd_lines.assign(32, emptyline);

	setBounds (20, 20, 640, 540);
	setVisible (true);
	g_GlWindow->setEnabled(false);
}



RCWindow::~RCWindow ()
{
	// grab some params in case that hasn't updates

#ifdef WIN32
	DeleteFile ("midump.txt");
#endif
}
/*
void RCWindow :: checkboxSet( int id, bool bState )
{
	mb->setChecked( id, bState );
}
*/
int
RCWindow::handleEvent (mxEvent *event)
{
	switch (event->event)
	{

	case mxEvent::Action:
	{
		switch (event->action)
		{
		case IDC_FILE_EXIT:
		{
			redraw ();
			mx::quit ();
		}
		break;

		case IDC_ACTION_CLS:
            getGlWindow()->LineClear();
            break;

        case IDC_ACTION_UNDO:
            getGlWindow()->LineUndo();
            break;

		case IDC_HELP_ABOUT:
			mxMessageBox (this,
				"Painter P20\n"
				"Build:\t" __DATE__ ".\n"
				"Email:\tjw26u22@soton.ac.uk\n", "Painter",
				MX_MB_OK | MX_MB_INFORMATION );
			break;

		} //switch (event->action)

	} // mxEvent::Action
	break;

	case mxEvent::Size:
	{
		int w = event->width;
		int h = event->height;
		int y = mb->getHeight ();
#ifdef WIN32
#define HEIGHT 120
#else
#define HEIGHT 140
		h -= 40;
#endif

//		d_GlWindow->setBounds (0, y, w, h - HEIGHT);
		d_GlWindow->setBounds (0, y, w, h);
//		d_cpl->setBounds (0, y + h - HEIGHT, w, HEIGHT);
	}
	break;

	break;
	} // event->event

	return 1;
}



void
RCWindow::redraw ()
{
	mxEvent event;
	event.event = mxEvent::Size;
	event.width = w2 ();
	event.height = h2 ();
	handleEvent (&event);
}
