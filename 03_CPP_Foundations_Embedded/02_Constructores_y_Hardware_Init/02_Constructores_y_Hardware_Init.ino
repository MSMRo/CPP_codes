/**
 * ============================================================================
 * TUTORIAL 09 (C++): Constructores, Patrón begin() y RAII en Embebidos
 * ============================================================================
 * Este código demuestra:
 * 1. Por qué NUNCA se debe inicializar hardware dentro de un constructor global
 *    (El problema del Orden de Inicialización Estática en C++).
 * 2. El patrón canónico de la industria: Constructor para datos + begin() para hardware.
 * 3. RAII (Resource Acquisition Is Initialization) en microcontroladores:
 *    Implementación de un 'InterruptLock' que garantiza la reactivación de
 *    interrupciones de forma 100% segura incluso con retornos tempranos (early return).
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
// 1. PATRÓN RAII (Resource Acquisition Is Initialization)
// ============================================================================
// Esta clase adquiere el bloqueo de interrupciones en su constructor
// y lo libera AUTOMÁTICAMENTE en su destructor al salir del ámbito ({ ... }).
class InterruptLock {
public:
    InterruptLock() {
        noInterrupts(); // Desactiva interrupciones (adquisición del recurso)
    }

    ~InterruptLock() {
        interrupts();   // Reactiva interrupciones (liberación garantizada del recurso)
    }

    // Prohibir copia para evitar comportamientos indefinidos
    InterruptLock(const InterruptLock&) = delete;
    InterruptLock& operator=(const InterruptLock&) = delete;
};

// ============================================================================
// 2. PATRÓN CANÓNICO EMBEBIDO: Constructor Liviano + begin()
// ============================================================================
class SensorTemperaturaSimulado {
private:
    uint8_t m_pin;
    float m_calibracionOffset;
    bool m_inicializado;

public:
    // El constructor SOLO inicializa variables en memoria SRAM.
    // NUNCA debe llamar a pinMode, Serial, Wire o delay() porque los objetos
    // globales se construyen ANTES de que la función main() de Arduino llame a init()!
    SensorTemperaturaSimulado(uint8_t pin, float offset)
        : m_pin(pin), m_calibracionOffset(offset), m_inicializado(false) {
        // Seguro: solo asignación de memoria local
    }

    // El método begin() configura el hardware físico cuando el microcontrolador
    // ya tiene sus relojes, buses y timers listos en setup().
    bool begin() {
        pinMode(m_pin, INPUT);
        m_inicializado = true;
        return true;
    }

    float leerTemperatura() {
        if (!m_inicializado) return -999.0f;

        // Simulamos lectura analógica protegida con RAII
        uint16_t lecturaCruda = 0;
        {
            // Bloque de sección crítica protegido con RAII
            InterruptLock lock; // <- Aquí se desactiva interrupts()
            lecturaCruda = analogRead(m_pin);
            // Al salir de esta llave }, el destructor de 'lock' reactiva las interrupciones
            // automáticamente, ¡incluso si hubiéramos puesto un return aquí mismo!
        }

        // Conversión a grados Celsius
        float voltaje = (lecturaCruda * 5.0f) / 1024.0f;
        return (voltaje * 100.0f) + m_calibracionOffset;
    }
};

// Instancia global (Se construye antes de que corra setup())
SensorTemperaturaSimulado sensor(A0, 1.5f);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" C++ EMBEBIDO: CONSTRUCTORES, BEGIN() Y RAII      "));
    Serial.println(F("=================================================="));

    // Aquí en setup(), los timers y registros ya están inicializados por el core.
    // Es el momento seguro para llamar a begin():
    if (sensor.begin()) {
        Serial.println(F("[OK] Sensor inicializado correctamente mediante begin()"));
    }

    // Demostración de lectura con protección RAII
    float temp = sensor.leerTemperatura();
    Serial.print(F("Temperatura inicial leida (con RAII Lock): "));
    Serial.print(temp);
    Serial.println(F(" *C"));

    Serial.println(F("\n[FIN SETUP] Entrando a loop()..."));
}

void loop() {
    float temp = sensor.leerTemperatura();
    Serial.print(F("Temp: "));
    Serial.print(temp);
    Serial.println(F(" *C"));
    delay(2000);
}
