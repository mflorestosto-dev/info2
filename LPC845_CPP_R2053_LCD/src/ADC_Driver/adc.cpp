/*
 * ADCDriver.cpp
 *
 *  Created on: 10 dic. 2024
 *      Author: SinstuTror
 */

#include "Defines.h"

uint32_t	ADC_Cuentas[CANTIDAD_CANALES_ADC];
uint32_t	ADC_Medicion[CANTIDAD_CANALES_ADC];
char buf[25];
TIMERSW 	TimerADC;

/*****************************************************************************
** Function name:		ADC_Init
**
** Descriptions:		Inicializamos el conversor analogico digital
**
** parameters:			void
** Returned value:		void
**
*****************************************************************************/
void ADC_Inicializar(void)
{
	SYSCON->PDRUNCFG &= ~(1 << 4);				// Enciendo el modulo del ADC que esta apagado por default

	SYSCON->SYSAHBCLKCTRL0 |= (1 << 24);		// 24 = ADC

	SWM0->PINENABLE0 &= ~(1 << 15);				// ADC_0 enabled on pin PIO0_7

	ADC0->CTRL 	= (1)							// CLKDIV se recomienda el minimo si no hay una necesidad de velocidad
				| (0 << 8)						// ASYNCMODE
				| (0 << 10)						// LPWRMODE
				| (0 << 30);					// CALMODE

	// A/D Conversion Sequence A Control Register

	ADC0->SEQ_CTRL[0] 	= (0)					// CANAL, lo voy a seleccionar en el momento de disparar la conversion
						| (0 << 12)				// TRIGGER
												//	Input 	Source 					Description
												//	0 								- No hardware trigger.
												//	1 		PININT0_IRQ 			GPIO_INT interrupt 0.
												//	2 		PININT1_IRQ 			GPIO_INT interrupt 1.
												//	3 		SCT0_OUT3 				SCT output 3.
												//	4 		SCT0_OUT4 				SCT output 4.
												//	5 		T0_MAT3 				CTIMER match 3.
												//	6 		CMP0_OUT_ADC 			Analog comparator output.
												//	7 		GPIO_INT_BMAT 			GPIO_INT bmatch.
												//	8 		ARM_TXEV 				ARM core TXEV event.
						| (1 << 18)				// TRIGPOL					0=FLANCO NEGATIVO DE LOS TRIGGERS 1=FLANCO POSITIVO DE LOS TRIGGERS
						| (0 << 19)				// SYNCBYPASS
						| (0 << 26)				// START OF CONVERSION		1=SOC 0=STOP
						| (0 << 29)				// SINGLESTEP
						| (0 << 30)				// MODE						0
						| (1 << 31);			// SEQA_ENA					0=SECUENCIA A DESHABILITADA 1=HABILITADA

	ADC0->INTEN = (1 << 0);						// Sequence A interrupt enable

	NVIC->ISER[0] = (1 << 16); 					/* enable interrupt ISE_ADC_SEQA*/

	TimerADC.Start(100, 100, ADC_Start);		// Configuro el timerADC para llamar a la funcion ADC_Disparo cada 100ms

}

void ADC_Start(void){
	ADC_Disparo(1);
}

void ADC_Disparo(uint32_t Canal)
{
	ADC0->SEQ_CTRL[0] 	= (1 << Canal)		// CANAL, seleccionar al momento de disparar la conversion
						| (0 << 12)		// TRIGGER
						| (1 << 18)		// TRIGPOL								0=FLANCO NEGATIVO DE LOS TRIGGERS 1=FLANCO POSITIVO DE LOS TRIGGERS
						| (0 << 19)		// SYNCBYPASS
						| (1 << 26)		// START OF CONVERSION	1=SOC 0=STOP
						| (0 << 29)		// SINGLESTEP
						| (0 << 30)		// MODE
						| (1 << 31);	// SEQA_ENA	0=SECUENCIA A DESHABILITADA 1=HABILITADA
}

void ADC_SEQA_IRQHandler(void)
{
	// Leo el registro de lectura de ADC
	uint32_t	Temporal = ADC0->SEQ_GDAT[0];

	// Parseo el canal del ADC leido
	uint32_t 	Canal = (Temporal >> 26) & 0x0f;
	uint32_t	Valor_Temporal;

	// Verifico si es correcto
	if(Canal >= CANTIDAD_CANALES_ADC)
		return;

	// Parseo el valor leido por el ADC
	Valor_Temporal = (Temporal >> 4) & 0xfff;

	ADC_Cuentas[Canal] = Valor_Temporal;

	Valor_Temporal = (Valor_Temporal * 10000) / 4096;

	ADC_Medicion[Canal] = Valor_Temporal;

	//sprintf(buf,"ADC Canal %d= %d\r\n", Canal, Valor_Temporal);
	//UART0_Send((uint8_t*)buf, 0);
}

uint32_t ADC_ReadChannel(uint32_t canal) {
    if (canal >= CANTIDAD_CANALES_ADC) {
        return 0; // Valor inválido si el canal está fuera de rango
    }
    return ADC_Medicion[canal]; // Devuelve la medición del canal solicitado
}

