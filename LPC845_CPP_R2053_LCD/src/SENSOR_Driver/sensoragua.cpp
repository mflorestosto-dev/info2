#include "Defines.h"

// Constructor del sensor
WaterLevelSensor::WaterLevelSensor(uint32_t channel, uint32_t full, uint32_t empty)
    : adcChannel(channel), fullThreshold(full), emptyThreshold(empty) {}

// Inicializar el ADC
void WaterLevelSensor::init() {
    ADC_Inicializar(); // Llamar a la función del driver ADC para inicializarlo
}

// Leer el valor crudo del ADC
uint32_t WaterLevelSensor::readRawValue() {
    ADC_Disparo(adcChannel); // Disparar la conversión en el canal del sensor
    return ADC_ReadChannel(adcChannel); // Obtener el valor medido
}

// Verificar si el nivel es lleno
bool WaterLevelSensor::isFull() {
    uint32_t value = readRawValue();
    return (value >= fullThreshold);
}

// Verificar si el nivel es vacío
bool WaterLevelSensor::isEmpty() {
    uint32_t value = readRawValue();
    return (value <= emptyThreshold);
}

// Obtener el nivel de agua escalado a 0-100%
uint32_t WaterLevelSensor::getWaterLevel() {
    uint32_t value = readRawValue();

    if (value >= fullThreshold) return 100;
    if (value <= emptyThreshold) return 0;

    // Escalar el valor entre los umbrales
    return ((value - emptyThreshold) * 80) / (fullThreshold - emptyThreshold + 20);
}

void WaterLevelSensor::disable() {
    // Apagar módulo ADC
    SYSCON->PDRUNCFG |= (1 << 4); // Apagar el módulo del ADC
    // Opcional: Puedes agregar más acciones, como apagar periféricos asociados
}

