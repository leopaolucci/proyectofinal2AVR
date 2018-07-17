/*
 * BH1750.h
 *
 *  Created on: 12 jul. 2018
 *      Author: Administrador
 */

#ifndef BH1750_H_
#define BH1750_H_

/*COMANDOS*/

#define PW_DOWN 0X00					//comando POWER DOWN
#define	PW_ON	0X01					//comando POWER ON
#define	RESET	0X07					//comando RESET
#define	CONT_HRES_M1	0X10			//modo de lectura continua de alta resolucion
#define	CONT_HRES_M2	0X11			//modo 2 de lectura contunua de alta resolucion
#define	CONT_LRES_M1	0X13			//modo de lectura continua de baja resolucion
#define	OT_HRES_M1	0X20				//leer 1 sola vez en modo de alta resolucion
#define	OT_HRES_M2	0X21				//leer 1 sola vez en modo 2 de alta resolucion
#define	OT_LRES_M1	0X23				//leer 1 sola vez en modo de baja resolucion

/*DIRECCIONES DE ESCLAVO + MODO*/

#define	ADDRL_R		0X47				//mando direccion del esclavo en modo lectura
#define	ADDRL_W		0X46				//mando direccion del esclavo en modo escritura
#define	ADDRH_R		0XB9				//mando direccion del esclavo en modo lectura
#define	ADDRH_W		0XB8				//mando direccion del esclavo en modo escritura

/*FUNCIONES PROPIAS*/





#endif /* BH1750_H_ */
