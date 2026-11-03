/**
 * ============================================================================
 * TUTORIAL 08 (C++): Clases, Encapsulamiento y Abstracción de Costo Cero
 * ============================================================================
 * Este código demuestra:
 * 1. Definición de una clase C++ para modelar un actuador/sensor.
 * 2. La única diferencia entre 'struct' y 'class' en C++ (visibilidad por defecto).
 * 3. Demostración práctica de costo cero: una clase sin métodos virtuales tiene
 *    exactamente el mismo tamaño en SRAM que una estructura en C.
 * 4. Métodos 'inline' y métodos constantes 'const'.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Estructura C tradicional equivalente
struct LedDatosC {
    uint8_t pin;
    bool estado;
};

// Clase C++ con encapsulamiento
class ControladorLed {
private:
    // Variables miembros privadas (protegidas contra modificaciones externas erróneas)
    uint8_t m_pin;
    bool m_estado;

public:
    // Constructor de inicialización de datos
    ControladorLed(uint8_t pin)
        : m_pin(pin), m_estado(false) {
        // NOTA: La configuración de hardware se delega a begin()
    }

    // Configuración de hardware (llamado explícitamente en setup())
    void begin() {
        pinMode(m_pin, OUTPUT);
        digitalWrite(m_pin, LOW);
    }

    // Métodos inline: al definirse dentro de la clase, el compilador los sustituye
    // directamente en el punto de llamada eliminando el overhead de salto de función
    inline void encender() {
        m_estado = true;
        digitalWrite(m_pin, HIGH);
    }

    inline void apagar() {
        m_estado = false;
        digitalWrite(m_pin, LOW);
    }

    inline void conmutar() {
        m_estado = !m_estado;
        digitalWrite(m_pin, m_estado ? HIGH : LOW);
    }

    // Método constante: garantiza al compilador que no modifica ninguna variable de la clase
    inline bool estaEncendido() const {
        return m_estado;
    }

    inline uint8_t getPin() const {
        return m_pin;
    }
};

// Instancia global del objeto
ControladorLed ledTest(LED_BUILTIN);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" C++ EMBEBIDO: CLASES Y ABSTRACCION DE COSTO CERO "));
    Serial.println(F("=================================================="));

    // 1. Demostración de tamaño en SRAM
    Serial.println(F("\n--- 1. Comparacion de Memoria (SRAM) ---"));
    Serial.print(F("sizeof(struct LedDatosC):     "));
    Serial.print(sizeof(struct LedDatosC));
    Serial.println(F(" bytes"));

    Serial.print(F("sizeof(class ControladorLed): "));
    Serial.print(sizeof(ControladorLed));
    Serial.println(F(" bytes"));

    Serial.println(F("-> CONCLUSION: Las clases en C++ sin 'virtual' NO anaden"));
    Serial.println(F("   absolutamente ningun byte adicional de memoria en el micro!"));

    // 2. Inicialización del hardware mediante begin()
    ledTest.begin();
    Serial.print(F("\nControlador inicializado para el Pin "));
    Serial.println(ledTest.getPin());

    // 3. Prueba de métodos
    Serial.println(F("\nEncendiendo LED..."));
    ledTest.encender();
    Serial.print(F("Estado: ")); Serial.println(ledTest.estaEncendido() ? F("ENCENDIDO") : F("APAGADO"));
    delay(1000);

    Serial.println(F("Apagando LED..."));
    ledTest.apagar();
    Serial.print(F("Estado: ")); Serial.println(ledTest.estaEncendido() ? F("ENCENDIDO") : F("APAGADO"));

    Serial.println(F("\n[FIN SETUP] Entrando a loop()..."));
}

void loop() {
    ledTest.conmutar();
    delay(500);
}
