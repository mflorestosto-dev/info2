#include "Defines.h"

extern WaterLevelSensor waterSensor;// Sensor de nivel de agua declarado externamente
extern Gpio 			valvCarga;
extern Gpio 			valvDesagote;
extern GPIOPWM 			Motor;
extern Gpio 			giroDerecha;
extern Gpio 			giroIzquierda;
extern Gpio 			nivelAgua;
extern Gpio				ledVerde;
extern Gpio 			ledRojo;
extern TIMERSW 			timerValvulas;         // Temporizador para controlar tiempo de llenado
extern TIMERSW 			timerMotor;

extern bool 			flag_lavadoRapido;
extern bool 			flag_lavadoCompleto;
extern bool 			flag_soloCentrifugado;
extern bool				flag_tramaRecibida;
extern bool 			flag_conexionExitosa;
extern bool 			ValvulasTerminadas;
extern bool				MotorTerminado;
extern bool 			paradaEmergencia;
uint8_t 				estadoValvulas			= 0;
uint8_t 				estadoMotor 			= 0;
uint8_t					estadoESP32				= 0;
uint8_t 				contLC					= 0;
uint8_t 				contLR					= 0;
bool 					flag_timerValvula 		= 0;
bool 					flag_timerMotor			= 0;
uint8_t 				contadorCiclos 			= 0;
uint8_t 				contadorGiros 			= 0;

char nombreRed[] = "iPhone de Matias Leonel";
char contraRed[] = "tute1234";
char ipRed[]	 = "";

// ==========================
//  Máquina de Estados - Válvulas
// ==========================
void MdE_Valvulas(void) {
    switch (estadoValvulas) {
        case 0: // Estado inicial: Verificar flags de lavado
            if (paradaEmergencia == 1) {
                estadoValvulas = 0;
                CerrarTodasValvulas();
            }
            if (flag_lavadoCompleto == ON || flag_lavadoRapido == ON) {
                valvCarga.Set(ON); // Activar válvula de carga
                timerValvulas.Start(10000, 0, finalizarTimerValvulas);
                estadoValvulas = 1; // Pasar al estado de llenado
            }
            break;

        case 1: // Estado de llenado
            if (paradaEmergencia == 1) {
                estadoValvulas = 0;
                CerrarTodasValvulas();
            }
            if (waterSensor.isFull() || flag_timerValvula == 1) { // Nivel lleno o tiempo excedido
                valvCarga.Set(OFF); // Cerrar válvula de carga
                flag_timerValvula = 0;
                estadoValvulas = 2; // Pasar al estado de espera del motor
            }
            break;

        case 2: // Estado de espera del motor
            if (paradaEmergencia == 1) {
                estadoValvulas = 0;
                CerrarTodasValvulas();
            }
            if ((contadorGiros == 4 && flag_lavadoRapido) || (contadorGiros == 6 && flag_lavadoCompleto)) {
                valvDesagote.Set(ON); // Activar válvula de descarga
                timerValvulas.Start(10000, 0, finalizarTimerValvulas);
                estadoValvulas = 3; // Pasar al estado de descarga
            }
            break;

        case 3: // Estado de descarga
            if (paradaEmergencia == 1) {
                estadoValvulas = 0;
                CerrarTodasValvulas();
            }
            if ((waterSensor.isEmpty() || flag_timerValvula == 1)) { // Nivel vacío o tiempo excedido
                valvDesagote.Set(OFF); // Cerrar válvula de descarga
                flag_timerValvula = 0;
                estadoValvulas = 4; // Reiniciar al estado inicial
            }
            break;
        case 4:
            if (paradaEmergencia == 1) {
                estadoValvulas = 0;
                CerrarTodasValvulas();
            }
            if (contadorCiclos >= 3) { // Nivel vacío o tiempo excedido
                CerrarTodasValvulas();
                flag_timerValvula = 0;
                ValvulasTerminadas = 1;
                estadoValvulas = 0; // Reiniciar al estado inicial
            }
            else{
            	valvCarga.Set(ON);
            	timerValvulas.Start(10000, 0, finalizarTimerValvulas);
            	estadoValvulas = 1;
            }
            break;

        default:
            estadoValvulas = 0; // Por defecto se reinicia al estado inicial
            break;
    }
}


