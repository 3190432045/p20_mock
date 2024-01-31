/*
//=============================================================================
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================
*/


#ifndef __SHARED__
#define __SHARED__


/*
// shared.h
*/


#ifdef __cplusplus
extern "C"
{
#endif


#ifndef __BUFFER__
#include "buffer.h"
#endif  /*__BUFFER__*/

#ifndef __CMDUTIL__
#include "cmdlib.h"
#endif  /*__CMDUTIL__*/


#define	BUFFER_SIZE     32  /* size must be 2 ^ n */
#define	BAUD_RATE       50 /* This is timespan not rate */
#define _SCAN_TIME_SPAN BAUD_RATE / 10

#define	MAX_MSG_LENGTH  (BUFFER_SIZE - 4)  /* BUFFER_SIZE - 4 */

#ifdef _SLOW
#define	MAX_WAIT_TIMES  (BUFFER_SIZE + 6) * 22
#elif defined _SLOWX2
#define	MAX_WAIT_TIMES  (BUFFER_SIZE + 6) * 33
#else
#define	MAX_WAIT_TIMES  (BUFFER_SIZE + 6) * 11
#endif

#ifdef _SERVER
#define	TX              0
#define	RX              1
#else
#define	TX              1
#define	RX              0
#endif /*// _SERVER*/

extern int flag_sender_ready;

extern ring_buffer sender_buffer;

void SD_buffer_put(byte elem);
void SD_print_buffer();
void SD_flush_buffer();

void sender_main();

/******************************/

extern int flag_receiver_ready;

extern ring_buffer receiver_buffer;

byte RC_buffer_get();
void RC_print_buffer();
void RC_flush_buffer();

void receiver_main();

#ifdef __cplusplus
}
#endif


#endif  /*__SHARED__*/
