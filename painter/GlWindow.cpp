/*
//
//
//
// file: GlWindow.cpp
// last modified:
// copyright:
// version:
//
// email:
// web:
//
*/

#include <mx.h>
#include <mxMessageBox.h>
//#include <mxTga.h>
//#include <mxPcx.h>
//#include <mxBmp.h>
#include <gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <cstdlib>
#include <ctime>
#include <cstring>

#include "CmdLine.h"
#include "shared.h"
#include "server.h"
#include "client.h"

#include "GlWindow.h"

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


extern char g_appTitle[];


GlWindow *g_GlWindow = 0;

GlWindow :: GlWindow( mxWindow *parent, int x, int y, int w, int h, const char *label, int style ) : mxGlWindow( parent, x, y, w, h, label, style )
{
//	glDepthFunc( GL_LEQUAL );

	if( !parent ) setVisible( true );
	else mx :: setIdleWindow ( this );

//	line_t line1 = {0,0,255,255};
//	CmdLines.assign(1, line1);
	    x0 = 0;
	    y0 = 0;
	    x1 = 0;
	    y1 = 0;
//    PrevLine.x0 = 0;
//    PrevLine.y0 = 0;
//    PrevLine.x1 = 0;
//    PrevLine.y1 = 0;

    setTimer( BAUD_RATE * 10 );
}

GlWindow :: ~GlWindow( void )
{
	mx::setIdleWindow( 0 );
}

