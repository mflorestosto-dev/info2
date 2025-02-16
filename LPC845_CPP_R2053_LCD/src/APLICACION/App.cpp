/*
 * App.cpp
 *
 *  Created on: 20 dic. 2024
 *      Author: matia
 */

#include "Defines.h"
extern bool flag_lavadoRapido;
extern bool flag_lavadoCompleto;
extern bool flag_soloCentrifugado;
extern bool	flag_tramaRecibida;
extern bool flag_conexionExitosa;
extern bool ValvulasTerminadas;
extern bool MotorTerminado;
extern bool paradaEmergencia;
extern GPIOPWM Motor;
extern Gpio 	giroDerecha;
extern Gpio 	giroIzquierda;
extern Gpio 	ledVerde;
extern Gpio 	ledRojo;
extern Gpio 	valvCarga;
extern Gpio 	valvDesagote;
extern uint8_t estadoMotor;
extern uint8_t estadoValvulas;
extern WaterLevelSensor waterSensor;
//extern WaterLevelSensor waterSensor;

uint32_t medicion = 0;


TIMERSW t;
void prueba (void);
uint8_t App(void){
	uint8_t caracter=0;
	Inicializacion();
	valvCarga.Set(ON);
	valvDesagote.Set(ON);
	while(1){
		MdeE();
		leerYEnviarRFID();
	    caracter = LeerBufferRX();
	    if(caracter != 255) analisisdeTramas(caracter);
	    if(ValvulasTerminadas == 1 && MotorTerminado == 1){
	    	UART0_Send((uint8_t *)">L,T,1<", 9);
	    	ValvulasTerminadas 	= 0;
	    	MotorTerminado 		= 0;
	    }
	}
	return 0;
}

void prueba(void){

	valvCarga.Set(OFF_VALV);
//	valvSuavizante.Set(ON_VALV);
//		for(int i=0; i<100000; i++);
//		valvSuavizante.Set(OFF_VALV);
//		valvDesagote.Set(ON_VALV);
//			for(int i=0; i<100000; i++);
//			valvDesagote.Set(OFF_VALV);
}

void MdeE(void){
	MdE_Motor();
	//MdE_ESP32();
	MdE_Valvulas();
}

void analisisdeTramas(uint8_t dato){
	AnalizarTramaUart0(dato);
	//AnalizarTramaConexion(dato);
}

void Inicializacion() {
	ADC_Inicializar();
    //Inicializar_PLL();
    UART0_Init(9600);
    InitRC522();
    waterSensor.init();
    CerrarTodasValvulas();

    giroDerecha.Set(OFF);
    giroIzquierda.Set(OFF);
    Motor.Set(0);

    ledVerde.Set(1);
    ledRojo.Set(1);

    //timerLlenado.Stop();
}

uint8_t LeerBufferRX( void )
{
	uint8_t caracter;

	caracter = UART0_PopRx();

		if ( caracter == NO_KEY )
			return NO_KEY;

	return caracter;
}

void AnalizarTramaConexion(uint8_t dato) {
    static uint8_t estado = 0;

    switch (estado) {
        case 0:  // Esperar la 'O'
            if (dato == 'O') {
                estado = 1;
            } else {
                estado = 0;  // Reiniciar si no es 'O'
            }
            break;

        case 1:  // Esperar la 'K'
            if (dato == 'K') {
                flag_conexionExitosa = true;  // Activar flag
                estado = 0;  // Reiniciar para recibir más tramas
            } else {
                estado = 0;  // Reiniciar si no es 'K'
            }
            break;

        default:
            estado = 0;
            break;
    }
}