// Función para cerrar todas las válvulas
void CerrarTodasValvulas(void) {
    valvCarga.Set(OFF);
    valvDesagote.Set(OFF);
}

void finalizarTimerValvulas(void){
	flag_timerValvula = 1;
}

// ==========================
//  Máquina de Estados - Motor
// ==========================
void MdE_Motor(void) {
    switch (estadoMotor) {
        case INICIO:
            // Verificar flags de inicio del ciclo
            if (flag_lavadoCompleto || flag_lavadoRapido || flag_soloCentrifugado) {

                Motor.Set(1);  // Iniciar motor con baja velocidad

                if (flag_lavadoCompleto || flag_lavadoRapido) {
                    giroIzquierda.Set(OFF);  // Asegurarse de que el otro pin esté apagado
                    giroDerecha.Set(ON);     // Iniciar giro derecho
                    timerMotor.Start(10000, 0, finalizarTimerMotor);
                    estadoMotor = GIRO_DERECHA;
                } else if (flag_soloCentrifugado) {
                    giroIzquierda.Set(OFF);
                    giroDerecha.Set(ON);     // Iniciar centrifugado directo
                    Motor.Set(10);           // Máxima velocidad
                    timerMotor.Start(20000, 0, finalizarTimerMotor);
                    estadoMotor = CENTRIFUGADO;
                }
            }
            break;

        case GIRO_DERECHA:
            if (flag_timerMotor) {
                giroDerecha.Set(OFF);       // Apagar giro derecho antes de girar a la izquierda
                giroIzquierda.Set(ON);      // Iniciar giro izquierdo
                timerMotor.Start(10000, 0, finalizarTimerMotor);
                flag_timerMotor = 0;
                estadoMotor = GIRO_IZQUIERDA;
                contadorGiros++;
            }
            break;

        case GIRO_IZQUIERDA:
            if (flag_timerMotor) {
                giroIzquierda.Set(OFF);     // Apagar giro izquierdo antes de girar a la derecha
                giroDerecha.Set(ON);        // Iniciar giro derecho
                timerMotor.Start(10000, 0, finalizarTimerMotor);
                flag_timerMotor = 0;
                estadoMotor = GIRO_DERECHA;
                contadorGiros++;
            }

            // Verificar condición para pasar a centrifugado
            if ((contadorGiros == 4 && flag_lavadoRapido) || (contadorGiros == 6 && flag_lavadoCompleto)) {
                Motor.Set(10);              // Máxima velocidad
                giroIzquierda.Set(OFF);     // Apagar giro izquierdo
                giroDerecha.Set(ON);        // Iniciar centrifugado
                timerMotor.Start(20000, 0, finalizarTimerMotor);
                contadorGiros = 0;
                estadoMotor = CENTRIFUGADO;
            }
            break;

        case CENTRIFUGADO:
            if (flag_timerMotor) {
                Motor.Set(0);               // Detener el motor
                giroDerecha.Set(OFF);       // Asegurarse de apagar la dirección

                if (flag_soloCentrifugado || ++contadorCiclos >= 3) {
                    estadoMotor = FINAL;    // Terminar ciclo
                } else {
                    estadoMotor = INICIO;  // Volver a girar
                }
            }
            break;

        case FINAL:
            contadorGiros = 0;
            contadorCiclos = 0;
            flag_lavadoCompleto = flag_lavadoRapido = flag_soloCentrifugado = 0;
            MotorTerminado = 1;
            estadoMotor = INICIO;
            break;
    }
}

// Finalización del timer (sin apagar el motor si se está cambiando de dirección)
void finalizarTimerMotor(void) {
    flag_timerMotor = 1;
}

// ==========================
//  Máquina de Estados - ESP32
// ==========================

void MdE_ESP32(void){
	char buff[50];
	switch(estadoESP32){
		case 0:
			sprintf(buff,"%s,%s", nombreRed, contraRed);
			UART0_Send((uint8_t*)buff, 0);
			estadoESP32 = 1;
		break;

		case 1:
			if(flag_conexionExitosa) ledVerde.Set(0);
			else ledRojo.Set(1);
		break;

	}
}

