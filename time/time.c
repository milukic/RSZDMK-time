#include "time.h"

#include <avr/io.h>
#include <avr/interrupt.h>

volatile unsigned long sys_time = 0;

ISR(TIMER0_COMPA_vect)
{
	sys_time++;
}

void initTime()
{
	TCCR0A = 0x02; //tajmer 0: CTC mod
#if F_CPU > 20000000
#error "Frekvencija takta mora biti manja od 20MHz!"
#endif
	unsigned long n = F_CPU / 1000;
	unsigned char clksel = 1;
	while (n > 255)
	{
		n /= 8;
		clksel++;
	}
	TCCR0B = clksel; //tajmer 0:
	OCR0A = (unsigned char) (n & 0xff) - 1; //perioda tajmera 0
	TIMSK0 = 0x02; //dozvola prekida tajmera 0
	sei();
	//I = 1 (dozvola prekida)
}

unsigned long millis()
{
	unsigned long tmp;
	cli();
	//zabrana prekida
	tmp = sys_time; //ocitavanje vremena
	sei();
	//dozvola prekida
	return tmp;
}

void delay_ms(unsigned long d)
{
	unsigned long t0 = millis();
	while (millis() - t0 < d);
}

