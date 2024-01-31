/*

===== sotfgpio.c ========================================================

*/

#ifdef _SOFTGPIO

#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#endif

#include "cmdlib.h"

#include "softgpio.h"


typedef struct
{
    char state;
} softgpio_t;


/*static char filepath[14][128];*/
static char filepath[14][9];
static qboolean pinmode[14] = {true};

int wiringPiSetup( void )
{
    char source[128];
    int i;
    Q_getwd(source);
    for(i=0; i<14; i++)

    {
        /*
        strcpy(filepath[i], source);
        sprintf (filepath[i] + strlen(filepath[i]),"pin%d.bin", i);
        */
        sprintf (filepath[i],"pin%d.bin", i);
    }

    return 0;
}

void pinMode(int pin, int mode)
{
    softgpio_t default_value;
    default_value.state = 1;
    if(mode == INPUT)
        pinmode[pin] = true;
    else if(mode == OUTPUT)
        pinmode[pin] = false;
    SaveFile (filepath[pin],&default_value,sizeof(softgpio_t));
}

int digitalRead(int pin)
{
    if(pinmode[pin])

    {
        softgpio_t *buffer;
        int ret;
        LoadFile (filepath[pin], (void **)&buffer);
        ret = buffer->state;
        free(buffer);
        return ret;
    }

    else
        Error("Write Only");
    return -1;
}

void digitalWrite(int pin, int value)
{
    if(!pinmode[pin])

    {
        softgpio_t buffer;
        buffer.state = (char)value;
        SaveFile (filepath[pin], &buffer,sizeof(softgpio_t));
    }

    else
        Error("Read Only");

}

void delay(unsigned int howLong)
{

#ifdef WIN32
    Sleep(howLong);
#else
    usleep(howLong * 1000);
#endif

}

void delayMicroseconds(unsigned int howLong)
{

#ifdef WIN32

#else
    usleep(howLong);
#endif

}

#endif /* _SOFTGPIO */
