/*
===============================================================================
 Name        : main.c
 Author      : $(author)
 Version     :
 Copyright   : $(copyright)
 Description : main definition
===============================================================================
*/


#include "Defines.h"

//Definiciones de pines
Gpio valvCarga		(0, 13, OUT);
Gpio valvSuavizante	(0, 14, OUT);
Gpio valvDesagote	(0, 15, OUT);
GPIOPWM Motor		(0, 18, 10);
Gpio giroDerecha	(0, 19, OUT);
Gpio giroIzquierda	(0, 20, OUT);
Gpio ledVerde 		(1,0,OUT);
Gpio ledRojo 		(1,2,OUT);

//GPIOS RFID
Gpio gpioNSS		(nss, OUT);
Gpio gpioNRST		(nrst, OUT);
Gpio gpioMISO		(miso, IN);
Gpio gpioMOSI		(mosi, OUT);
Gpio gpioSCK		(sck, OUT);

//TIMERSW timerLlenado;
SYSTICK sis			(1000);
TIMERSW timerMotor, timerValvulas;

// Sensor de nivel de agua
WaterLevelSensor waterSensor(1, SENSOR_AGUA_LLENO, SENSOR_AGUA_VACIO); // P0.6 es el pin

// Flags de control
bool flag_lavadoCompleto 	= false;
bool flag_lavadoRapido 		= false;
bool flag_soloCentrifugado 	= false;
bool flag_conexionExitosa 	= false;
bool flag_tramaRecibida 	= 0; // Bandera para saber si se recibió una trama válida
char modoLavadoSeleccionado = 0;         // Guardará el modo de lavado ('R', 'N', 'C')

int main(void) {
   App();
}





