/*
 * mydef.h
 *
 *  Created on: 4 jun. 2018
 *      Author: pc
 */

#ifndef MYDEF_H_
#define MYDEF_H_

#define set_bit(reg,bit) reg|=(1<<bit)          //pondra en 1 el bit elegido de la variable elegida.
#define clr_bit(reg,bit) reg&=~(1<<bit)         //pondra en 0 el bit elegido de la variable elegida.
#define tog_bit(reg,bit) reg^=(1<<bit)          //toggleara el bit elegido de la variable elegida.
#define set_output(ddr,bit) ddr|=(1<<bit)    //setea salida.
#define set_input(ddr,bit) ddr&=~(1<<bit)     //setea entrada.
#define get_bit(ddr,bit) ((ddr>>bit)&1)          //devuelve el bit elegido de la variable elegida (para if).
#define clr_reg(reg) reg&=0                     //limpia el registro elegido.
#define set_reg(reg) reg|=255

#define led4_out set_output(DDRD,4)
#define led5_out set_output(DDRD,5)
#define	led6_out set_output(DDRD,6)
#define	led7_out set_output(DDRD,7)

#define	led4_off set_bit(PORTD,4)
#define	led5_off set_bit(PORTD,5)
#define	led6_off set_bit(PORTD,6)
#define	led7_off set_bit(PORTD,7)

#define	led4_on clr_bit(PORTD,4)
#define	led5_on clr_bit(PORTD,5)
#define	led6_on clr_bit(PORTD,6)
#define	led7_on clr_bit(PORTD,7)

#define	led4_tog tog_bit(DDRD,4)
#define	led5_tog tog_bit(DDRD,5)
#define	led6_tog tog_bit(DDRD,6)
#define	led7_tog tog_bit(DDRD,7)

#define set_sw7  set_input(DDRE,7)
#define set_sw6  set_input(DDRE,6)
#define	set_sw5  set_input(DDRE,5)

#define get_sw7 get_bit(PINE,7)
#define get_sw6 get_bit(PINE,6)
#define	get_sw5 get_bit(PINE,5)

#define enable_interrupt_INT0	set_bit(EIMSK,INT0)
#define enable_interrupt_INT1	set_bit(EIMSK,INT1)
#define enable_interrupt_INT2	set_bit(EIMSK,INT2)
#define enable_interrupt_INT3	set_bit(EIMSK,INT3)
#define enable_interrupt_INT4	set_bit(EIMSK,INT4)
#define enable_interrupt_INT5	set_bit(EIMSK,INT5)
#define enable_interrupt_INT6	set_bit(EIMSK,INT6)
#define enable_interrupt_INT7	set_bit(EIMSK,INT7)

#define disable_interrupt_INT0	clr_bit(EIMSK,INT0)
#define disable_interrupt_INT1	clr_bit(EIMSK,INT1)
#define disable_interrupt_INT2	clr_bit(EIMSK,INT2)
#define disable_interrupt_INT3	clr_bit(EIMSK,INT3)
#define disable_interrupt_INT4	clr_bit(EIMSK,INT4)
#define disable_interrupt_INT5	clr_bit(EIMSK,INT5)
#define disable_interrupt_INT6	clr_bit(EIMSK,INT6)
#define disable_interrupt_INT7	clr_bit(EIMSK,INT7)


#define INT0_falling_edge		EICRA|=(1<<ISC01); \
								EICRA&=~(1<<ISC00)
#define INT1_falling_edge		EICRA|=(1<<ISC11); \
								EICRA&=~(1<<ISC10)
#define INT2_falling_edge		EICRA|=(1<<ISC21); \
								EICRA&=~(1<<ISC20)
#define INT3_falling_edge		EICRA|=(1<<ISC31); \
								EICRA&=~(1<<ISC30)
#define INT4_falling_edge		EICRB|=(1<<ISC41); \
								EICRB&=~(1<<ISC40)
#define INT5_falling_edge		EICRB|=(1<<ISC51); \
								EICRB&=~(1<<ISC50)
#define INT6_falling_edge		EICRB|=(1<<ISC61); \
								EICRB&=~(1<<ISC60)
#define INT7_falling_edge		EICRB|=(1<<ISC71); \
								EICRB&=~(1<<ISC70)

#define INT0_rising_edge		EICRA|=(1<<ISC01)|(1<<ISC00)
#define INT1_rising_edge		EICRA|=(1<<ISC11)|(1<<ISC10)
#define INT2_rising_edge		EICRA|=(1<<ISC21)|(1<<ISC20)
#define INT3_rising_edge		EICRA|=(1<<ISC31)|(1<<ISC30)
#define INT4_rising_edge		EICRB|=(1<<ISC41)|(1<<ISC40)
#define INT5_rising_edge		EICRB|=(1<<ISC51)|(1<<ISC50)
#define INT6_rising_edge		EICRB|=(1<<ISC61)|(1<<ISC60)
#define INT7_rising_edge		EICRB|=(1<<ISC71)|(1<<ISC70)


#define enc 0
#define apa 1

#endif /* MYDEF_H_ */
