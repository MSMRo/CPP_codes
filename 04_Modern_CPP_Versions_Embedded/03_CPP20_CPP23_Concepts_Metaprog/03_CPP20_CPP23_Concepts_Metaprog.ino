/**
 * ============================================================================
 * TUTORIAL 13 (C++20 & C++23): Conceptos (Concepts) y Metaprogramación
 * ============================================================================
 * Este código demuestra:
 * 1. C++20 Concepts (Conceptos): Restricciones de tipos claras y legibles en compilación
 *    que reemplazan por completo las crípticas macros SFINAE / std::enable_if.
 * 2. C++20 'consteval': Funciones que DEBEN evaluarse exclusivamente en tiempo de compilación.
 * 3. C++20 Three-way comparison (<=> / Spaceship operator) para estructuras de telemetría.
 * 4. C++23 Preview: std::expected y la filosofía de 'cero excepciones' en microcontroladores.
 *
 * NOTA DE ARQUITECTURA:
 * Las plantillas y conceptos residen en 'cpp20_features.h' para permitir que el compilador
 * real de GCC los procese con soporte completo sin interferencia del preprocesador de Arduino.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>
#include "cpp20_features.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

constexpr uint32_t FREQ_TIMER = calcularFrecuenciaTimer(80000000UL, 80);

LedPinC20 miLedHardware(LED_BUILTIN);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    miLedHardware.begin();

    Serial.println(F("\n=================================================="));
    Serial.println(F(" C++20 Y C++23: CONCEPTS Y METAPROGRAMACION       "));
    Serial.println(F("=================================================="));

    // Mostrar estándar C++ activo
    Serial.print(F("Estandar C++ reportado (__cplusplus): "));
    Serial.println(__cplusplus);

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L
    Serial.println(F("-> [ACTIVO]: C++20 CONCEPTS soportado nativamente!"));
#else
    Serial.println(F("-> [AVISO]: Toolchain en modo C++11/C++17 (Concepts emulados via templates)."));
#endif

    // 1. Mostrar valor de consteval / constexpr
    Serial.print(F("Frecuencia calculada con consteval/constexpr: "));
    Serial.print(FREQ_TIMER);
    Serial.println(F(" Hz"));

    // 2. Parpadeo con la función protegida por Concepto
    Serial.println(F("Ejecutando parpadearRapido() con restriccion de tipo..."));
    parpadearRapido(miLedHardware);

    // 3. Comparación de estructuras
    PaqueteMuestreo p1{ 101, 512 };
    PaqueteMuestreo p2{ 101, 512 };
    if (p1 == p2) {
        Serial.println(F("[OK] Comparacion de PaqueteMuestreo completada."));
    }

    Serial.println(F("\n[FIN SETUP]"));
}

void loop() {
    delay(1000);
}