int GlWindow :: handleEvent( mxEvent *event )
{
//	static float oldrx, oldry, oldtz, oldtx, oldty;
//	static float oldlx, oldly;
//	static int oldx, oldy;
//	static double lastupdate;

	switch( event->event )
	{
/*
    case mxEvent::ParentNotify:
    {
        char msg[MAX_MSG_LENGTH + 1];
        msg[MAX_MSG_LENGTH] = '\0';
        int i = 0;

        for(drawlines_t::iterator iter_buffer = Buffer.begin(); iter_buffer != Buffer.end() && i < MAX_MSG_LENGTH; ++iter_buffer, i+=4)
        {
        msg[i] = iter_buffer->x0;
        msg[i+1] = iter_buffer->y0;
        msg[i+2] = iter_buffer->x1;
        msg[i+3] = iter_buffer->y1;
        }

        msg[i] = '\0';

        if(Buffer.size() == MAX_MSG_LENGTH/4)
        {
            SV_insert_msg(msg);
            Buffer.clear();
        }
        else
            if(!SV_insert_msg_nb(msg))
                Buffer.clear();
    }
    break;
*/
    case mxEvent::Timer:
    {
			redraw();

            if(this->isEnabled())
            {
                if(Buffer.size() < MAX_MSG_LENGTH/4)
                    {
                    unsigned char msg[MAX_MSG_LENGTH + 1];
                    msg[MAX_MSG_LENGTH] = '\0';
                    int i = 0;

                    msg[MAX_MSG_LENGTH] = '\0';

                    for(drawlines_t::iterator iter_buffer = Buffer.begin(); iter_buffer != Buffer.end() && i < MAX_MSG_LENGTH; ++iter_buffer, i+=4)
                    {
                    msg[i] = iter_buffer->x0;
                    msg[i+1] = iter_buffer->y0;
                    msg[i+2] = iter_buffer->x1;
                    msg[i+3] = iter_buffer->y1;
                    }

                    msg[i] = '\0';

                    if(Buffer.size() == MAX_MSG_LENGTH/4)
                    {
                        SV_insert_msg(msg);
                        Buffer.clear();
                    }
                    else
                        if(!SV_insert_msg_nb(msg))
                            Buffer.clear();
                }
                else if(Buffer.size() > MAX_MSG_LENGTH/4)
                        Buffer.clear();
            }
            else if(!this->isEnabled())
            {
                char msg[MAX_MSG_LENGTH + 1];
                msg[MAX_MSG_LENGTH] = '\0';
                line_t NewLine = {0,0,0,0};

                if(!CL_get_msg(msg))
                {
                    for(int i=0; i < MAX_MSG_LENGTH; i+=4)
                    {
                        if(msg[i])
                            NewLine.x0 = msg[i];
                        else
                            break;
                        if(msg[i+1])
                            NewLine.y0 = msg[i+1];
                        else
                            break;
                        if(msg[i+2])
                            NewLine.x1 = msg[i+2];
                        else
                            break;
                        if(msg[i+3])
                            NewLine.y1 = msg[i+3];
                        else
                            break;

                        if(NewLine.x0 == 255 && NewLine.y0 == 255 && NewLine.x1 == 255 && NewLine.y1 == 255)
                            CmdLines.clear();
                        else
                            CmdLines.push_back(NewLine);

#ifdef _DEBUG
                        std::printf("RCWindow::newline: %d %d %d %d\n" ,NewLine.x0, NewLine.y0, NewLine.x1, NewLine.y1);
#endif // _DEBUG

                    }
                }
            }

		return 1;
    }
    break;

    /*
	case mxEvent::Idle:
	{

			redraw();
			delay(100);

//		prev = curr;

            if(!this->isEnabled())
            {
                char msg[MAX_MSG_LENGTH + 1];
                msg[MAX_MSG_LENGTH] = '\0';
                line_t NewLine = {0,0,0,0};

                if(!CL_get_msg(msg))
                {
                    for(int i=0; i < MAX_MSG_LENGTH; i+=4)
                    {
                        if(msg[i])
                            NewLine.x0 = msg[i];
                        else
                            break;
                        if(msg[i+1])
                            NewLine.y0 = msg[i+1];
                        else
                            break;
                        if(msg[i+2])
                            NewLine.x1 = msg[i+2];
                        else
                            break;
                        if(msg[i+3])
                            NewLine.y1 = msg[i+3];
                        else
                            break;

                        if(NewLine.x0 == 255 && NewLine.y0 == 255 && NewLine.x1 == 255 && NewLine.y1 == 255)
                            CmdLines.clear();
                        else
                            CmdLines.push_back(NewLine);

#ifdef _DEBUG
                        std::printf("RCWindow::newline: %d %d %d %d\n" ,NewLine.x0, NewLine.y0, NewLine.x1, NewLine.y1);
#endif // _DEBUG

                    }
                }
            }

		return 1;
	}
	break;
    */
	case mxEvent::MouseUp:
	{
//        char msg[MAX_MSG_LENGTH + 1];
//        int i = 0;
	    line_t NewLine = {x0,y0,x1,y1};

//	    PrevLine = NewLine;
	    if(Buffer.size() < MAX_MSG_LENGTH/4)
        {
            unsigned char msg[MAX_MSG_LENGTH + 1];
            msg[MAX_MSG_LENGTH] = '\0';
            int i = 0;

            msg[MAX_MSG_LENGTH] = '\0';

            CmdLines.push_back(NewLine);
            Buffer.push_back(NewLine);

#ifdef _DEBUG
            std::printf("SDWindow::newline\n");
#endif /*// _DEBUG*/

            for(drawlines_t::iterator iter_buffer = Buffer.begin(); iter_buffer != Buffer.end() && i < MAX_MSG_LENGTH; ++iter_buffer, i+=4)
            {
            msg[i] = iter_buffer->x0;
            msg[i+1] = iter_buffer->y0;
            msg[i+2] = iter_buffer->x1;
            msg[i+3] = iter_buffer->y1;
            }

            msg[i] = '\0';

            if(Buffer.size() == MAX_MSG_LENGTH/4)
            {
                SV_insert_msg(msg);
                Buffer.clear();
            }
            else
                if(!SV_insert_msg_nb(msg))
                    Buffer.clear();
        }
        else if(Buffer.size() > MAX_MSG_LENGTH/4)
                Buffer.clear();

	    x0 = 0;
	    y0 = 0;
	    x1 = 0;
	    y1 = 0;
//		g_viewerSettings.pause = false;
	}
	break;

	case mxEvent::MouseDown:
	{
//	    std::printf("%d,%d\n", event->x, event->y);
//      std::printf("%d,%d,%d,%d\n", x0, y0,x1,y1);
	    x0 = unsigned (char( event->x * 255.0f / w2() ));
	    y0 = unsigned (char( event->y * 255.0f / h2() ));
	    x1 = x0;
	    y1 = y0;

		return 1;
	}
	break;

	case mxEvent::MouseDrag:
	{
//	    std::printf("%d,%d,%d,%d\n", x0, y0,x1,y1);
	    x1 = unsigned (char( event->x * 255.0f / w2() ));
	    y1 = unsigned (char( event->y * 255.0f / h2() ));

		redraw ();

		return 1;
	}
	break;
	} // switch (event->event)

	return 1;
}

void GlWindow :: draw( void )
{
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    glViewport( 0, 0, w2(), h2() ); //resize glviewport
    //glViewport( 0, 0, w(), h() ); //resize glviewport

//    glPushMatrix();

    glLineWidth(3);

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_LINES);

    for(drawlines_t::iterator iter_lines = CmdLines.begin(); iter_lines != CmdLines.end(); ++iter_lines)
    {
        glVertex2f(iter_lines->x0 * 2.0f / 255.0f -1.0f, iter_lines->y0 * -2.0f / 255.0f +1.0f);
        glVertex2f(iter_lines->x1 * 2.0f / 255.0f -1.0f, iter_lines->y1 * -2.0f / 255.0f +1.0f);
    }

    glVertex2f(x0 * 2.0f / 255.0f -1.0f, y0 * -2.0f / 255.0f +1.0f);
    glVertex2f(x1 * 2.0f / 255.0f -1.0f, y1 * -2.0f / 255.0f +1.0f);

    glEnd();


//    glFlush();

//    glPopMatrix();
}

void GlWindow :: LineClear(){

    unsigned char msg[5] = {255, 255, 255, 255, 0};
    Buffer.clear();
    SV_insert_msg(msg);
    CmdLines.clear();
    redraw();

}
