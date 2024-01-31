/*

===== sender.c ========================================================

*/


#include <stdio.h>

#include "buffer.h"

#ifdef _SOFTGPIO
#include "softgpio.h"
#else
#include <wiringPi.h>
#endif

#include "shared.h"


int flag_sender_ready;
ring_buffer sender_buffer;
static byte elem;


#ifdef _DEBUG
void SD_buffer_put(byte elem)   /*function may block process */
{
    if(!buffer_full(&sender_buffer))
        buffer_put(&sender_buffer, &elem);
    else
    {
        printf("Sender::buffer is full, please wait...\n");
        for(;buffer_full(&sender_buffer);)
            delay(BAUD_RATE);
        buffer_put(&sender_buffer, &elem);
    }
}
#else
void SD_buffer_put(byte elem)
{
    for(;buffer_full(&sender_buffer);)
        delay(BAUD_RATE);
    buffer_put(&sender_buffer, &elem);
}
#endif /*// _DEBUG*/

void SD_print_buffer()
{
    printf("------------------\n");
    buffer_print(&sender_buffer);
    buffer_display(&sender_buffer);
    printf("------------------\n");
}

void SD_flush_buffer()
{
    buffer_flush(&sender_buffer);
}

void sender_init()
{
    wiringPiSetup();
    pinMode(TX, OUTPUT);

#ifdef _DEBUG
    printf("Sender::GPIO TX is ready\n");
#endif /*// _DEBUG*/

    buffer_init(&sender_buffer, BUFFER_SIZE);
    digitalWrite(TX, HIGH);
    delay(BAUD_RATE);
    delay(BAUD_RATE); /* sleep for 2 bit */
    flag_sender_ready = 1;

#ifdef _DEBUG
    printf("Sender::sender is ready\n");
#endif /*// _DEBUG*/

}

void sender_main()
{

    sender_init();

    for(;;)
    {
        if(!buffer_empty(&sender_buffer))
        {

#ifdef _DEBUG
            printf("Sender::sending...\n");
#endif /*// _DEBUG*/

            digitalWrite(TX, LOW);  /* start bit */
            delay(BAUD_RATE);

            buffer_get(&sender_buffer, &elem);

#ifdef _MSB
            if((elem >> 7) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 6) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 5) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 4) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 3) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 2) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 1) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if(elem & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);
#else
            if(elem & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 1) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 2) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 3) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 4) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 5) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 6) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);

            if((elem >> 7) & 1)
                digitalWrite(TX, HIGH);
            else
                digitalWrite(TX, LOW);
            delay(BAUD_RATE);
#endif  /* _MSB */

            digitalWrite(TX, HIGH); /*stop bit*/
            delay(BAUD_RATE);

            delay(BAUD_RATE);   /* optional */

#ifdef _SLOW
            delay(BAUD_RATE * 11);
#endif /*// _SLOW*/

#ifdef _SLOWX2
            delay(BAUD_RATE * 22);
#endif /*// _SLOWX2*/

        }

        delay(BAUD_RATE);
    }

}
