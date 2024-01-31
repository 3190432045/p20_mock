/*
//
//
//
// file: GlWindow.h
// last modified:
// copyright:
// version:
//
// email:
// web:
//
*/


#ifndef INCLUDED_GLWINDOW
#define INCLUDED_GLWINDOW



#ifndef INCLUDED_MXGLWINDOW
#include <mxGlWindow.h>
#endif

#include "CmdLine.h"

class GlWindow : public mxGlWindow //should be replaced with QOpenGLWidget in Qt
{
public:
	// CREATORS
	GlWindow (mxWindow *parent, int x, int y, int w, int h, const char *label, int style);
	~GlWindow ();

	// MANIPULATORS
	virtual int handleEvent (mxEvent *event);
	virtual void draw ();

	// ACCESSORS
    void InsertLine (const line_t &newline) { CmdLines.push_back(newline); redraw(); }
//    line_t FetchLine ( void ) const { return PrevLine; }
    void LineClear() ;
    void LineUndo () { if(!CmdLines.empty()) CmdLines.pop_back(); redraw(); }
private:
    drawlines_t CmdLines;
    drawlines_t Buffer;
//    line_t PrevLine;
    unsigned char x0;
    unsigned char y0;
    unsigned char x1;
    unsigned char y1;
};



extern GlWindow *g_GlWindow;



#endif // INCLUDED_GLWINDOW

