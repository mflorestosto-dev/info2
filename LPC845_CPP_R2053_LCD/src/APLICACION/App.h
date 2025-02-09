/*
 * App.h
 *
 *  Created on: 20 dic. 2024
 *      Author: matia
 */

#ifndef APLICACION_APP_H_
#define APLICACION_APP_H_

#define NO_KEY 255

void MdeE(void);
void analisisdeTramas(uint8_t dato);
void AnalizarTramaUart0(uint8_t dato);
void AnalizarTramaConexion(uint8_t dato);
void leerYEnviarRFID(void);
uint8_t App(void);
uint8_t LeerBufferRX( void );
void Inicializacion(void);
#endif /* APLICACION_APP_H_ */
