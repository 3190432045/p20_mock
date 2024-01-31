/*
//=============================================================================
//
// Purpose:
//
// $NoKeywords: $
//=============================================================================
*/


#ifndef __SOFTGPIO__
#define __SOFTGPIO__


/*
// softgpio.h
*/


#ifdef __cplusplus
extern "C"
{
    namespace softgpio {
#endif

#ifdef _SOFTGPIO


#define	INPUT			 0
#define	OUTPUT			 1
#define	PWM_OUTPUT		 2
#define	GPIO_CLOCK		 3
#define	SOFT_PWM_OUTPUT		 4
#define	SOFT_TONE_OUTPUT	 5
#define	PWM_TONE_OUTPUT		 6


#define	LOW			     0
#define	HIGH			 1


int wiringPiSetup( void );
void pinMode(int pin, int mode);

int digitalRead(int pin);

void digitalWrite(int pin, int value);

void delay(unsigned int howLong);
void delayMicroseconds(unsigned int howLong);


#ifdef __cplusplus
    } //namespace softgpio
}
#endif

#endif /* _SOFTGPIO */


#endif  /*__SOFTGPIO__*/
