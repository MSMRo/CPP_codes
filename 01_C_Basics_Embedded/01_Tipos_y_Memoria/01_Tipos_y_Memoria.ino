/**
 * ============================================================================
 * TUTORIAL 01: Tipos de Datos y Memoria en C Embebido
 * ============================================================================
 * Este código demuestra:
 * 1. La diferencia de tamaños entre tipos estándar (int, long) y tipos exactos (<stdint.h>).
 * 2. La importancia de elegir el tipo más pequeño posible en microcontroladores.
 * 3. El uso del modificador `volatile` para variables modificadas por hardware o interrupciones.
 * 4. Cómo medir el uso de memoria en tiempo de ejecución.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h> // Estándar ANSI C para tipos con ancho de bits explícito

// Variable global normal (almacenada en la sección .data de la SRAM si está inicializada)
uint8_t contadorNormal = 0;

// Variable global con el calificador `volatile`:
// Le indica al compilador que su valor puede cambiar en cualquier momento fuera del
// flujo normal del programa (por ejemplo, dentro de una ISR de interrupción o periférico).
// El compilador nunca optimizará el acceso a esta variable guardándola en un registro de CPU.
volatile uint16_t ticksHardware = 0;

// Variable constante: En microcontroladores suele ubicarse en memoria Flash / ROM
const uint32_t VELOCIDAD_SERIAL = 115200;

void setup() {
    Serial.begin(VELOCIDAD_SERIAL);
    while (!Serial && millis() < 3000); // Espera en placas nativas USB (Leonardo, RP2040, etc.)

    Serial.println(F("\n=================================================="));
    Serial.println(F("   TUTORIAL C EMBEBIDO: TIPOS EXACTOS Y MEMORIA   "));
    Serial.println(F("=================================================="));

    // 1. Demostración de tamaños en bytes según la arquitectura
    Serial.println(F("\n--- 1. Tamanos de tipos genericos en C (sizeof) ---"));
    Serial.print(F("sizeof(char):      ")); Serial.print(sizeof(char));      Serial.println(F(" byte(s)"));
    Serial.print(F("sizeof(short):     ")); Serial.print(sizeof(short));     Serial.println(F(" byte(s)"));
    Serial.print(F("sizeof(int):       ")); Serial.print(sizeof(int));       Serial.println(F(" byte(s) [En AVR=2B, en ESP32/ARM=4B!]"));
    Serial.print(F("sizeof(long):      ")); Serial.print(sizeof(long));      Serial.println(F(" byte(s)"));
    Serial.print(F("sizeof(long long): ")); Serial.print(sizeof(long long)); Serial.println(F(" byte(s)"));
    Serial.print(F("sizeof(float):     ")); Serial.print(sizeof(float));     Serial.println(F(" byte(s)"));
    Serial.print(F("sizeof(double):    ")); Serial.print(sizeof(double));    Serial.println(F(" byte(s) [En AVR es igual a float (4B)]"));
    Serial.print(F("sizeof(void*):     ")); Serial.print(sizeof(void*));     Serial.println(F(" byte(s) [Punteros: AVR=2B, ESP32/ARM=4B]"));

    // 2. Tipos explícitos de <stdint.h> (RECOMENDADO EN EMBEBIDOS)
    Serial.println(F("\n--- 2. Tipos de ancho fijo (<stdint.h>) ---"));
    Serial.print(F("uint8_t   (0 a 255):                 ")); Serial.print(sizeof(uint8_t));   Serial.println(F(" byte"));
    Serial.print(F("int8_t    (-128 a 127):              ")); Serial.print(sizeof(int8_t));    Serial.println(F(" byte"));
    Serial.print(F("uint16_t  (0 a 65,535):              ")); Serial.print(sizeof(uint16_t));  Serial.println(F(" bytes"));
    Serial.print(F("int16_t   (-32,768 a 32,767):        ")); Serial.print(sizeof(int16_t));   Serial.println(F(" bytes"));
    Serial.print(F("uint32_t  (0 a 4,294,967,295):       ")); Serial.print(sizeof(uint32_t));  Serial.println(F(" bytes"));
    Serial.print(F("uint64_t  (Entero de 64 bits):       ")); Serial.print(sizeof(uint64_t));  Serial.println(F(" bytes"));

    // 3. Demostración de desbordamiento (Overflow) controlado
    Serial.println(F("\n--- 3. Desbordamiento de enteros (Integer Overflow) ---"));
    uint8_t byteMaximo = 255;
    Serial.print(F("Valor inicial uint8_t: ")); Serial.println(byteMaximo);
    byteMaximo = byteMaximo + 1; // 255 + 1 desborda a 0 en enteros sin signo
    Serial.print(F("Despues de sumar 1:    ")); Serial.println(byteMaximo);

    // 4. Modificador volatile en acción
    ticksHardware++;
    Serial.print(F("\nValor de ticksHardware (volatile): "));
    Serial.println(ticksHardware);

    Serial.println(F("\n[FIN SETUP] Entrando a loop()..."));
}

void loop() {
    contadorNormal++;
    // En cada iteración incrementamos y mostramos el valor
    if (contadorNormal % 10 == 0) {
        Serial.print(F("Ciclo activo - contadorNormal: "));
        Serial.println(contadorNormal);
    }
    delay(500);
}
