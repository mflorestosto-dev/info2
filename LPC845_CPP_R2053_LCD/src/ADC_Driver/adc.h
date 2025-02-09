/*
 * ADCDriver.h
 *
 *  Created on: 10 dic. 2024
 *      Author: SinstuTror
 */

#ifndef DRIVER_ADC_ADCDRIVER_H_
#define DRIVER_ADC_ADCDRIVER_H_

#define 	CANTIDAD_CANALES_ADC	2

void ADC_Inicializar(void);
uint32_t ADC_ReadChannel(uint32_t canal);
void ADC_Disparo(uint32_t Canal);
void ADC_Start(void);

extern "C" {
void ADC_SEQA_IRQHandler(void);
}


#endif /* DRIVER_ADC_ADCDRIVER_H_ */
