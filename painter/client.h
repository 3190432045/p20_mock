/*
//=============================================================================
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================
*/

/*

frame structure
|0|D0|D1|D2|D3|D4|D5|D6|D7|1|1|
data packet structure
|0|length of data|-data-|-|crc16byte0|crc16byte1|
ACK (acknowledge) packet structure
|6|crc16byte0|crc16byte1|

*/


#ifndef __CLIENT__
#define __CLIENT__


/*
// client.h
*/


#ifdef __cplusplus
extern "C"
{
#endif

extern int flag_client_ready;
extern int flag_client_isdumping;

extern int flag_client_get_ack;
extern byte client_crc16[3];

int CL_get_msg (char *msg);

void client_main();

#ifdef __cplusplus
}
#endif


#endif  /*__CLIENT__*/
