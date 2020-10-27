/* ###################################################################
**     Filename    : ProcessorExpert.c
**     Project     : ProcessorExpert
**     Processor   : MC9S08QE128CLK
**     Version     : Driver 01.12
**     Compiler    : CodeWarrior HCS08 C Compiler
**     Date/Time   : 2014-02-18, 15:36, # CodeGen: 0
**     Abstract    :
**         Main module.
**         This module contains user's application code.
**     Settings    :
**     Contents    :
**         No public methods
**
** ###################################################################*/
/*!
** @file ProcessorExpert.c
** @version 01.12
** @brief
**         Main module.
**         This module contains user's application code.
*/         
/*!
**  @addtogroup ProcessorExpert_module ProcessorExpert module documentation
**  @{
*/         
/* MODULE ProcessorExpert */


/* Including needed modules to compile this module/procedure */
#include "Cpu.h"
#include "Events.h"
#include "TI1.h"
#include "AS1.h"
#include "AD1.h"
/* Include shared modules, which are used for whole project */
#include "PE_Types.h"
#include "PE_Error.h"
#include "PE_Const.h"
#include "IO_Map.h"

/* User includes (#include below this line is not maintained by Processor Expert) */

unsigned char estado = ESPERAR;

typedef union{
unsigned char u8[2];
unsigned int u16;
}VALOR;

volatile VALOR vADC1,vADC2,d1,d2,d3,d4,d5,d6,d7,d8;			//Variable que guarda el valor del ADC sin pasar por el protocolo
volatile VALOR tr1, tr2, tr3, tr4;	//Variables que servirán para ajustar los parametros a ser enviados mediante el protocolo

unsigned char pADC[6] = {0xF3,0x00,0x00,0x00,0x00,0x00};	//Trama a enviar por serial
 char err;	//Variable para comprobar errores en el envio
 int i=0;


void main(void)
{
  /* Write your local variable definition here */

	
  /*** Processor Expert internal initialization. DON'T REMOVE THIS CODE!!! ***/
  PE_low_level_init();
  /*** End of Processor Expert internal initialization.                    ***/

  /* Write your code here */
    
  /* For example: for(;;) { } */
  //AD1_Start();
  for(;;){

  	switch (estado){
  		case ESPERAR:
  			break;
  			
  		case MEDIR:
  			//Se miden todos los canales del ADC
  			AD1_MeasureChan(TRUE,0);
  			AD1_GetChanValue16(0,&vADC1.u16);
  			AD1_MeasureChan(TRUE,1);
  			AD1_GetChanValue16(1,&vADC2.u16);
  			AD1_MeasureChan(TRUE,2);
  			AD1_GetChanValue16(2,&d1.u16);
  			AD1_MeasureChan(TRUE,3);
  			AD1_GetChanValue16(3,&d2.u16);
  			AD1_MeasureChan(TRUE,4);
  			AD1_GetChanValue16(4,&d3.u16);
  			AD1_MeasureChan(TRUE,5);
  			AD1_GetChanValue16(5,&d4.u16);
  			AD1_MeasureChan(TRUE,6);
  			AD1_GetChanValue16(6,&d5.u16);
  			AD1_MeasureChan(TRUE,7);
  			AD1_GetChanValue16(7,&d6.u16);
  			AD1_MeasureChan(TRUE,8);
  			AD1_GetChanValue16(8,&d7.u16);
  			AD1_MeasureChan(TRUE,9);
  			AD1_GetChanValue16(9,&d8.u16);
  			
  			
  			//Primer analogico
  			vADC1.u16 = vADC1.u16 >> 4;
  			tr1.u16 = vADC1.u16 & 0x007F;
  			tr2.u16 = vADC1.u16 >> 7;
  			tr2.u16 = tr2.u16 & 0x1F;
  			
  			//Segundo segundo analogico
  			vADC2.u16 = vADC2.u16 >> 4;
  			tr3.u16 = vADC2.u16 & 0x007F;
  			tr4.u16 = vADC2.u16 >> 7;
  			tr4.u16 = tr4.u16 & 0x1F;
  			
  			pADC[1] = tr2.u16;
  			pADC[2] = tr1.u16;
  			
  			pADC[3] = tr4.u16;
  			pADC[4] = tr3.u16;
  			
  			//Digitales
  			
  			d1.u16 = d1.u16 >> 4;
  			d2.u16 = d2.u16 >> 4;
  			d3.u16 = d3.u16 >> 4;
  			d4.u16 = d4.u16 >> 4;
  			d5.u16 = d5.u16 >> 4;
  			d6.u16 = d6.u16 >> 4;
  			d7.u16 = d7.u16 >> 4;
  			d8.u16 = d8.u16 >> 4;
  	
  			
  			
  			//Se opera el byte de digitales, fijando los umbrales de decision
  			if(d1.u16 >= 3040){
  				pADC[5] = pADC[5] | 0x01;
  			}
  			else if(d1.u16 < 3040){
  				pADC[5] = pADC[5] & 0xFE;
  			}
  					
  			if(d2.u16 >= 2233){
  				pADC[5] = pADC[5] | 0x02;
  			}
  			else if(d2.u16 < 2233){
  				pADC[5] = pADC[5] & 0xFD;
  			}
  			
  			if(d3.u16 >= 2233){
  				pADC[5] = pADC[5] | 0x04;
			}
			else if(d3.u16 < 2233){
				pADC[5] = pADC[5] & 0xFB;
			}
  			
  			if(d4.u16 >= 2482){
  				pADC[5] = pADC[5] | 0x08;
			}
			else if(d4.u16 < 2482){
				pADC[5] = pADC[5] & 0xF7;
			}
  			
  			if(d5.u16 >= 1861){
  				pADC[5] = pADC[5] | 0x10;
			}
			else if(d5.u16 < 1861){
				pADC[5] = pADC[5] & 0xEF;
			}
  			
  			if(d6.u16 >= 1861){
  				pADC[5] = pADC[5] | 0x20;
			}
			else if(d6.u16 < 1861){
				pADC[5] = pADC[5] & 0xDF;
			}
  			
  			if(d7.u16 >= 1985){
  				pADC[5] = pADC[5] | 0x40;
			}
			else if(d7.u16 < 1985){
				pADC[5] = pADC[5] & 0xBF;
			}
  			
  			if(d8.u16 >= 2482){
  				pADC[5] = pADC[5] | 0x80;
			}
			else if(d8.u16 < 2482){
				pADC[5] = pADC[5] & 0x7F;
			}
  			estado = ENVIAR;
  			break;
  	
  		case ENVIAR:
  			//Se envia la trama, verificando error en el envio
  			for(i=0;i<6;i++){
				do{
					err = AS1_SendChar(pADC[i]);
				}while((err != ERR_OK));
  			}	
  			
  			estado = ESPERAR;
  			
  			break;
  			
  		default:
  			break;
  	
  	}
  }
  
  /*** Don't write any code pass this line, or it will be deleted during code generation. ***/
  /*** Processor Expert end of main routine. DON'T MODIFY THIS CODE!!! ***/
  for(;;){}
  /*** Processor Expert end of main routine. DON'T WRITE CODE BELOW!!! ***/
} /*** End of main routine. DO NOT MODIFY THIS TEXT!!! ***/

/* END ProcessorExpert */
/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.3 [05.08]
**     for the Freescale HCS08 series of microcontrollers.
**
** ###################################################################
*/
