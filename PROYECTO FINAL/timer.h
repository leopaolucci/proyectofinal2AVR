#ifndef TIM_DEFS_H_
#define TIM_DEFS_H_

#include<avr/interrupt.h>

/* Definiciones timer 0 */

/* Valor contra el que compara (para el modo compare match) */
#define timer0_compare_value 			OCR0

/* Valor del timer */
#define timer0_timer_value 				TCNT0

/* Salida asociada al timer */
#define timer0_out						DDRB |= (1<<4)

/* Modos de funcionamiento */
#define timer0_mode_normal 				TCCR0&=~((1<<WGM01)|(1<<WGM00))
#define timer0_mode_ctc_toggle 			TCCR0|=(1<<WGM01); \
	                           	    	TCCR0&=~(1<<WGM00); \
	                           	    	TCCR0&=~(1<<COM01); \
	                           	    	TCCR0|=(1<<COM00)
#define timer0_mode_ctc_clear 			TCCR0|=(1<<WGM01); \
	                          	  		TCCR0&=~(1<<WGM00); \
	                          	  		TCCR0|=(1<<COM01); \
	                          	  		TCCR0&=~(1<<COM00)
#define timer0_mode_ctc_set 			TCCR0|=(1<<WGM01); \
	                        			TCCR0&=~(1<<WGM00); \
	                        			TCCR0|=(1<<COM01); \
	                        			TCCR0|=(1<<COM00)
#define timer0_mode_fastPWM 			TCCR0|=((1<<WGM01)|(1<<WGM00)); \
	                        			TCCR0|=(1<<COM01); \
	                        			TCCR0&=~(1<<COM00)
#define timer0_mode_fastPWM_inverting 	TCCR0|=((1<<WGM01)|(1<<WGM00)); \
	                                  	TCCR0|=(1<<COM01)|(1<<COM00)

/* Modo de interrupci�n */
#define timer0_int_overflow 			TIMSK|=(1<<TOIE0) // Cuando desborda
#define timer0_int_compare 				TIMSK|=(1<<OCIE0) // Cuando alcanza el valor indicado
#define timer0_int_off 					TIMSK&=~((1<<OCIE0)|(1<<TOIE0))

/* Detiene el timer */
#define timer0_stop 					TCCR0&=~((1<<CS02)|(1<<CS01)|(1<<CS00))

/* Divisores de frecuencia */
#define timer0_prescaler_1 				TCCR0&=~((1<<CS02)|(1<<CS01)); \
	                       	   	   	    TCCR0|=(1<<CS00)
#define timer0_prescaler_8 				TCCR0&=~((1<<CS02)|(1<<CS00)); \
	                       	   	   	   	TCCR0|=(1<<CS01)
#define timer0_prescaler_32 			TCCR0&=~(1<<CS02); \
	                        			TCCR0|=((1<<CS01)|(1<<CS00))
#define timer0_prescaler_64 			TCCR0|=(1<<CS02); \
	                        			TCCR0&=~((1<<CS01)|(1<<CS00))
#define timer0_prescaler_128 			TCCR0|=((1<<CS02)|(1<<CS00)); \
	                         	 	 	TCCR0&=~(1<<CS01)
#define timer0_prescaler_256 			TCCR0|=((1<<CS02)|(1<<CS01)); \
	                         	 	 	TCCR0&=~(1<<CS00)
#define timer0_prescaler_1024 			TCCR0|=((1<<CS02)|(1<<CS01)|(1<<CS00))


/* Definiciones timer 1 */

#define timer1_compare_valueA 			OCR1A
#define timer1_compare_valueB 			OCR1B
#define timer1_timer_value 				TCNT1

#define timer1_outA						DDRB |= (1<<5)
#define timer1_outB						DDRB |= (1<<6)

#define timer1_mode_normal 				TCCR1A&=~((1<<WGM11)|(1<<WGM10)); \
	                       	   	   	   	TCCR1B&=~((1<<WGM13)|(1<<WGM12))
#define timer1_mode_fastPWM_8bits 		TCCR1A|=(1<<COM1A1); \
	                              	  	TCCR1A&=~(1<<COM1A0); \
	                              	  	TCCR1A|=(1<<COM1B1); \
	                              	  	TCCR1A&=~(1<<COM1B0); \
	                              	  	TCCR1A&=~(1<<WGM11); \
	                              	  	TCCR1A|=(1<<WGM10); \
	                              	  	TCCR1B&=~(1<<WGM13); \
	                              	  	TCCR1B|=(1<<WGM12)
#define timer1_mode_fastPWM_9bits 		TCCR1A|=(1<<COM1A1); \
	                              	  	TCCR1A&=~(1<<COM1A0); \
	                              	  	TCCR1A|=(1<<COM1B1); \
	                              	  	TCCR1A&=~(1<<COM1B0); \
	                              	  	TCCR1A|=(1<<WGM11); \
	                              	  	TCCR1A&=~(1<<WGM10); \
	                              	  	TCCR1B&=~(1<<WGM13); \
	                              	  	TCCR1B|=(1<<WGM12)