//void AnalizarTramaUart0(uint8_t dato) {
//    static uint8_t estado = 0;
//    static char modoLavado = 0;
//
//    switch (estado) {
//    case 0: // Inicio de trama
//        if (dato == '>') {
//            estado = 1;
//        } else {
//            estado = 0;
//        }
//        break;
//
//    case 1: // Verificar comando L (Lavado)
//        if (dato == 'L') {
//            estado = 2;
//        } else {
//            estado = 0;
//        }
//        break;
//
//    case 2: // Coma después de L
//        if (dato == ',') {
//            estado = 3;
//        } else {
//            estado = 0;
//        }
//        break;
//
//    case 3: // Verificar modo de lavado
//        if (dato == 'R' || dato == 'N' || dato == 'C') {
//            modoLavado = dato; // Guardar el modo de lavado
//            estado = 4;
//        } else {
//            estado = 0;
//        }
//        break;
//
//    case 4: // Fin de trama <
//        if (dato == '<') {
//            // Confirmación y ejecución del modo de lavado
//            if (modoLavado == 'R') { // Lavado Rápido
//                flag_lavadoRapido = 1;
//                UART0_Send((uint8_t *)">L,R,ACK<", 10);
//            } else if (modoLavado == 'N') { // Lavado Normal
//                flag_lavadoCompleto = 1;
//                UART0_Send((uint8_t *)">L,N,ACK<", 10);
//            } else if (modoLavado == 'C') { // Centrifugado
//                flag_soloCentrifugado = 1;
//                UART0_Send((uint8_t *)">L,C,ACK<", 10);
//            }
//
//            // Reiniciar estado y modo
//            estado = 0;
//            flag_tramaRecibida = 1; // Activar la bandera de trama recibida
//        } else {
//            estado = 0;
//        }
//        break;
//
//    default:
//        estado = 0;
//        break;
//    }
//}


void AnalizarTramaUart0(uint8_t dato) {
    static uint8_t estado = 0, chk = 0;
    static char modoLavado = 0;

    switch (estado) {
    case 0: // Inicio de trama
        if (dato == '>') {
            estado = 1;
            chk = dato;
        } else {
            estado = 0;
        }
        break;

    case 1: // Verificar comando L (Lavado)
        if (dato == 'L') {
            estado = 2;
            chk ^= dato;
        } else {
            estado = 0;
        }
        break;

    case 2: // Coma después de L
        if (dato == ',') {
            estado = 3;
            chk ^= dato;
        } else {
            estado = 0;
        }
        break;

    case 3: // Verificar modo de lavado
        if (dato == 'R' || dato == 'N' || dato == 'C' || dato == 'P') {
            modoLavado = dato; // Guardar el modo de lavado
            chk ^= dato;
            estado = 4;
        } else {
            estado = 0;
        }
        break;

    case 4: // Coma después del modo
        if (dato == ',') {
            estado = 5;
            chk ^= dato;
        } else {
            estado = 0;
        }
        break;

    case 5: // Chequeo del Checksum
        if (dato == chk) {
            estado = 6; // Pasar al siguiente estado
        } else {
            UART0_Send((uint8_t *)">ERR,CHK<", 9); // Enviar error de checksum
            estado = 0;
        }
        break;

    case 6: // Fin de trama <
        if (dato == '<') {
        	if(modoLavado == 'P'){
        		paradaEmergencia = 1;
        		UART0_Send((uint8_t *)">L,P,1<", 7);

        	}
            // Confirmación y ejecución del modo de lavado
            if (modoLavado == 'R') { // Lavado Rápido
                flag_lavadoRapido = 1;
                UART0_Send((uint8_t *)">L,A,1<", 7);
            }
            if (modoLavado == 'N') { // Lavado Normal
                flag_lavadoCompleto = 1;
                UART0_Send((uint8_t *)">L,A,1<", 7);
            }
            if (modoLavado == 'C') { // Centrifugado
                flag_soloCentrifugado = 1;
                UART0_Send((uint8_t *)">L,A,1<", 7);
            }
            // Reiniciar estado y checksum
            estado = 0;
            chk = 0;
            flag_tramaRecibida = 1; // Activar la bandera
        } else {
            estado = 0;
        }
        break;

    default:
        estado = 0;
        break;
    }
}
