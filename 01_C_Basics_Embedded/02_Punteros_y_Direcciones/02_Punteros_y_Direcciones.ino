/**
 * ============================================================================
 * TUTORIAL 02: Punteros, Direcciones de Memoria y Buffers en C Embebido
 * ============================================================================
 * Este código demuestra:
 * 1. Qué es una dirección de memoria física en SRAM y cómo obtenerla con `&`.
 * 2. Operador de desreferenciación `*` para leer o modificar datos.
 * 3. Aritmética de punteros para recorrer buffers de sensores o UART.
 * 4. Paso por puntero a funciones para no saturar el Stack de la memoria SRAM.
 * 5. Punteros constantes vs punteros a datos constantes (`const uint8_t*`).
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

// Función que recibe por valor (COPIA el dato completo en el stack)
void modificarPorValor(uint8_t dato) {
    dato = 99; // Solo modifica la copia local en el stack
}

// Función que recibe por puntero (SOLO pasa la dirección de memoria, 2 o 4 bytes)
void modificarPorPuntero(uint8_t* pDato) {
    if (pDato != NULL) {
        *pDato = 99; // Modifica el valor original en su celda física de memoria
    }
}

// Función que procesa un buffer sin copiarlo, usando puntero a datos constantes
// (garantiza que la función es de solo lectura y no corromperá el buffer)
uint16_t calcularSumaBuffer(const uint8_t* pBuffer, size_t longitud) {
    uint16_t suma = 0;
    for (size_t i = 0; i < longitud; i++) {
        // Acceso usando aritmética de punteros: *(pBuffer + i) es idéntico a pBuffer[i]
        suma += *(pBuffer + i);
    }
    return suma;
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F("   TUTORIAL C EMBEBIDO: PUNTEROS Y DIRECCIONES    "));
    Serial.println(F("=================================================="));

    // 1. Declaración de variable y su puntero
    uint8_t sensorLectura = 42;
    uint8_t* pSensor = &sensorLectura; // pSensor almacena la DIRECCIÓN física en SRAM

    Serial.println(F("\n--- 1. Variable y su direccion de memoria ---"));
    Serial.print(F("Valor de sensorLectura:          ")); Serial.println(sensorLectura);
    Serial.print(F("Direccion (&sensorLectura) en HEX: 0x")); 
    Serial.println((uintptr_t)&sensorLectura, HEX);
    Serial.print(F("Valor almacenado en pSensor:     0x")); 
    Serial.println((uintptr_t)pSensor, HEX);
    Serial.print(F("Desreferenciacion (*pSensor):    ")); 
    Serial.println(*pSensor);

    // 2. Modificación a través del puntero
    Serial.println(F("\n--- 2. Modificacion mediante desreferenciacion ---"));
    *pSensor = 75; // Modifica directamente la posición de memoria apuntada
    Serial.print(F("Nuevo valor de sensorLectura tras *pSensor = 75: "));
    Serial.println(sensorLectura);

    // 3. Paso por valor vs Paso por puntero
    Serial.println(F("\n--- 3. Paso por valor vs Paso por puntero ---"));
    modificarPorValor(sensorLectura);
    Serial.print(F("Despues de modificarPorValor:   ")); Serial.println(sensorLectura); // Sigue siendo 75
    modificarPorPuntero(&sensorLectura);
    Serial.print(F("Despues de modificarPorPuntero: ")); Serial.println(sensorLectura); // Ahora es 99

    // 4. Aritmética de punteros sobre un buffer de telemetría
    Serial.println(F("\n--- 4. Aritmetica de punteros en un buffer ---"));
    uint8_t paqueteTelemetria[5] = {10, 20, 30, 40, 50};
    uint8_t* pByte = paqueteTelemetria; // El nombre del array decae a un puntero a su primer elemento

    for (uint8_t i = 0; i < 5; i++) {
        Serial.print(F("Elemento [")); Serial.print(i); Serial.print(F("] en 0x"));
        Serial.print((uintptr_t)pByte, HEX);
        Serial.print(F(" = ")); Serial.println(*pByte);
        pByte++; // Avanza el puntero a la siguiente posición contigua de memoria
    }

    uint16_t total = calcularSumaBuffer(paqueteTelemetria, 5);
    Serial.print(F("Suma total del buffer calculada por puntero: "));
    Serial.println(total);

    Serial.println(F("\n[FIN SETUP] Entrando a loop()..."));
}

void loop() {
    delay(1000);
}
