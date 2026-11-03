/**
 * ============================================================================
 * TUTORIAL 12 (C++14 & C++17): Características Avanzadas de Costo Cero
 * ============================================================================
 * Este código demuestra:
 * 1. C++14: Literales binarios (0b...), separadores y constexpr con bucles.
 * 2. C++17 'if constexpr': Eliminación total de ramas muertas en tiempo de compilación.
 * 3. C++17 Structured Bindings (Desempaquetado estructurado): auto [x, y] = fn().
 * 4. C++17 Atributo [[nodiscard]]: Obligación de capturar códigos de error críticos.
 * 5. C++17 'inline variables': Constantes globales sin violar la regla ODR.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>
#include "tipos_cpp17.h"

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// ============================================================================
// 1. C++14: CONSTEXPR EVALUADO EN COMPILACIÓN
// ============================================================================
constexpr uint32_t FRECUENCIA_CPU = 16000000UL;
constexpr uint8_t MASCARA_CANALES = 0b10110001; // Canales 0, 4, 5 y 7

constexpr uint8_t calcularChecksumCompileTime(uint8_t id, uint16_t valor) {
    return static_cast<uint8_t>(id ^ (valor & 0xFF) ^ ((valor >> 8) & 0xFF));
}

constexpr uint8_t CRC_PRECALCULADO = calcularChecksumCompileTime(0x0A, 1024);

// ============================================================================
// 2. C++17: [[nodiscard]] PARA MANEJO DE ERRORES OBLIGATORIO
// ============================================================================
[[nodiscard]] CodigoRetorno iniciarPeriferico(uint8_t pin) {
    if (pin == 0xFF) return CodigoRetorno::ERROR_HARDWARE;
    pinMode(pin, OUTPUT);
    return CodigoRetorno::EXITO;
}

// ============================================================================
// 3. C++17: STRUCTURED BINDINGS (Desempaquetado)
// ============================================================================
DatosClima leerEstacionClima() {
    return { 24.5f, 58.2f, 1013 };
}

// ============================================================================
// 4. C++17: IF CONSTEXPR (Eliminación de código en compilación)
// ============================================================================
template <typename T>
void transmitirDatoSerial(T valor) {
    // Si T es de 1 byte, el compilador SOLO compila esta rama y BORRA
    // físicamente la otra del archivo binario .hex / .bin!
    if constexpr (sizeof(T) == 1) {
        Serial.print(F("[1-BYTE OPTIMIZADO]: "));
        Serial.println(static_cast<uint32_t>(valor));
    } else if constexpr (sizeof(T) == 4) {
        Serial.print(F("[4-BYTES WORD]: "));
        Serial.println(valor);
    } else {
        Serial.print(F("[TIPO GENERICO]: "));
        Serial.println(valor);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" C++14 Y C++17 EMBEBIDO: CONSTEXPR, BINDINGS, ETC "));
    Serial.println(F("=================================================="));

    // 1. C++14: Literales y Checksum precalculado
    Serial.println(F("\n--- 1. C++14: Literales y Constexpr ---"));
    Serial.print(F("FRECUENCIA_CPU:   ")); Serial.print(FRECUENCIA_CPU); Serial.println(F(" Hz"));
    Serial.print(F("MASCARA_CANALES:  0x")); Serial.println(MASCARA_CANALES, HEX);
    Serial.print(F("CRC_PRECALCULADO: 0x")); Serial.println(CRC_PRECALCULADO, HEX);

    // 2. C++17: [[nodiscard]]
    Serial.println(F("\n--- 2. C++17: [[nodiscard]] ---"));
    CodigoRetorno res = iniciarPeriferico(LED_BUILTIN);
    if (res == CodigoRetorno::EXITO) {
        Serial.println(F("[OK] iniciarPeriferico evaluado exitosamente"));
    }

    // 3. C++17: Structured Bindings
    Serial.println(F("\n--- 3. C++17: Structured Bindings (Desempaquetado) ---"));
    auto [temp, hum, pres] = leerEstacionClima();
    Serial.print(F("Temperatura: ")); Serial.print(temp); Serial.println(F(" *C"));
    Serial.print(F("Humedad:     ")); Serial.print(hum);  Serial.println(F(" %"));
    Serial.print(F("Presion:     ")); Serial.print(pres); Serial.println(F(" hPa"));

    // 4. C++17: if constexpr
    Serial.println(F("\n--- 4. C++17: if constexpr (Optimizacion de ramas) ---"));
    uint8_t byteSensor = 0x5A;
    uint32_t palabra32 = 123456789UL;
    transmitirDatoSerial(byteSensor);
    transmitirDatoSerial(palabra32);

    // 5. C++17: Inline Variable
    Serial.println(F("\n--- 5. C++17: Inline Static Variable ---"));
    Serial.print(F("Timeout maximo del sistema: "));
    Serial.print(ConfiguracionSistema::TIMEOUT_MAXIMO_MS);
    Serial.println(F(" ms"));

    Serial.println(F("\n[FIN SETUP]"));
}

void loop() {
    delay(1000);
}
