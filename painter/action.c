/*

===== sotfgpio.c ========================================================

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

#include "action.h"


void CMD_help()
{
    printf("------------------\n"
           "svsendmsg" " <string>"
           "\n"
           "\n"
           "sdecho"
           "\n"
           "sdputc" " <char>"
           "\n"
           "sdflush"
           "\n"
           "rcecho"
           "\n"
           "rcgetc"
           "\n"
           "rcflush"
           "\n"
           "\n"
           "help"
           "\n"
           "quit"
           "\n"
           "------------------\n"
          );
}

void action_main()
{
    char cmd[128];
    char narg[128];

    for(; !flag_sender_ready || !flag_receiver_ready;)
        delay(BAUD_RATE);

    printf("Enter 'help' for a list of commands\n");
    for(;;)

    {
        gets(cmd);

        strcpy(narg,cmd);
        narg[9]='\0';

        if(!strcmp(narg,"svsendmsg"))
            SV_insert_msg(&cmd[10]);

        if(!strcmp(cmd,"sdecho"))
            SD_print_buffer();

        strcpy(narg,cmd);
        narg[6]='\0';

        if(!strcmp(narg,"sdputc"))
            SD_buffer_put(cmd[7]);

        if(!strcmp(cmd,"sdflush"))
            SD_flush_buffer();

        /*if(!strcmp(cmd,"rcsync"))
            RC_sync();*/

        if(!strcmp(cmd,"rcecho"))
            RC_print_buffer();

        if(!strcmp(cmd,"rcgetc"))
            printf("%d\n", RC_buffer_get());

        if(!strcmp(cmd,"rcflush"))
            RC_flush_buffer();

        if(!strcmp(cmd,"help"))
            CMD_help();

        if(!strcmp(cmd,"quit"))
            exit(0);

    }

}
