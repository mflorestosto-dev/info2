/*
 * GpioPWM.h
 *
 *  Created on: 31 ago. 2024
 *      Author: matia
*/
#ifndef GPIOPWM_DRIVER_GPIOPWM_H_
#define GPIOPWM_DRIVER_GPIOPWM_H_

#include "Defines.h"

class GPIOPWM: public Gpio
{
	private:
		uint8_t duty = 0;
	public:
		GPIOPWM(uint32_t _port, uint32_t _pin, uint32_t _dutyInit);

		void Set(uint32_t duty_conf);
		void Work(void);
};



#endif /* GPIOPWM_DRIVER_GPIOPWM_H_ */
