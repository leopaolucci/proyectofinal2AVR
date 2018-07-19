/*
 * main
 *
 *  Created on: 19 jun. 2018
 *      Author: Administrador
 */
#include <stdlib.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "BH1750.h"
#include "HDK128.h"
#include "lcd_alfa.h"
#include "mydef.h"
#include "timer.h"
#include "TWI_master.h"

#define FOSC 8000000 				// Clock Speed
#define BAUD 9600 					// Baud Rate
#define MYUBRR FOSC/16/BAUD-1 		// Datasheet formula. UBRR
#define AC 0						//modo automatico
#define MC 1						//modo manual


unsigned int read_bh();					//funcion de lectura de sensor
void init_read();						//funcion de inicializacion de lectura
void config_set(void);					//configuracion del código
void UART_Init( unsigned int ubrr ); 	//configuracion de UART1
void brillo(unsigned int);				//funcion de actualizacion del brillo
void lcd_mostrar(void);  				//funcion de actualizacion del modo, estado, brillo y cantidad de luz
void respuesta(void);					//me fijo si hay que responder algo
unsigned int lux(unsigned int);			//convierto a lux el dato del sensor
void manual(void);						//rutina en modo manual
void automatico(void);					//rutina en modo automatico

volatile int i=0;
volatile int cont=0;					//variable de conteo timer
volatile int refresh=0;					//variable de control tiempo de refresco de pantalla
volatile unsigned char bxxx[]="010";	//valor de brillo inicial en 10
volatile unsigned char luxom[]="00001";  //variable para imprimir el valor del luxometro
volatile unsigned char view[]="000";
volatile char mode= AC;					//mode en AC indica el modo automatico, en MC el modo manual
volatile char blue_dat ='H';    		//le coloco un valor inicial que no moleste
volatile unsigned int data=0;			//aca guardo las lecturas de sensor de luz
volatile bit_t flag;					//creamos los flags


int main(void)
{
	sei();						//habilito interrupciones globales
	config_set();				//cargamos la configuracion del código
	twi_init();					//inicializamos el sensor de luz
	init_read();				//iniciamos lectura de sensor
	UART_Init(MYUBRR);			//inicializamos la UART
	lcd_init(LCD_DISP_ON);		//inicializo el display
	lcd_clrscr();				//limpiamos pantalla

	UART1_TX_OFF;				//apagamos la interrupcion por transmision completa, solo dejamos la de recepcion


	while(1)					//comienzo de bucle de programa
    {
		respuesta();			//me fijo si hay que responder algo O flags en alto

		if(mode==AC)
		{
			disable_interrupt_INT5;	//desactivo interrupcion de boton 5
			disable_interrupt_INT6;	//desactivo interrupcion de boton 6
			automatico();			//voy a rutina en modo automatico

		}
		else
		{
			enable_interrupt_INT5;	//activo interrupcion de boton 5
			enable_interrupt_INT6;	//activo interrupcion de boton 6

			while((atoi(bxxx)%10)!=0)	//regulo el valor del brillo a un valor multiplo de 10 para simplificar
			{
				itoa(atoi(bxxx)-1,bxxx,10);
			}

			timer0_compare_value=(int)((225/90)*(atoi(bxxx)-10)+25); //ajusto el ancho de pulso de acuerdo al brillo (interpolacion)

			manual();				//voy a rutina en modo manual
		}
    }
	return 0;
}

void UART_Init( unsigned int ubrr )
{


/* Set baud rate */
UBRR1H = (unsigned char)(ubrr>>8);
UBRR1L = (unsigned char)ubrr;

/* Enable receiver and transmitter */ /*enable RX interrupt*/

UCSR1B = (1<<RXEN)|(1<<TXEN)|(1<<RXCIE1);

/* Set frame format: 8data, 2stop bit */

UCSR1C = (1<<USBS)|(3<<UCSZ0);

}

