/**
 * ============================================================================
 * TUTORIAL 10 (C++): Polimorfismo Dinámico (Vtable) vs Polimorfismo Estático
 * ============================================================================
 * Este código demuestra:
 * 1. Herencia clásica y funciones virtuales ('virtual'): cómo funciona la Vtable.
 * 2. El costo oculto en memoria SRAM: el puntero de tabla virtual (__vptr).
 * 3. La sobrecarga de ejecución de llamadas indirectas (ICALL) vs directas.
 * 4. Polimorfismo Estático con Plantillas (Templates / CRTP):
 *    ¡Misma abstracción elegante pero con 0 bytes de sobrecarga y 100% inlining!
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
// ENFOQUE 1: POLIMORFISMO DINÁMICO TRADICIONAL (Con funciones virtuales)
// ============================================================================
class DispositivoVirtualBase {
public:
    // Al declarar al menos una función 'virtual', el compilador inserta
    // un puntero oculto (__vptr) dentro de CADA instancia de la clase!
    virtual void encender() = 0;
    virtual void apagar() = 0;
    virtual ~DispositivoVirtualBase() {} // Destructor virtual
};

class LedVirtual : public DispositivoVirtualBase {
private:
    uint8_t m_pin;
public:
    LedVirtual(uint8_t pin) : m_pin(pin) {}
    void begin() { pinMode(m_pin, OUTPUT); }

    void encender() override {
        digitalWrite(m_pin, HIGH);
    }
    void apagar() override {
        digitalWrite(m_pin, LOW);
    }
};

// ============================================================================
// ENFOQUE 2: POLIMORFISMO ESTÁTICO (Plantillas / Abstracción de Costo Cero)
// ============================================================================
// Esta clase no tiene ninguna función virtual. No tiene tabla virtual ni __vptr.
class LedEstatico {
private:
    uint8_t m_pin;
public:
    LedEstatico(uint8_t pin) : m_pin(pin) {}
    void begin() { pinMode(m_pin, OUTPUT); }

    inline void encender() {
        digitalWrite(m_pin, HIGH);
    }
    inline void apagar() {
        digitalWrite(m_pin, LOW);
    }
};

// Función genérica de alto nivel que opera sobre CUALQUIER dispositivo
// sin requerir punteros de tabla virtual
template <typename TDispositivo>
void dispararCicloAlerta(TDispositivo& dev) {
    dev.encender();
    delay(200);
    dev.apagar();
    delay(200);
}

// Instancias para pruebas
LedVirtual ledConVtable(LED_BUILTIN);
LedEstatico ledSinVtable(LED_BUILTIN);

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    ledConVtable.begin();
    ledSinVtable.begin();

    Serial.println(F("\n=================================================="));
    Serial.println(F(" C++ EMBEBIDO: VTABLE vs POLIMORFISMO ESTATICO    "));
    Serial.println(F("=================================================="));

    // 1. Comparación del tamaño físico en memoria SRAM
    Serial.println(F("\n--- 1. Impacto en memoria SRAM de la Vtable ---"));
    Serial.print(F("sizeof(LedEstatico)    [SIN virtual]: "));
    Serial.print(sizeof(LedEstatico));
    Serial.println(F(" byte(s) (Solo almacena m_pin)"));

    Serial.print(F("sizeof(LedVirtual)     [CON virtual]: "));
    Serial.print(sizeof(LedVirtual));
    Serial.print(F(" bytes (m_pin + __vptr de "));
    Serial.print(sizeof(void*));
    Serial.println(F(" bytes!)"));

    Serial.println(F("\n-> EXPLICACION ARQUITECTONICA:"));
    Serial.println(F("   Cada objeto con métodos virtuales paga el costo de un puntero"));
    Serial.println(F("   adicional a la tabla de saltos en Flash. En arrays de sensores"));
    Serial.println(F("   o periféricos, esto triplica el uso de SRAM en micros pequeños."));

    // 2. Ejecución dinámica (indirecta vía puntero base)
    Serial.println(F("\n--- 2. Ejecucion Polimorfismo Dinamico (vtable) ---"));
    DispositivoVirtualBase* pDev = &ledConVtable;
    pDev->encender(); // Salto indirecto: lee la vtable y salta a la dirección
    delay(300);
    pDev->apagar();

    // 3. Ejecución estática con plantillas (inlined directo en compilación)
    Serial.println(F("\n--- 3. Ejecucion Polimorfismo Estatico (Templates) ---"));
    dispararCicloAlerta(ledSinVtable);

    Serial.println(F("\n[FIN SETUP] Ambos métodos probados exitosamente."));
}

void loop() {
    delay(1000);
}
