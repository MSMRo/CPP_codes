/**
 * ============================================================================
 * TUTORIAL 11 (C++11): La Base del C++ Moderno en Sistemas Embebidos
 * ============================================================================
 * Este código demuestra:
 * 1. 'constexpr': Evaluación matemática en compilación (0 ciclos de CPU en el micro).
 * 2. 'enum class' con tipo subyacente explícito (: uint8_t) para ahorrar SRAM y evitar colisiones.
 * 3. 'static_assert': Pruebas unitarias de arquitectura ejecutadas por el compilador.
 * 4. 'nullptr': Tipado seguro frente al ambiguo NULL o 0.
 * 5. 'auto': Inferencia de tipos sin sobrecarga de runtime.
 * 6. Lambdas sin captura: Decaen directamente a punteros de función C normales de costo cero.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// ============================================================================
// 1. CONSTEXPR: Cómputo en tiempo de compilación
// ============================================================================
// Esta función calcula el divisor de Baud Rate de la UART en tiempo de compilación.
// La operación matemática la hace el procesador de tu PC al compilar, no el microcontrolador!
constexpr uint16_t calcularUartBaud(uint32_t f_cpu, uint32_t baud) {
    return static_cast<uint16_t>((f_cpu / (16UL * baud)) - 1);
}

// Constante resuelta en compilación
constexpr uint16_t UBRR_VAL = calcularUartBaud(16000000UL, 115200);

// ============================================================================
// 2. ENUM CLASS: Enumeraciones con alcance y tamaño de memoria explícito
// ============================================================================
// Al especificar ': uint8_t', garantizamos que este enum ocupe solo 1 byte en SRAM
// en lugar de los 2 o 4 bytes de un enum C tradicional.
enum class EstadoSensor : uint8_t {
    DESCONECTADO = 0,
    CALIBRANDO   = 1,
    LISTO        = 2,
    ERROR_CRC    = 3
};

// ============================================================================
// 3. STATIC_ASSERT: Validación estricta en tiempo de compilación
// ============================================================================
// Si el tamaño no es 1 byte, la compilación se detendrá con el mensaje de error
static_assert(sizeof(EstadoSensor) == 1, "Error: EstadoSensor debe ocupar exactamente 1 byte de SRAM");
static_assert(UBRR_VAL >= 0, "Error: UBRR_VAL no puede ser negativo");

// ============================================================================
// 4. LAMBDAS SIN CAPTURA COMO CALLBACKS DE COSTO CERO
// ============================================================================
using AccionLed_t = void (*)(uint8_t);

void invocarAccion(uint8_t pin, AccionLed_t accion) {
    if (accion != nullptr) { // Uso de nullptr (fuertemente tipado)
        accion(pin);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" C++11 EMBEBIDO: CONSTEXPR, ENUM CLASS Y LAMBDAS  "));
    Serial.println(F("=================================================="));

    // 1. Mostrar valor de constexpr
    Serial.println(F("\n--- 1. constexpr (Calculado en compilacion) ---"));
    Serial.print(F("UBRR_VAL precalculado para 16MHz y 115200 baud: "));
    Serial.println(UBRR_VAL);

    // 2. Mostrar enum class
    Serial.println(F("\n--- 2. enum class (: uint8_t) ---"));
    EstadoSensor estado = EstadoSensor::LISTO;
    Serial.print(F("sizeof(EstadoSensor): "));
    Serial.print(sizeof(estado));
    Serial.println(F(" byte(s)"));
    Serial.print(F("Valor numerico subyacente: "));
    Serial.println(static_cast<uint8_t>(estado));

    // 3. Uso de auto
    Serial.println(F("\n--- 3. Inferencia con auto ---"));
    auto tiempoInicio = millis(); // 'auto' deduce uint32_t sin sobrecarga
    Serial.print(F("Tipo deducido para tiempoInicio, valor: "));
    Serial.println(tiempoInicio);

    // 4. Invocación de Lambda sin captura
    // Una lambda []() { ... } sin captura se convierte automáticamente
    // en un puntero a función estándar sin gastar memoria dinámica
    Serial.println(F("\n--- 4. Lambda sin captura como callback C ---"));
    invocarAccion(LED_BUILTIN, [](uint8_t p) {
        digitalWrite(p, HIGH);
        Serial.println(F("[LAMBDA EJECUTADA] Pin encendido via callback!"));
    });
    delay(500);
    invocarAccion(LED_BUILTIN, [](uint8_t p) {
        digitalWrite(p, LOW);
        Serial.println(F("[LAMBDA EJECUTADA] Pin apagado via callback!"));
    });

    Serial.println(F("\n[FIN SETUP] Entrando a loop()..."));
}

void loop() {
    delay(1000);
}
