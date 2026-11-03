/**
 * ============================================================================
 * TUTORIAL 05 (ANSI C): Punteros a Funciones, Callbacks y Dispatch Tables
 * ============================================================================
 * Este código demuestra:
 * 1. Declaración y sintaxis de punteros a funciones con 'typedef'.
 * 2. El patrón Callback para desacoplar el driver de hardware de la aplicación.
 * 3. Tablas de salto o despacho (Dispatch Tables) para procesar comandos en O(1)
 *    sin usar bloques infinitos de 'switch-case' o 'if-else'.
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// 1. Definición del tipo puntero a función usando ANSI C typedef
// Representa una función que recibe un id (uint8_t) y una duración en ms (uint32_t) y retorna void
typedef void (*EventoBotonCallback_t)(uint8_t idBoton, uint32_t duracionMs);

// Puntero global a función (almacena el callback registrado)
EventoBotonCallback_t callbackRegistrado = NULL;

// Función del "Driver" que registra el callback de la aplicación
void registrarEventoBoton(EventoBotonCallback_t cb) {
    callbackRegistrado = cb;
}

// Función del "Driver" que simula la detección física del pulsador
void simularPulsacionHardware(uint8_t idBoton, uint32_t duracionMs) {
    // Si la aplicación registró un callback, lo invocamos a través del puntero
    if (callbackRegistrado != NULL) {
        callbackRegistrado(idBoton, duracionMs); // Invocación indirecta
    }
}

// --- APLICACIÓN: Funciones que actúan como Callbacks ---
void miManejadorBoton(uint8_t idBoton, uint32_t duracionMs) {
    Serial.print(F("[CALLBACK ACTIVADO] Boton #"));
    Serial.print(idBoton);
    Serial.print(F(" pulsado durante "));
    Serial.print(duracionMs);
    Serial.println(F(" ms"));
}

// --- PARTE 2: TABLAS DE DESPACHO (DISPATCH TABLES) ---
// Supongamos que recibimos un comando numérico por puerto serie (0: APAGAR, 1: ENCENDER, 2: CONMUTAR)
typedef void (*ComandoHandler_t)(void);

void comandoApagarLED(void) {
    digitalWrite(LED_BUILTIN, LOW);
    Serial.println(F("-> Comando Ejecutado: LED APAGADO"));
}

void comandoEncenderLED(void) {
    digitalWrite(LED_BUILTIN, HIGH);
    Serial.println(F("-> Comando Ejecutado: LED ENCENDIDO"));
}

void comandoConmutarLED(void) {
    static bool estado = false;
    estado = !estado;
    digitalWrite(LED_BUILTIN, estado ? HIGH : LOW);
    Serial.println(F("-> Comando Ejecutado: LED CONMUTADO"));
}

// Tabla de punteros a funciones en memoria (Dispatch Table)
// Al ser 'const', el compilador puede almacenarla en memoria Flash (ROM) en vez de SRAM
const ComandoHandler_t TABLA_COMANDOS[] = {
    comandoApagarLED,   // Indice 0
    comandoEncenderLED,  // Indice 1
    comandoConmutarLED   // Indice 2
};

const uint8_t NUM_COMANDOS = sizeof(TABLA_COMANDOS) / sizeof(TABLA_COMANDOS[0]);

// Ejecución directa en tiempo O(1)
void ejecutarComando(uint8_t codigo) {
    if (codigo < NUM_COMANDOS) {
        // Ejecución directa de la función indexada
        TABLA_COMANDOS[codigo]();
    } else {
        Serial.print(F("Comando desconocido: "));
        Serial.println(codigo);
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" ANSI C EMBEBIDO: PUNTEROS A FUNCIONES Y CALLBACKS"));
    Serial.println(F("=================================================="));

    // 1. Registro del callback
    Serial.println(F("\n--- 1. Registro e invocacion de Callbacks ---"));
    registrarEventoBoton(miManejadorBoton);

    // Simulación de eventos
    simularPulsacionHardware(1, 150);
    simularPulsacionHardware(2, 2300);

    // 2. Uso de la Dispatch Table
    Serial.println(F("\n--- 2. Ejecucion mediante Dispatch Table O(1) ---"));
    Serial.println(F("Ejecutando comando 1 (ENCENDER):"));
    ejecutarComando(1);
    delay(500);

    Serial.println(F("Ejecutando comando 0 (APAGAR):"));
    ejecutarComando(0);
    delay(500);

    Serial.println(F("Ejecutando comando 2 (CONMUTAR):"));
    ejecutarComando(2);

    Serial.println(F("\n[FIN SETUP] Envia '0', '1' o '2' por Serial para probar..."));
}

void loop() {
    if (Serial.available() > 0) {
        char c = Serial.read();
        if (c >= '0' && c <= '9') {
            uint8_t cmd = c - '0';
            ejecutarComando(cmd);
        }
    }
}
