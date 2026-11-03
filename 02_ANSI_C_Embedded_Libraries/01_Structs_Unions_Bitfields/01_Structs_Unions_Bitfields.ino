/**
 * ============================================================================
 * TUTORIAL 04 (ANSI C): Structs, Unions y Bit-fields en Sistemas Embebidos
 * ============================================================================
 * Este código demuestra:
 * 1. Definición de 'struct' y el fenómeno de alineación (padding) de bytes.
 * 2. Atributo __attribute__((packed)) para protocolos de comunicación binarios.
 * 3. 'union' para descomposición directa de datos (Type Punning) sin copiar memoria.
 * 4. 'bit-fields' (campos de bits) para empaquetar múltiples banderas en 1 solo byte.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

// 1. Estructura estándar sin empaquetar (el compilador puede agregar padding según la arquitectura)
struct TelemetriaNormal {
    uint8_t idSensor;    // 1 byte
    uint32_t timestamp;  // 4 bytes (En microcontroladores de 32 bits, puede alinearse a múltiplo de 4)
    uint8_t checksum;    // 1 byte
};

// 2. Estructura empaquetada: garantiza 0 bytes de padding (vital para tramas de radio/UART/LoRa)
struct __attribute__((packed)) TelemetriaEmpaquetada {
    uint8_t idSensor;    // 1 byte
    uint32_t timestamp;  // 4 bytes exactos contiguos
    uint8_t checksum;    // 1 byte exacto contiguo
};

// 3. Unión para desarmar un número decimal float (4 bytes IEEE-754) en bytes individuales
// para enviarlo byte a byte por UART/I2C/SPI sin conversión a texto
union ConvertidorFloat {
    float valorDecimal;
    uint8_t bytes[4];
    uint32_t valorEntero;
};

// 4. Estructura con campos de bits (Bit-fields):
// Permite representar registros de configuración de hardware ocupando bits específicos
struct RegistroEstado {
    uint8_t habilitado    : 1; // Bit 0: 0=Apagado, 1=Encendido (1 bit)
    uint8_t modoOperacion : 2; // Bits 1-2: 4 modos posibles (0 a 3) (2 bits)
    uint8_t alertaError   : 1; // Bit 3: Bandera de falla (1 bit)
    uint8_t velocidad     : 4; // Bits 4-7: Nivel de velocidad (0 a 15) (4 bits)
    // Total sumado = 1 + 2 + 1 + 4 = 8 bits = ¡Exactamente 1 solo byte en SRAM!
};

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" ANSI C EMBEBIDO: STRUCTS, UNIONS Y BIT-FIELDS    "));
    Serial.println(F("=================================================="));

    // --- PARTE 1: Padding y Estructuras Empaquetadas ---
    Serial.println(F("\n--- 1. Alineacion y Padding de Memoria ---"));
    Serial.print(F("sizeof(TelemetriaNormal):      "));
    Serial.print(sizeof(struct TelemetriaNormal));
    Serial.println(F(" bytes (Suele tener padding en 32-bit)"));

    Serial.print(F("sizeof(TelemetriaEmpaquetada):  "));
    Serial.print(sizeof(struct TelemetriaEmpaquetada));
    Serial.println(F(" bytes (Siempre exactamente 1+4+1 = 6 bytes)"));

    // --- PARTE 2: Uniones y Descomposición de Bytes ---
    Serial.println(F("\n--- 2. Uniones (Type Punning) para Envio Serie ---"));
    union ConvertidorFloat conversor;
    conversor.valorDecimal = 23.75f; // Temperatura de ejemplo

    Serial.print(F("Valor original en float: "));
    Serial.println(conversor.valorDecimal, 2);

    Serial.print(F("Representacion en 4 bytes HEX: [ "));
    for (uint8_t i = 0; i < 4; i++) {
        Serial.print(F("0x"));
        if (conversor.bytes[i] < 0x10) Serial.print(F("0"));
        Serial.print(conversor.bytes[i], HEX);
        Serial.print(F(" "));
    }
    Serial.println(F("]"));

    // Reconstrucción instantánea sin costo computacional
    union ConvertidorFloat receptor;
    for (uint8_t i = 0; i < 4; i++) {
        receptor.bytes[i] = conversor.bytes[i];
    }
    Serial.print(F("Float reconstruido en destino: "));
    Serial.println(receptor.valorDecimal, 2);

    // --- PARTE 3: Campos de Bits (Bit-Fields) ---
    Serial.println(F("\n--- 3. Campos de Bits (Bit-fields) ---"));
    Serial.print(F("sizeof(struct RegistroEstado): "));
    Serial.print(sizeof(struct RegistroEstado));
    Serial.println(F(" byte(s)"));

    struct RegistroEstado estado;
    estado.habilitado = 1;
    estado.modoOperacion = 2; // Modo 2
    estado.alertaError = 0;
    estado.velocidad = 12;    // Nivel 12 de 15

    Serial.print(F("Estado: Habilitado=")); Serial.print(estado.habilitado);
    Serial.print(F(", Modo="));             Serial.print(estado.modoOperacion);
    Serial.print(F(", Error="));            Serial.print(estado.alertaError);
    Serial.print(F(", Vel="));              Serial.println(estado.velocidad);

    // Ver el byte crudo en memoria
    uint8_t byteCrudo = *((uint8_t*)&estado);
    Serial.print(F("Valor del byte empaquetado en binario: 0b"));
    for (int8_t b = 7; b >= 0; b--) {
        Serial.print((byteCrudo & (1 << b)) ? '1' : '0');
    }
    Serial.println();

    Serial.println(F("\n[FIN SETUP]"));
}

void loop() {
    delay(1000);
}
