/*
//
//
//
// file: painter.c
// last modified:
// copyright:
// version:
//
// email:
// web:
//
*/


#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "cmdlib.h"
#include "threads.h"

#include "GlPainter.h"
#include "shared.h"
#include "server.h"
#include "client.h"
#include "action.h"

#include "painter.h"

qboolean dumplog = false;
void WriteLog (char *name)
{
	FILE		*out;

	out = fopen (name, "w");
	if (!out)
		Error ("Couldn't open %s", name);

    fprintf (out, "\n");


	fclose (out);
}

void thread_glpainter()
{

#ifdef _DEBUG
    printf("start thread_glpainter\n");
#endif /*// _DEBUG*/

    glpainter_main();

#ifdef _DEBUG
    printf("thread_glpainter terminated\n");
#endif /*// _DEBUG*/


}

void thread_sender()
{

#ifdef _DEBUG
    printf("start thread_sender\n");
#endif /*// _DEBUG*/

    sender_main();

#ifdef _DEBUG
    printf("thread_sender terminated\n");
#endif /*// _DEBUG*/

}

void thread_receiver()
{

#ifdef _DEBUG
    printf("start thread_receiver\n");
#endif /*// _DEBUG*/

    receiver_main();

#ifdef _DEBUG
    printf("thread_receiver terminated\n");
#endif /*// _DEBUG*/

}

void thread_server()
{

#ifdef _DEBUG
    printf("start thread_server\n");
#endif /*// _DEBUG*/

    server_main();

#ifdef _DEBUG
    printf("thread_server terminated\n");
#endif /*// _DEBUG*/

}

void thread_client()
{

#ifdef _DEBUG
    printf("start thread_client\n");
#endif /*// _DEBUG*/

    client_main();

#ifdef _DEBUG
    printf("thread_client terminated\n");
#endif /*// _DEBUG*/

}

void thread_cmd()
{

#ifdef _DEBUG
    printf("start thread_cmd\n");
#endif /*// _DEBUG*/

    action_main();

#ifdef _DEBUG
    printf("thread_cmd terminated\n");
#endif /*// _DEBUG*/

}

void thread_create(int id)
{
    switch(id)
    {
        case 0 : thread_sender();break;
        case 1 : thread_receiver();break;
        case 2 : thread_server();break;
        case 3 : thread_client();break;
        case 4 : thread_cmd();break;
        case 5 : thread_glpainter();break;
    }
}

int main( int argc, char **argv )
{
    int i;
	double		start, end;

	printf( "painter.exe  (%s)\n", __DATE__ );
	printf ("----- Painter ----\n");

	verbose = true;  /* Originally FALSE */

	for (i=0 ; i<argc ; i++)
	{
		if (!strcmp(argv[i],"-log"))
			dumplog = true;
		else if (!strcmp(argv[i],"-verbose"))
		{
			verbose = true;
		}
		else if (!strcmp(argv[i],"-terse"))
		{
			verbose = false;
		}
		else if (!strcmp(argv[i],"-threads"))
		{
			if ( ++i < argc )
			{
				numthreads = atoi (argv[i]);
				if ( numthreads <= 0 )
				{
					fprintf(stderr, "Error: expected positive value after '-threads'\n" );
					return 1;
				}
			}
			else
			{
				fprintf( stderr, "Error: expected a value after '-threads'\n" );
				return 1;
			}
		}
	}

	if (i != argc )
		Error ("usage: painter [-log] [-threads n] [-verbose] [-terse]");

    ThreadSetDefault ();

#ifndef WIN32
    numthreads = 6;
#endif /*// WIN32*/

	start = I_FloatTime ();
	RunThreadsOnIndividual(6, false, thread_create);
/*
 //      RunThreadsOn (6, true, thread_create);
 //      RunThreadsOn (6, true, thread_create);
 */
	end = I_FloatTime ();
    printf ("%5.0f seconds elapsed\n", end-start);

    return 0;
}
