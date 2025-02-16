/*
 * MDE.h
 *
 *  Created on: 6 nov. 2024
 *      Author: matia
 */

#ifndef MDE_H_
#define MDE_H_

#define TIEMPO_VALVULA_ACTIVA
#define TIEMPO_DERECHA_ON             	10
#define TIEMPO_IZQUIERDA_ON           	10
#define TIEMPO_CENTRIFUGADO_ON        	10
#define TIEMPO_CENTRIFUGADO_LARGO_ON  	20
#define PIN_VALV_PRELAV 				0,0
#define PIN_VALV_LAV					0,1
#define PIN_VALV_SUAVIZANTE				0,2
#define PIN_VALV_VACIADO				0,3
#define PIN_SENSOR_NIVEL				0,5
#define PIN_MOTOR_DERECHO				0,6
#define PIN_MOTOR_IZQUIERDO				0,7
#define PIN_MOTOR_CENTRIFUGADO			0,8

//ESTADOS MOTOR
#define INICIO 							0
#define GIRO_IZQUIERDA					1
#define GIRO_DERECHA					2
#define CENTRIFUGADO					3
#define FINAL							4

#define OUT								1
#define IN								0

#define ON								1
#define OFF								0

#define ON_VALV							0
#define OFF_VALV						1

#define LLENO							5160
#define VACIO							500

#define NOMBRE_RED						"iPhone de Matias Leonel"
#define CONTRA_RED						"tute1234"
#define DIRECCION_IP					""
//typedef enum {INICIO, GIRO_DERECHA, GIRO_IZQUIERDA, CENTRIFUGADO, FIN} EstadoMotor;
//static EstadoMotor estadoMotor 			= INICIO;
//static uint8_t flag_soloCentrifugado 	= 0;
//static uint8_t contadorGiros 			= 0;
//static uint8_t contadorCiclos 			= 0;
//
//static uint8_t flag_lavadoCompleto 			= 0;
//static uint8_t flag_lavadoRapido   			= 0;
//static uint8_t flag_timerValvula   			= 0;
//static uint8_t estadoValvula       			= 0;
//static uint8_t contValvula         			= 0;

void MdE_Valvulas(void);
void CerrarTodasValvulas(void);
void flagTimerValvula(void);
void GestionarValvulas(void);
void verificarNivelAgua(); // Prototipo

void IniciarMotor(uint32_t tiempo, void (*callback)(void));
void CambiarEstadoMotor(void);
void MdE_Motor(void);
void finalizarTimerMotor(void);
void finalizarTimerValvulas(void);

void MdE_ESP32(void);

#endif /* MDE_H_ */
