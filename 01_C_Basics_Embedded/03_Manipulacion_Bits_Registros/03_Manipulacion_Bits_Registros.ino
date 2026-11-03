/**
 * ============================================================================
 * TUTORIAL 03: Manipulación de Bits y Acceso a Registros de Hardware
 * ============================================================================
 * Este código demuestra:
 * 1. Operaciones binarias fundamentales: AND (&), OR (|), XOR (^), NOT (~), SHIFT (<<, >>).
 * 2. Los 4 modismos clásicos de bits en sistemas embebidos:
 *    - Poner a 1 (SET):     REG |= (1 << BIT)
 *    - Poner a 0 (CLEAR):   REG &= ~(1 << BIT)
 *    - Conmutar (TOGGLE):   REG ^= (1 << BIT)
 *    - Consultar (TEST):    (REG & (1 << BIT)) != 0
 * 3. Manipulación directa de registros de puertos GPIO (DDRx, PORTx, PINx en AVR)
 *    comparando el rendimiento con digitalWrite() de la capa Arduino.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040 (con soporte portátil).
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

// En algunas placas (como la definicion generica de ESP32), LED_BUILTIN no esta predefinido
#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// Macros estándar ANSI C para manipulación de bits
#define BIT_SET(registro, bit)    ((registro) |= (1U << (bit)))
#define BIT_CLEAR(registro, bit)  ((registro) &= ~(1U << (bit)))
#define BIT_TOGGLE(registro, bit) ((registro) ^= (1U << (bit)))
#define BIT_CHECK(registro, bit)  (((registro) & (1U << (bit))) != 0)

void imprimirBinario8(uint8_t valor) {
    for (int8_t i = 7; i >= 0; i--) {
        Serial.print((valor & (1U << i)) ? '1' : '0');
        if (i == 4) Serial.print(' '); // Espaciador visual para separar nibbles
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    Serial.println(F("\n=================================================="));
    Serial.println(F("  TUTORIAL C EMBEBIDO: MANIPULACION DE BITS/REG   "));
    Serial.println(F("=================================================="));

    uint8_t registroSimulado = 0b00000000;

    // 1. Poner un bit a 1 (SET)
    Serial.println(F("\n--- 1. Operacion SET (Poner a 1) ---"));
    Serial.print(F("Registro Inicial:           0b")); imprimirBinario8(registroSimulado); Serial.println();
    BIT_SET(registroSimulado, 5); // Enciende el bit 5 (0b00100000)
    Serial.print(F("Tras BIT_SET(reg, 5):       0b")); imprimirBinario8(registroSimulado); Serial.println();
    BIT_SET(registroSimulado, 2); // Enciende el bit 2 (0b00100100)
    Serial.print(F("Tras BIT_SET(reg, 2):       0b")); imprimirBinario8(registroSimulado); Serial.println();

    // 2. Comprobar un bit (TEST)
    Serial.println(F("\n--- 2. Operacion TEST (Comprobar estado) ---"));
    Serial.print(F("Esta activo el bit 5? "));
    Serial.println(BIT_CHECK(registroSimulado, 5) ? F("SI (1)") : F("NO (0)"));
    Serial.print(F("Esta activo el bit 0? "));
    Serial.println(BIT_CHECK(registroSimulado, 0) ? F("SI (1)") : F("NO (0)"));

    // 3. Conmutar un bit (TOGGLE)
    Serial.println(F("\n--- 3. Operacion TOGGLE (Invertir estado) ---"));
    BIT_TOGGLE(registroSimulado, 5); // Estaba en 1, pasa a 0
    Serial.print(F("Tras BIT_TOGGLE(reg, 5):    0b")); imprimirBinario8(registroSimulado); Serial.println();
    BIT_TOGGLE(registroSimulado, 5); // Estaba en 0, pasa a 1
    Serial.print(F("Tras segundo TOGGLE(reg, 5):0b")); imprimirBinario8(registroSimulado); Serial.println();

    // 4. Poner a 0 un bit (CLEAR)
    Serial.println(F("\n--- 4. Operacion CLEAR (Poner a 0) ---"));
    BIT_CLEAR(registroSimulado, 2);
    Serial.print(F("Tras BIT_CLEAR(reg, 2):     0b")); imprimirBinario8(registroSimulado); Serial.println();

    // 5. Configuración de Hardware Directo (Ejemplo LED Integrado)
    Serial.println(F("\n--- 5. Hardware Directo: Pin 13 / LED ---"));
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    // En Arduino Uno/Nano: El pin 13 es PB5 (Puerto B, bit 5).
    // Registro de direccion de datos: DDRB (1=Salida, 0=Entrada)
    DDRB |= (1 << DDB5);  // Equivalente a pinMode(13, OUTPUT), pero en 1 ciclo de reloj (62.5 ns)
    Serial.println(F("Configurado DDRB para PB5 (Pin 13) como SALIDA directa"));
#else
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.println(F("Placa no-AVR: Usando pinMode portatil para LED_BUILTIN"));
#endif

    Serial.println(F("\n[FIN SETUP] Parpadeando LED con registros directos..."));
}

void loop() {
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
    // Escribir 1 en el registro PINB conmuta el estado de PORTB en hardware AVR! (Técnica ultra-rápida)
    PINB = (1 << PINB5);
#else
    static bool estado = false;
    estado = !estado;
    digitalWrite(LED_BUILTIN, estado ? HIGH : LOW);
#endif

    delay(500);
}
