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


#ifndef __SERVER__
#define __SERVER__


/*
// server.h
*/


#ifdef __cplusplus
extern "C"
{
#endif

extern int flag_server_ready;
extern int flag_server_isdumping;

void SV_insert_msg(unsigned char *msg);
int SV_insert_msg_nb(unsigned char *msg);

void server_main();

#ifdef __cplusplus
}
#endif


#endif  /*__SERVER__*/

