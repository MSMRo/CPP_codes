/**
 * ============================================================================
 * TUTORIAL 06 (ANSI C): Máquina de Estados Finita (FSM) No Bloqueante
 * ============================================================================
 * Este código demuestra:
 * 1. Implementación de una Máquina de Estados Finita (FSM) en ANSI C puro.
 * 2. Uso de 'typedef enum' para modelar estados discretos.
 * 3. Ejecución no bloqueante basada en marcas de tiempo (eliminación de delay()).
 * 4. Manejo de transiciones condicionales y eventos externos (ej. botón/comando).
 *
 * Compatible con: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
 * ============================================================================
 */

#include <Arduino.h>
#include <stdint.h>

#ifndef LED_BUILTIN
#define LED_BUILTIN 2
#endif

// 1. Definición de Estados de la FSM mediante enum ANSI C
typedef enum {
    ESTADO_ROJO = 0,
    ESTADO_VERDE,
    ESTADO_AMARILLO,
    ESTADO_EMERGENCIA
} EstadoFSM_t;

// 2. Estructura de contexto de la máquina de estados
typedef struct {
    EstadoFSM_t estadoActual;
    uint32_t tiempoEntradaEstado;
    uint32_t duracionEstadoMs;
    uint16_t conteoCiclos;
} MaquinaEstados_t;

// Instancia estática de la FSM
static MaquinaEstados_t fsmSemaforo;

// Función para cambiar de estado de forma controlada (registra el tiempo de transición)
void FSM_Transicionar(MaquinaEstados_t* pFsm, EstadoFSM_t nuevoEstado, uint32_t duracionMs) {
    pFsm->estadoActual = nuevoEstado;
    pFsm->tiempoEntradaEstado = millis();
    pFsm->duracionEstadoMs = duracionMs;
}

// Inicialización de la FSM
void FSM_Init(MaquinaEstados_t* pFsm) {
    pFsm->conteoCiclos = 0;
    FSM_Transicionar(pFsm, ESTADO_ROJO, 2000); // Comienza en ROJO por 2 segundos
    Serial.println(F("[FSM] Iniciada en estado ESTADO_ROJO (2000 ms)"));
}

// Actualización periódica no bloqueante de la FSM
void FSM_Update(MaquinaEstados_t* pFsm) {
    uint32_t tiempoActual = millis();
    uint32_t tiempoTranscurrido = tiempoActual - pFsm->tiempoEntradaEstado;

    switch (pFsm->estadoActual) {
        case ESTADO_ROJO:
            digitalWrite(LED_BUILTIN, HIGH); // Representamos ROJO encendiendo el LED fijo
            if (tiempoTranscurrido >= pFsm->duracionEstadoMs) {
                Serial.println(F("[FSM] Transicion: ROJO -> VERDE"));
                FSM_Transicionar(pFsm, ESTADO_VERDE, 2500); // Verde por 2.5s
            }
            break;

        case ESTADO_VERDE:
            digitalWrite(LED_BUILTIN, LOW); // Representamos VERDE apagando el LED
            if (tiempoTranscurrido >= pFsm->duracionEstadoMs) {
                Serial.println(F("[FSM] Transicion: VERDE -> AMARILLO"));
                FSM_Transicionar(pFsm, ESTADO_AMARILLO, 1000); // Amarillo por 1s
            }
            break;

        case ESTADO_AMARILLO:
            // Parpadeo rápido en amarillo
            digitalWrite(LED_BUILTIN, ((tiempoTranscurrido / 100) % 2 == 0) ? HIGH : LOW);
            if (tiempoTranscurrido >= pFsm->duracionEstadoMs) {
                pFsm->conteoCiclos++;
                Serial.print(F("[FSM] Ciclo completado #"));
                Serial.println(pFsm->conteoCiclos);
                Serial.println(F("[FSM] Transicion: AMARILLO -> ROJO"));
                FSM_Transicionar(pFsm, ESTADO_ROJO, 2000);
            }
            break;

        case ESTADO_EMERGENCIA:
            // Parpadeo continuo de advertencia
            digitalWrite(LED_BUILTIN, ((tiempoTranscurrido / 250) % 2 == 0) ? HIGH : LOW);
            // El estado de emergencia solo se sale si el usuario envía 'R' por serial
            break;

        default:
            // Estado inválido defensivo: reiniciar a estado seguro
            FSM_Transicionar(pFsm, ESTADO_ROJO, 2000);
            break;
    }
}

void setup() {
    Serial.begin(115200);
    while (!Serial && millis() < 3000);

    pinMode(LED_BUILTIN, OUTPUT);

    Serial.println(F("\n=================================================="));
    Serial.println(F(" ANSI C EMBEBIDO: MAQUINA DE ESTADOS FINITA (FSM) "));
    Serial.println(F("=================================================="));
    Serial.println(F("Comandos Serial:"));
    Serial.println(F("  'E' -> Disparar estado de EMERGENCIA"));
    Serial.println(F("  'R' -> RESTAURAR operacion normal"));

    FSM_Init(&fsmSemaforo);
}

void loop() {
    // 1. Ejecutar la lógica de la FSM de manera no bloqueante
    FSM_Update(&fsmSemaforo);

    // 2. Comprobar eventos externos (puerto serie)
    if (Serial.available() > 0) {
        char cmd = Serial.read();
        if (cmd == 'E' || cmd == 'e') {
            Serial.println(F("\n>>> ALERTA EXTERNA: Activando ESTADO_EMERGENCIA <<<"));
            FSM_Transicionar(&fsmSemaforo, ESTADO_EMERGENCIA, 0);
        } else if (cmd == 'R' || cmd == 'r') {
            Serial.println(F("\n>>> RESTAURANDO a operacion normal <<<"));
            FSM_Transicionar(&fsmSemaforo, ESTADO_ROJO, 2000);
        }
    }
}
