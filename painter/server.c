/*

===== server.c ========================================================

*/


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef _SOFTGPIO
#include "softgpio.h"
#else
#include <wiringPi.h>
#endif

#include "shared.h"
#include "client.h"

#include "server.h"


int flag_server_ready;
int flag_server_isdumping;
static int server_resend_count;

static char server_msg_buffer[MAX_MSG_LENGTH + 1];


#ifdef _DEBUG
void SV_insert_msg ( unsigned char *msg )   /*function may block process */
{
    if(strlen(msg) > MAX_MSG_LENGTH)
    {
        printf("Server::max message length exceeded, aborted!\n");
        return;
    }
    if(flag_server_ready)
        strcpy(server_msg_buffer, msg);
    else
    {
        printf("Server::server is busy, please wait...\n");
        for(; !flag_server_ready;)
            delay(BAUD_RATE);
        strcpy(server_msg_buffer, msg);
    }
}
#else
void SV_insert_msg ( unsigned char *msg )
{
    if(strlen(msg) > MAX_MSG_LENGTH)
        return;
    for(; !flag_server_ready;)
        delay(BAUD_RATE);
    strcpy(server_msg_buffer, msg);
}
#endif /*// _DEBUG*/

#ifdef _DEBUG
int SV_insert_msg_nb ( unsigned char *msg )
{
    if(strlen(msg) > MAX_MSG_LENGTH)
    {
        printf("Server::max message length exceeded, aborted!\n");
        return -2;
    }
    if(flag_server_ready)
    {
        strcpy(server_msg_buffer, msg);
        return 0;
    }
    else
    {
        printf("Server::server is busy, please wait...\n");
        return -1;
    }
}
#else
int SV_insert_msg_nb ( unsigned char *msg )
{
    if(strlen(msg) > MAX_MSG_LENGTH)
        return -2;
    if(flag_server_ready)
    {
        strcpy(server_msg_buffer, msg);
        return 0;
    }
    else
        return -1;
}
#endif /*// _DEBUG*/

void server_main ( void )
{

    byte server_pak[BUFFER_SIZE + 1];

    byte *pak_data_length = &server_pak[1];
    byte *pak_data = &server_pak[2];
    unsigned short *pak_crc;
    byte *pak_crc_byte;
    byte *pak_end;

    int i;
    unsigned short crcvalue;

    server_pak[0] = 0;  /* start bit */


    for(; !flag_sender_ready || !flag_receiver_ready;)
        delay(BAUD_RATE);
    SD_flush_buffer();
    flag_server_ready = 1;

#ifdef _DEBUG
            printf("Server::server is ready\n");
#endif /*// _DEBUG*/

    for(;;)
    {
        if(server_msg_buffer[0]!='\0')
        {
            flag_server_ready = 0;

            for(i=0; server_msg_buffer[i]!='\0' && i<MAX_MSG_LENGTH + 1; i++)
                pak_data[i] = server_msg_buffer[i];

            server_msg_buffer[0]='\0';
            *pak_data_length = i;
            pak_crc = (unsigned short *) &pak_data[i];
            pak_crc_byte = &pak_data[i];
            pak_end = &pak_data[i+2];
            pak_end[0] = '\0';

            CRC_Init(&crcvalue);
            for(i=0; i<*pak_data_length; i++) /* crc16 CCITT_FALSE */
                CRC_ProcessByte(&crcvalue, pak_data[i]);
            *pak_crc = CRC_Value(crcvalue);

#ifdef _DEBUG
            printf("Server::ready to send packet:");
            for(i=0; i<*pak_data_length + 4; i++)
            {
                printf("%d ", server_pak[i]);
            }
            printf("\n");
#endif /*// _DEBUG*/

            for(; flag_client_isdumping;)
                delay(BAUD_RATE);

            flag_server_isdumping = 1;
            for(i=0; i<*pak_data_length + 4; i++)
                SD_buffer_put(server_pak[i]);
            flag_server_isdumping = 0;

#ifdef _DEBUG
            printf("Server::sending packet...\n");
#endif /*// _DEBUG*/

            for(; !flag_server_ready;)
            {
                for(i=0; i<MAX_WAIT_TIMES; i++,delay(BAUD_RATE))  /* fix me */
                    if(flag_client_get_ack)
                    {
                        if((client_crc16[0] == pak_crc_byte[0] && client_crc16[1] == pak_crc_byte[1]) || (client_crc16[0] == pak_crc_byte[1] && client_crc16[1] == pak_crc_byte[0]))
                        {
                            printf("Server::ACK packet received\nServer::message sent successfully\n");
                            server_resend_count = 0;
                            flag_server_ready = 1;
                            break;
                        }
                        else
                        {
                            printf("Server::ACK packet CRC16 failed, resending...\n");
                            if(server_resend_count == 2)
                            {
                                printf("Server::second resend failed, aborted!\n");
                                flag_server_ready = 1;
                                server_resend_count = 0;
                                break;
                            }
                            server_resend_count++;
                            for(; flag_client_isdumping;)
                                delay(BAUD_RATE);

                            flag_server_isdumping = 1;
                            for(i=0; i<*pak_data_length + 4; i++)
                                SD_buffer_put(server_pak[i]);
                            flag_server_isdumping = 0;

                            break;
                        }
                    }

                if(!flag_client_get_ack)
                {
                        printf("Server::ACK timeout, resending...\n");
                        if(server_resend_count == 2)
                        {
                            printf("Server::second resend failed, aborted!\n");
                            flag_server_ready = 1;
                            server_resend_count = 0;
                            break;
                        }
                        server_resend_count++;
                        for(; flag_client_isdumping;)
                            delay(BAUD_RATE);

                        flag_server_isdumping = 1;
                        for(i=0; i<*pak_data_length + 4; i++)
                            SD_buffer_put(server_pak[i]);
                        flag_server_isdumping = 0;
                }

                flag_client_get_ack = 0;
            }

#ifdef _DEBUG
            printf("Server::server is ready\n");
#endif /*// _DEBUG*/

        }
        delay(BAUD_RATE);
    }
}
