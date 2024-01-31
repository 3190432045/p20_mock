/*

===== client.c ========================================================

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
#include "server.h"

#include "client.h"


int flag_client_ready;
int flag_client_isdumping;
int flag_client_get_ack;
byte client_crc16[3];

static char client_msg_buffer[MAX_MSG_LENGTH + 1];


#ifdef _DEBUG
int CL_get_msg ( char *msg )
{
    /*
    if(strlen(msg) < MAX_MSG_LENGTH)
    {
        printf("Client::not enough space, aborted!\n");
        return -2;
    }
    */
    if(flag_client_ready)
    {
        strcpy(msg, client_msg_buffer);
        client_msg_buffer[0] = '\0';
        return 0;
    }
    else
        return -1;
}
#else
int CL_get_msg ( char *msg )
{
    if(flag_client_ready)
    {
        strcpy(msg, client_msg_buffer);
        client_msg_buffer[0] = '\0';
        return 0;
    }
    else
        return -1;
}
#endif /*// _DEBUG*/

void client_main()
{

    byte client_pak[BUFFER_SIZE + 1];

    byte *pak_data_length = &client_pak[1];
    byte *pak_data = &client_pak[2];
    byte *pak_crc;
    byte *pak_crc_byte;
    byte *pak_end;

    int i;
    unsigned short crcvalue;

    for(; !flag_sender_ready || !flag_receiver_ready;)
        delay(BAUD_RATE);
    RC_flush_buffer();
    flag_client_ready = 1;

#ifdef _DEBUG
            printf("Client::client is ready\n");
#endif /*// _DEBUG*/

    for(;;)
    {
        client_pak[0] = RC_buffer_get();

        if(client_pak[0] == 0)    /* data packet */
        {
            flag_client_ready = 0;

#ifdef _DEBUG
            printf("Client::receiving packet...\n");
#endif /*// _DEBUG*/

            *pak_data_length = RC_buffer_get();
            if(*pak_data_length>MAX_MSG_LENGTH)
                printf("Client::invalid packet length, aborted!\n");

            for(i=0; i<*pak_data_length; i++)
                pak_data[i] = RC_buffer_get();

            pak_crc_byte = &pak_data[i];
            pak_crc_byte[0] = RC_buffer_get();
            pak_crc_byte[1] = RC_buffer_get();
            pak_end = &pak_data[i+2];
            pak_end[0] = '\0';

            CRC_Init(&crcvalue);
            for(i=0; i<*pak_data_length; i++) /* crc16 CCITT_FALSE */
                CRC_ProcessByte(&crcvalue, pak_data[i]);
            crcvalue = CRC_Value(crcvalue);
            pak_crc = (byte *) &crcvalue;

            if((pak_crc[0] == pak_crc_byte[0] && pak_crc[1] == pak_crc_byte[1]) || (pak_crc[0] == pak_crc_byte[1] && pak_crc[1] == pak_crc_byte[0]))
            {

#ifdef _DEBUG
                printf("Client::packet received\n");
                for(i=0; i<*pak_data_length + 4; i++)
                {
                    printf("%d ", client_pak[i]);
                }
                printf("\n");
#endif /*// _DEBUG*/

                printf("Client::new message received:");
                for(i=0; i<*pak_data_length; i++)
                {
                    client_msg_buffer[i] = pak_data[i];
                    /*printf("%c", pak_data[i]);*/
                }
                printf("%s\n", client_msg_buffer);

                for(; flag_server_isdumping;)
                    delay(BAUD_RATE);

                flag_client_isdumping = 1;
                SD_buffer_put(6);
                SD_buffer_put(pak_crc_byte[0]);
                SD_buffer_put(pak_crc_byte[1]);
                flag_client_isdumping = 0;

            }
            else
                printf("Client::data packet CRC16 failed, aborted!\n");
        }
        else if(client_pak[0] == 6)    /* ACK packet */
        {
            client_crc16[0] = RC_buffer_get();
            client_crc16[1] = RC_buffer_get();
            flag_client_get_ack = 1;

#ifdef _DEBUG
            printf("Client::ACK to local server...\n");
#endif /*// _DEBUG*/

        }

        /*RC_flush_buffer();*/

#ifdef _DEBUG
        printf("Client::client is ready\n");
#endif /*// _DEBUG*/

        flag_client_ready = 1;
    }
}