void config_set()
{
	set_output(DDRB,4);	//establezco como salida el pin B4 que maneja la salida del PWM
	set_sw5;			//se habilita el pulsador del pin E5
	set_sw6;			//se habilita el pulsador del pin E6

	enable_interrupt_INT5;		//habilito interrupcion externa por pin E5
	enable_interrupt_INT6;		//habilito interrupcion externa por pin E6
	INT5_falling_edge;			//establezco que la INT5 se habilite por flanco descendente
	INT6_falling_edge;			//establezco que la INT6 se habilite por flanco descendente

	clr_reg(PORTA);				//limpio puerto	A
	clr_reg(PORTC);				//limpio puerto	C
	clr_reg(PORTD);				//limpio puerto	D
	clr_reg(PORTE);				//limpio puerto	E


	timer0_mode_fastPWM;		//configuro al timer en modo fast PWM
	timer0_prescaler_8;			//establezco el prescaler en 8, dando un tiempo por cuenta de ciclo de 1 useg.
	timer0_compare_value =25; 	//defino el OCR de comparacion para el PWM inicial al 10%

	timer2_mode_normal;		  	//timer 2 en modo normal
	timer2_prescaler_8;			//establezco el prescaler en 8, dando un tiempo por cuenta de ciclo de 1 useg.
	timer2_timer_value = 155; 	// Desborda en 100useg
	timer2_int_overflow;		//habilitamos interrupcion por desborde de timer

	flag.B0=0;					//pongo en 0 flag de recepcion de B
	flag.B1=0;					//pongo en 0 flag de refresco de pantalla (cont 1000)
	flag.B2=0;					//pongo en 0 flag de refresco de lectura de sensor(cont 3000)
	flag.B3=0;					//pongo en 0 flag de respuesta (..hay algo para responder..)
	flag.B4=0;					//pongo en 0 flag de respuesta para A
	flag.B5=0;					//pongo en 0 flag de respuesta para L
	flag.B6=0;					//pongo en 0 flag de respuesta para S
	flag.B7=0;					//pongo en 0 flag de flag de stop de timer

}

unsigned int read_bh()
{
	unsigned int datin=0;

					twi_start_cond();					//mandamos condicion de START
					i2c_write_byte(ADDRL_R); 			//mandamos address + modo escritura
					datin=(int)i2c_read_byte2(1);			//leemos el sensor
					datin=(datin<<8);
					datin|=(int)i2c_read_byte2(0);			//leemos la otra parte del registro
					twi_stop_cond();					//condicion de STOP de trama

					return datin;
}

void init_read()
{
				twi_start_cond();					//mandamos condicion de START
				i2c_write_byte(ADDRL_W); 			//mandamos address + modo escritura
				i2c_write_byte(CONT_HRES_M1);		//mandamos el comando de medicion de alta resolucion (CONTINUO)
				twi_stop_cond();					//condicion de STOP de trama
				_delay_ms(120);						//esperamos a que haga la primer conversion
}

void lcd_mostrar()
{
	char msj[6];

	lcd_clrscr();
	lcd_gotoxy(0,0);
	lcd_puts("MODE:");
	lcd_putc((mode==AC) ? 'A':'M');				//si mode esta en AC mandamos A sino M

	lcd_gotoxy(8,0);
	lcd_puts("LED:");
	if(flag.B7)
	{
		lcd_puts("OFF");
	}
	else	lcd_puts("ON");

	lcd_gotoxy(0,1);
	lcd_puts("BR:");
	lcd_puts(bxxx);

	lcd_gotoxy(8,1);
	lcd_puts("LX:");
	sprintf(luxom,"%i",lux(data));				//guardamos en luxom el dato del sensor
	lcd_puts(luxom);

}


void manual(void)
{
  if(flag.B1)
  {
	  lcd_mostrar();
	  flag.B1=0;
  }

}

void automatico(void)
{
	unsigned int conver=0;

	conver=atoi(bxxx);
	timer0_compare_value=(int)((225/90)*(conver-10)+25); //ajusto el ancho de pulso de acuerdo al brillo  (interpolacion)

}


void brillo(unsigned int din)
{
	unsigned int conver=0;

	conver=(int)((-90/65534)*(lux(din)-1)+100);			//interpolacion de valores para el brillo--MAX para 65534 lx MIN  para 1 lx.
	sprintf(bxxx,"%i",conver);							//guardo resultado en bxxx
}


void respuesta(void)
{

	//char msj[6];

	if(flag.B3)				//¿HAY ALGO PARA RESPONDER?, si el flag esta en bajo directamente este bloque de codigo no lo ve
	{
		if(flag.B4)			//flag de respuesta A
		{
			if(flag.B7)
			{
//				sprintf(msj,"OFF");
//				while (!UDRE); 					// Esperar la TX
//				UDR1 = (int)msj; 				// Escribir en el buffer
			}
			else
			{
//				sprintf(msj,"ON");
//				while (!UDRE); 					// Esperar la TX
//				UDR1 = (int)msj; 				// Escribir en el buffer
			}
			flag.B4=0;					//bajamos el flag de respuesta A
		}

		if(flag.B5)			//flag de respuesta L
		{
//			if(!flag.B7)	//si el flag b7 esta en bajo, significa que hay pwm del led
//			{
//				sprintf(msj,"ON:");
//				while (!UDRE); 					// Esperar la TX
//				UDR1 = (int)msj; 				// Escribir en el buffer
//				while (!UDRE); 					// Esperar la TX
//				UDR1 = (int)bxxx; 				// Escribir en el buffer
//			}
//			else
//			{
//				sprintf(msj,"OFF:");
//				while (!UDRE); 					// Esperar la TX
//				UDR1 = (int)msj; 				// Escribir en el buffer
//			}
//			flag.B5=0;
		}
		if(flag.B6)			//flag de respuesta S
		{
//			sprintf(msj,"lx:");
//			while (!UDRE); 					// Esperar la TX
//			UDR1 = (int)msj; 				// Escribir en el buffer
//			while (!UDRE); 					// Esperar la TX
//			UDR1 = lux(data); 				// Escribir en el buffer
//			flag.B6=0;						// bajamos bandera
		}

		flag.B3=0;			//bajamos flag de respuesta
	}
	if(flag.B1)							//si hay que refrescar pantalla
	{
				lcd_mostrar();
				flag.B1=0;
	}
	if(flag.B2)
	{
		data = read_bh();					//leemos el estado del sensor
		if(mode==AC) brillo(data);			//actualizamos el brillo
	    flag.B2=0;
	}
}

