#include "Defines.h"

GPIOPWM::GPIOPWM(uint32_t _port, uint32_t _pin, uint32_t _dutyInit) : Gpio(_port, _pin, 1)
{
	duty = _dutyInit;
}

void GPIOPWM::Set(uint32_t duty_conf)
{
  if(duty_conf > 10)
    duty_conf = 10;

  duty = duty_conf;
}

void GPIOPWM::Work(void)
{
  // 0 1 2 3 4 5 6 7 8 9
  //       d
  // 11111110000000000000

  static char div = 0;

  if(div > duty)
  {
	  Gpio::Set(0);
  }
  else
  {
	  Gpio::Set(1);
  }

  div++;
  if(div >= 10)
    div = 0;
}
