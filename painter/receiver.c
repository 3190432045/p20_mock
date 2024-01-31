/*

===== receiver.c ========================================================

*/


#include <stdio.h>

#include "buffer.h"

#ifdef _SOFTGPIO
#include "softgpio.h"
#else
#include <wiringPi.h>
#endif

#include "shared.h"


int flag_receiver_ready;
ring_buffer receiver_buffer;
static byte elem;


#ifdef _DEBUG
byte RC_buffer_get()    /*function may block process */
{
    byte elem;
    if(!buffer_empty(&receiver_buffer))
        buffer_get(&receiver_buffer, &elem);
    else
    {
        printf("Receiver::buffer is empty, please wait...\n");
        for(;buffer_empty(&receiver_buffer);)
            delay(BAUD_RATE);
        buffer_get(&receiver_buffer, &elem);
    }
    return elem;
}
#else
byte RC_buffer_get()
{
    byte elem;
    for(;buffer_empty(&receiver_buffer);)
        delay(BAUD_RATE);
    buffer_get(&receiver_buffer, &elem);
    return elem;
}
#endif /*// _DEBUG*/

void RC_print_buffer()
{
    printf("------------------\n");
    buffer_print(&receiver_buffer);
    buffer_display(&receiver_buffer);
    printf("------------------\n");
}

void RC_flush_buffer()
{
    buffer_flush(&receiver_buffer);
}

void receiver_init()
{
    wiringPiSetup();
    pinMode(RX, INPUT);

#ifdef _DEBUG
    printf("Receiver::GPIO RX is ready\n");
#endif /*// _DEBUG*/

    buffer_init(&receiver_buffer, BUFFER_SIZE);
    for(; !digitalRead(RX);)
        delay(BAUD_RATE);
    flag_receiver_ready = 1;

#ifdef _DEBUG
    printf("Receiver::receiver is ready\n");
#endif /*// _DEBUG*/

}

void receiver_main()
{

    receiver_init();

    for(;;)
    {
        if(digitalRead(RX))     /* stop bit */
        {
            for(; digitalRead(RX);)
                delay(_SCAN_TIME_SPAN);

#ifdef _DEBUG
            printf("Receiver::receiving...\n");
#endif /*// _DEBUG*/

            delay(BAUD_RATE + BAUD_RATE/2);

#ifdef _MSB

            if(digitalRead(RX))
                elem |= 1 << 7;
            else
                elem |= 0 << 7;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 6;
            else
                elem |= 0 << 6;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 5;
            else
                elem |= 0 << 5;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 4;
            else
                elem |= 0 << 4;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 3;
            else
                elem |= 0 << 3;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 2;
            else
                elem |= 0 << 2;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 1;
            else
                elem |= 0 << 1;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1;
            else
                elem |= 0;
            delay(BAUD_RATE);
#else
            if(digitalRead(RX))
                elem |= 1;
            else
                elem |= 0;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 1;
            else
                elem |= 0 << 1;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 2;
            else
                elem |= 0 << 2;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 3;
            else
                elem |= 0 << 3;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 4;
            else
                elem |= 0 << 4;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 5;
            else
                elem |= 0 << 5;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 6;
            else
                elem |= 0 << 6;
            delay(BAUD_RATE);

            if(digitalRead(RX))
                elem |= 1 << 7;
            else
                elem |= 0 << 7;
            delay(BAUD_RATE);
#endif  /* _MSB */

            buffer_put(&receiver_buffer, &elem);
            elem = 0;
        }
        else
            delay(BAUD_RATE);
    }

}

