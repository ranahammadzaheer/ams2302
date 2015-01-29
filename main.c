/*
 * main.c
 *
 * Created on: 13.03.2013
 *     Author: Pascal Gollor
 *        web: http://www.pgollor.de
 *
 * Dieses Werk ist unter einer Creative Commons Lizenz vom Typ
 * Namensnennung - Nicht-kommerziell - Weitergabe unter gleichen Bedingungen 3.0 Deutschland zugänglich.
 * Um eine Kopie dieser Lizenz einzusehen, konsultieren Sie
 * http://creativecommons.org/licenses/by-nc-sa/3.0/de/ oder wenden Sie sich
 * brieflich an Creative Commons, 444 Castro Street, Suite 900, Mountain View, California, 94041, USA.
 *
 *
 * testet with:
 * - avr-gcc 4.3.4
 * - Atmega8 @ 8 MHz
 */


#include "main.h"

#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "uart.h"
#include "am2302.h"


#define led_on      PORT_LED |= (1 << LED);
#define led_off     PORT_LED &= ~(1 << LED);


int main(void)
{
	DDR_SENSOR &= ~(1 << SENSOR); // define as input
	DDR_LED |= (1 << LED); // define as output

	PORT_SENSOR &= ~(1 << SENSOR);  // disable pullup

	led_off;

	uart_init(BAUDRATE);
 	sei();

	usart_write_str(CR "--- AM2302 ---" CR);

	while(1)
	{
		uint16_t humidity = 0;
		uint16_t temp = 0;

		led_on;

		uint8_t error = am2302(&humidity, &temp);
		if (!error)
		{
			printf("%i,%i%% %i,%iC" CR, humidity/10, humidity%10, temp/10, temp%10);
		}
		else
		{
			printf("Error %i" CR, error);
		}

		led_off;
		
		_delay_ms(1000);
	}

	return 0;
}
