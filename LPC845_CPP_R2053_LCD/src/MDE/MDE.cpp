#include "Defines.h"

extern WaterLevelSensor waterSensor;// Sensor de nivel de agua declarado externamente
extern Gpio 			valvCarga;
extern Gpio 			valvSuavizante;
extern Gpio 			valvDesagote;
extern GPIOPWM 			Motor;
extern Gpio 			giroDerecha;
extern Gpio 			giroIzquierda;
extern Gpio 			nivelAgua;
extern Gpio				ledVerde;
extern Gpio 			ledRojo;
extern TIMERSW 			timerValvula;         // Temporizador para controlar tiempo de llenado
extern TIMERSW 			timerMotor;

extern bool 			flag_lavadoRapido;
extern bool 			flag_lavadoCompleto;
extern bool 			flag_soloCentrifugado;
extern bool				flag_tramaRecibida;
extern bool 			flag_conexionExitosa;
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
            if (flag_lavadoCompleto == ON || flag_lavadoRapido == ON) {
                valvCarga.Set(ON); // Activar válvula de llenado
                estadoValvulas = 1; // Pasar al estado de llenado
            }
            break;

        case 1: // Estado de llenado: Sensor detecta nivel lleno
            if (waterSensor.isFull()) { // Si el nivel está lleno
                valvCarga.Set(OFF); // Cerrar válvula de llenado
                estadoValvulas = 2; // Pasar al estado de espera del motor
            }
            break;

        case 2: // Estado de espera del motor
            if (flag_timerMotor == ON) {
                valvDesagote.Set(ON); // Activar válvula de vaciado
                estadoValvulas = 3; // Pasar al estado de vaciado
            }
            break;

        case 3: // Estado de vaciado
            if (waterSensor.isEmpty()) { // Si el nivel está vacío
                valvDesagote.Set(OFF); // Cerrar válvula de vaciado
                if (flag_lavadoCompleto == ON) {
                    valvSuavizante.Set(ON); // Activar válvula de suavizante
                    estadoValvulas = 4; // Pasar al estado de llenado de suavizante
                } else {
                    estadoValvulas = 0; // Reiniciar al estado inicial
                }
            }
            break;

        case 4: // Estado de llenado de suavizante
            if (waterSensor.isFull()) { // Si el nivel está lleno
                valvSuavizante.Set(OFF); // Cerrar válvula de suavizante
                estadoValvulas = 5; // Pasar al estado de espera del motor para vaciado final
            }
            break;

        case 5: // Estado de espera del motor para vaciado final
            if (flag_timerMotor == ON) {
                valvDesagote.Set(ON); // Activar válvula de vaciado
                estadoValvulas = 6; // Pasar al estado de vaciado final
            }
            break;

        case 6: // Estado de vaciado final
            if (waterSensor.isEmpty()) { // Si el nivel está vacío
                valvDesagote.Set(OFF); // Cerrar válvula de vaciado
                estadoValvulas = 0; // Reiniciar al estado inicial
            }
            break;

        default:
            estadoValvulas = 0; // Estado por defecto: reiniciar al inicio
            break;
    }
}

// Función para cerrar todas las válvulas
void CerrarTodasValvulas(void) {
    valvCarga.Set(OFF);
    valvSuavizante.Set(OFF);
    valvDesagote.Set(OFF);
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

