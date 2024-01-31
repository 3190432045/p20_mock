/*
//
//
//
// file: RCWindow.h
// last modified:
// copyright:
// version:
//
// email:
// web:
//
*/

#ifndef INCLUDED_RCWINDOW
#define INCLUDED_RCWINDOW



#ifndef INCLUDED_MXWINDOW
#include <mx/mxWindow.h>
#endif

//#include "CmdLine.h"

#define IDC_FILE_EXIT			1017

#define IDC_ACTION_CONNECT		1101
#define IDC_ACTION_DISCONNECT   1102
#define IDC_ACTION_INFO         1103
#define IDC_ACTION_UNDO         1104
#define IDC_ACTION_REDO         1105
#define IDC_ACTION_CLS          1106
#define IDC_ACTION_REFRESH      1107

#define IDC_HELP_GOTOHOMEPAGE	1301
#define IDC_HELP_ABOUT			1302

//for window
#define X_RESOLUTION 640
#define Y_RESOLUTION 540


class mxMenuBar;
class GlWindow;


class RCWindow : public mxWindow
{
public:
	// CREATORS
	RCWindow ();
	~RCWindow ();

	// MANIPULATORS
	virtual int handleEvent (mxEvent *event);
//	void checkboxSet( int id, bool bState );
	void redraw ();

	// ACCESSORS
	mxMenuBar *getMenuBar () const { return mb; }
	GlWindow *getGlWindow () const { return d_GlWindow; }

private:
    mxMenuBar *mb;
	GlWindow *d_GlWindow;
//    line_t PrevLine;
//    drawlines_t Buffer;
};

extern RCWindow *g_RCWindow;
//extern char g_appTitle[];

#endif // INCLUDED_MDLVIEWER
