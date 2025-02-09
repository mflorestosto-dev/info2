#ifndef SENSORAGUA_H_
#define SENSORAGUA_H_

#include "Defines.h"

// Definir valores por defecto para el sensor
#define SENSOR_AGUA_ADC_CANAL 1 // Canal ADC predeterminado (PIO0_20 = ADC_6)
#define SENSOR_AGUA_LLENO 5000  // Valor ADC para nivel lleno
#define SENSOR_AGUA_VACIO 2500  // Valor ADC para nivel vacío

class WaterLevelSensor {
private:
    uint32_t adcChannel;    // Canal ADC del sensor
    uint32_t fullThreshold; // Valor ADC para indicar nivel lleno
    uint32_t emptyThreshold;// Valor ADC para indicar nivel vacío

public:
    WaterLevelSensor(uint32_t channel = SENSOR_AGUA_ADC_CANAL, uint32_t full = SENSOR_AGUA_LLENO, uint32_t empty = SENSOR_AGUA_VACIO);

    void init();                     // Inicializar el ADC y el sensor
    uint32_t readRawValue();         // Leer el valor crudo del ADC
    bool isFull();                   // Verificar si el nivel es lleno
    bool isEmpty();                  // Verificar si el nivel es vacío
    uint32_t getWaterLevel();        // Obtener el nivel de agua escalado a 0-100%
    void disable();
};

#endif /* SENSORAGUA_H_ */


//#ifndef WATER_LEVEL_SENSOR_DRIVER_H_
//#define WATER_LEVEL_SENSOR_DRIVER_H_
//
//class WaterLevelSensor {
//public:
//    WaterLevelSensor(uint8_t adcChannel, float maxLevel, Gpio *powerPin);
//    void init();
//    uint32_t readRawValue();
//    bool isFull();
//    void powerOn();
//    void powerOff();
//
//private:
//    uint8_t adcChannel;
//    float maxLevel;
//    Gpio *powerPin; // Pin GPIO para controlar la alimentación del sensor
//};
//
//#endif /* WATER_LEVEL_SENSOR_DRIVER_H_ */