unsigned int lux(unsigned int din)
{
	unsigned int lx;

	lx=(int)((float)din/1.2);			//devuelvo valor de lux

	return lx;
}


ISR(TIMER2_OVF_vect)					//interrupcion timer2 OVF - lectura de BH1750
{
	cont++;
	refresh++;

	if(refresh==200)							//si pasaron 20ms
	{
		flag.B1=1;							//habilito flag de refrescar pantalla
		refresh=0;							//reiniciamos el refrescado de pantalla
	}

	if(cont==3000)							//si pasaron 300ms
	{
	   cont=0;
	   flag.B2=1;							//activamos el flag de actualizacion de sensor
	}

	timer0_timer_value = 155;				//volvemos a cargar el valor inicial de conteo
}

ISR(USART1_RX_vect)							//lectura de datos bluetooth
{

	unsigned int aux=0;


	blue_dat = UDR1;    //LEEMOS DATO BLUETOOTH

	switch(blue_dat)
			{
				case'A':

					if(mode==AC) mode=MC;
					else mode=AC;

				break;

				case'B':
					if(mode==MC)
					{
						flag.B0=1;
					}
				break;

				case 'L':
					if(mode==MC)
					{
						if(!flag.B7)			//verificamos que el timer no este en stop
						{
							timer0_stop ;		//apagamos el led
							flag.B7=1;			//levantamos flag de stop de pwm
							flag.B3=1;			//flag de respues en alto
							flag.B5=1;			//flag de respuesta L en alto
							clr_bit(PINB,4);	//ponemos en bajo la salida del timer
						}
						else
						{
							timer0_prescaler_8;		//encendemos el pwm
							flag.B7=0;				//bajamos el flag de stop de pwm
							flag.B3=1;				//flag de respuesta en alto
							flag.B5=1;				//flag de respuesta L en alto
						}
					}
				break;

				case 'S':
						while (!UDRE); 					// Esperar la TX
						UDR1 = (int)data; 				// Escribir en el buffer
				break;

				default:
					if(flag.B0)
					{
						if(i<2)
						{
						sprintf(view[i],"%c",blue_dat);
						i++;
						}
						if(i==2)
						{
						   sprintf(view[i],"%c",blue_dat);

						   if(atoi(view)>10 && atoi(view)<=100)
						   {
							   sprintf(bxxx,"%s",view);
						   }
							flag.B0=0;
						}
					}
				break;
			}
 }


ISR(INT5_vect)							//inicio codigo interrupcion INT5
{
	unsigned int conver=0;

	if(mode==MC)
	{
			conver=atoi(bxxx);					//pasaje caracter a int -> guardo en conver

			if(conver>10)						//baja el ciclo de actividad si la variable de decenas es mayor que 1. MINIMO 10%.
			{

				conver= conver-10;				//decremento el valor de decenas en 1
				timer0_compare_value -= 25;		//decremento en un 10% al ciclo de actividad
			}

			itoa(conver,bxxx, 10);				//pasaje int a caracter -> guardo en Bxxx

	}
}										//fin interrupcion

ISR(INT6_vect)							//inicio codigo interrupcion INT6
{
	unsigned int conver=0;

	if(mode==MC)
	{
	conver=atoi(bxxx);					//pasaje caracter a int -> guardo en conver

	if(conver<=90)						//Sube el ciclo de actividad si la variable decenas es menor a 9. MAXIMO 90%.
	{
		conver=conver+10;						//aumento en 1 las decenas
		timer0_compare_value += 25;	//aumento en un 10% el ciclo de actividad
	}

	itoa(conver,bxxx, 10);				//pasaje int a caracter -> guardo en Bxxx

	}
}					//fin interrupcion

//