#define timer1_mode_fastPWM_10bits 		TCCR1A|=(1<<COM1A1); \
	                               	    TCCR1A&=~(1<<COM1A0); \
	                               	    TCCR1A|=(1<<COM1B1); \
	                               	    TCCR1A&=~(1<<COM1B0); \
	                               	    TCCR1A|=((1<<WGM11)|(1<<WGM10)); \
	                               	    TCCR1B&=~(1<<WGM13); \
	                               	    TCCR1B|=(1<<WGM12)

#define timer1_int_overflow 			TIMSK|=(1<<TOIE1)
#define timer1_int_compare 				TIMSK|=((1<<OCIE1A)|(1<<OCIE1B))
#define timer1_int_input 				TIMSK|=(1<<TICIE1)
#define timer1_int_off 					TIMSK&=~((1<<TOIE1)|(1<<OCIE1B)|(1<<OCIE1A)|(1<<TICIE1))


#define timer1_stop 					TCCR1B&=((1<<CS12)|(1<<CS11)|(1<<CS10))

#define timer1_prescaler_1 				TCCR1B&=~((1<<CS12)|(1<<CS11)); \
	                       	   	   	    TCCR1B|=(1<<CS10)
#define timer1_prescaler_8 				TCCR1B&=~((1<<CS12)|(1<<CS10)); \
	                       	   	   	    TCCR1B|=(1<<CS11)
#define timer1_prescaler_64 			TCCR1B&=~(1<<CS12); \
	                        			TCCR1B|=((1<<CS11)|(CS10))
#define timer1_prescaler_256 			TCCR1B&=~((1<<CS11)|(1<<CS10)); \
	                         	 	 	TCCR1B|=(1<<CS12)
#define timer1_prescaler_1024 			TCCR1B&=~(1<<CS11); \
	                          	  	  	TCCR1B|=((1<<CS12)|(1<<CS10))

#define timer1_external_clk_falling 	TCCR1B&=~(1<<CS10); \
	                                	TCCR1B|=((1<<CS12)|(1<<CS11))
#define timer1_external_clk_rising 		TCCR1B|=((1<<CS12)|(1<<CS11)|(1<<CS10))


/* Definiciones timer 2 */

#define timer2_compare_value 			OCR2
#define timer2_timer_value 				TCNT2

#define timer2_out						DDRB |= (1<<7)

#define timer2_mode_normal 				TCCR2&=~((1<<WGM21)|(1<<WGM20))
#define timer2_mode_ctc_toggle 			TCCR2|=(1<<WGM21); \
	                           	   	   	TCCR2&=~(1<<WGM20); \
	                           	   	   	TCCR2&=~(1<<COM21); \
	                           	   	   	TCCR2|=(1<<COM20)
#define timer2_mode_ctc_clear 			TCCR2|=(1<<WGM21); \
	                          	  	  	TCCR2&=~(1<<WGM20); \
	                          	  	  	TCCR2|=(1<<COM21); \
	                          	  	  	TCCR2&=~(1<<COM20)
#define timer2_mode_ctc_set 			TCCR2|=(1<<WGM21); \
	                        			TCCR2&=~(1<<WGM20); \
	                        			TCCR2|=(1<<COM21); \
	                        			TCCR2|=(1<<COM20)
#define timer2_mode_fastPWM 			TCCR2|=((1<<WGM21)|(1<<WGM20)); \
	                        			TCCR2|=(1<<COM21); \
	                        			TCCR2&=~(1<<COM20)
#define timer2_mode_fastPWM_inverting 	TCCR2|=((1<<WGM21)|(1<<WGM20)); \
	                                  	TCCR2|=(1<<COM21)|(1<<COM20)

#define timer2_int_overflow 			TIMSK|=(1<<TOIE2)
#define timer2_int_compare 				TIMSK|=(1<<OCIE2)
#define timer2_int_off 					TIMSK&=~((1<<OCIE2)|(1<<TOIE2))

#define timer2_stop 					TCCR2&=~((1<<CS22)|(1<<CS21)|(1<<CS20))

#define timer2_prescaler_1 				TCCR2&=~((1<<CS22)|(1<<CS21)); \
	                       	   	   	    TCCR0|=(1<<CS20)
#define timer2_prescaler_8 				TCCR2&=~((1<<CS22)|(1<<CS20)); \
	                       	   	   	    TCCR2|=(1<<CS21)
#define timer2_prescaler_64 			TCCR2&=~(1<<CS22); \
	                        			TCCR2|=((1<<CS21)|(1<<CS20))
#define timer2_prescaler_256 			TCCR2|=(1<<CS22); \
	                         	 	 	TCCR2&=~((1<<CS21)|(1<<CS20))
#define timer2_prescaler_1024 			TCCR2|=((1<<CS22)|(1<<CS20)); \
	                          	  	  	TCCR2&=~(1<<CS21)

#define timer2_external_clk_falling 	TCCR2&=~(1<<CS20); \
	                                	TCCR2|=((1<<CS22)|(1<<CS21))
#define timer2_external_clk_rising 		TCCR2|=((1<<CS22)|(1<<CS21)|(1<<CS20))



#endif /* TIM_DEFS_H_ */
