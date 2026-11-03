/**
 * ============================================================================
 * TUTORIAL 07 (ANSI C): Uso de una Librería Modular en C desde Arduino
 * ============================================================================
 * Este código demuestra:
 * 1. Cómo incluir y consumir una librería escrita en C ANSI puro (led_driver.h).
 * 2. Manejo de múltiples instancias independientes sin clases ni sobrecarga de C++.
 * 3. Ejecución no bloqueante controlando dos LEDs a diferentes frecuencias.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include "led_driver.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Pin auxiliar para segundo LED (Pin 12 en Uno, GPIO 4 en ESP32)
#if defined(ESP32)
#define PIN_LED_AUXILIAR 4
#else
#define PIN_LED_AUXILIAR 12
#endif

// Instancias independientes del contexto del driver
static LedHandle_t ledPrincipal;
static LedHandle_t ledAuxiliar;

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" ANSI C EMBEBIDO: LIBRERIA MODULAR Y DRIVERS      "));
    Serial.println(F("=================================================="));

    // 1. Inicialización del primer LED (LED integrado)
    LedStatus_t res1 = LED_Init(&ledPrincipal, LED_BUILTIN, false);
    if (res1 == LED_STATUS_OK) {
        Serial.println(F("[OK] LED Principal inicializado en pin integrado."));
        // Parpadeo asimétrico: 100 ms encendido (destello rápido), 900 ms apagado
        LED_SetBlinkAsync(&ledPrincipal, 100, 900);
    }

    // 2. Inicialización del segundo LED (Pin auxiliar)
    LedStatus_t res2 = LED_Init(&ledAuxiliar, PIN_LED_AUXILIAR, false);
    if (res2 == LED_STATUS_OK) {
        Serial.println(F("[OK] LED Auxiliar inicializado en pin auxiliar."));
        // Parpadeo simétrico: 500 ms encendido, 500 ms apagado
        LED_SetBlink(&ledAuxiliar, 1000);
    }

    Serial.println(F("\nAmbos LEDs corriendo de forma no bloqueante..."));
}

void loop() {
    // Actualización no bloqueante periódica de cada instancia
    LED_Update(&ledPrincipal);
    LED_Update(&ledAuxiliar);
}
