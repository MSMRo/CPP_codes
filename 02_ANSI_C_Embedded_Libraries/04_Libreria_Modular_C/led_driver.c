/**
 * ============================================================================
 * IMPLEMENTACIÓN: led_driver.c (Librería en ANSI C)
 * ============================================================================
 * Implementación de la lógica del driver.
 * Demuestra:
 * 1. Funciones 'static' privadas (encapsulamiento en C puro).
 * 2. Validación defensiva de punteros contra NULL.
 * 3. Lógica no bloqueante multi-instancia.
 * ============================================================================
 */

#include "led_driver.h"
#include <Arduino.h>

// --- FUNCIONES AUXILIARES PRIVADAS (ESTÁTICAS) ---
// Al declarar 'static', estas funciones solo existen dentro de este archivo (.c)
// y ninguna otra parte del programa puede verlas ni llamarlas directamente.

static void escribirPinHardware(const LedHandle_t* pLed, bool encender) {
    bool nivelLogico;
    if (pLed->invertido) {
        nivelLogico = encender ? LOW : HIGH;
    } else {
        nivelLogico = encender ? HIGH : LOW;
    }
    digitalWrite(pLed->pin, nivelLogico ? HIGH : LOW);
}

// --- IMPLEMENTACIÓN DE LA API PÚBLICA ---

LedStatus_t LED_Init(LedHandle_t* pLed, uint8_t pin, bool activeLow) {
    if (pLed == NULL) {
        return LED_STATUS_ERROR_NULL_PTR;
    }

    pLed->pin = pin;
    pLed->invertido = activeLow;
    pLed->modo = LED_MODE_OFF;
    pLed->tiempoUltimoCambio = 0;
    pLed->intervaloEncendido = 500;
    pLed->intervaloApagado = 500;
    pLed->estadoFisicoActual = false;
    pLed->inicializado = true;

    pinMode(pLed->pin, OUTPUT);
    escribirPinHardware(pLed, false);

    return LED_STATUS_OK;
}

LedStatus_t LED_TurnOn(LedHandle_t* pLed) {
    if (pLed == NULL) return LED_STATUS_ERROR_NULL_PTR;
    if (!pLed->inicializado) return LED_STATUS_ERROR_NOT_INITIALIZED;

    pLed->modo = LED_MODE_ON;
    pLed->estadoFisicoActual = true;
    escribirPinHardware(pLed, true);

    return LED_STATUS_OK;
}

LedStatus_t LED_TurnOff(LedHandle_t* pLed) {
    if (pLed == NULL) return LED_STATUS_ERROR_NULL_PTR;
    if (!pLed->inicializado) return LED_STATUS_ERROR_NOT_INITIALIZED;

    pLed->modo = LED_MODE_OFF;
    pLed->estadoFisicoActual = false;
    escribirPinHardware(pLed, false);

    return LED_STATUS_OK;
}

LedStatus_t LED_Toggle(LedHandle_t* pLed) {
    if (pLed == NULL) return LED_STATUS_ERROR_NULL_PTR;
    if (!pLed->inicializado) return LED_STATUS_ERROR_NOT_INITIALIZED;

    pLed->estadoFisicoActual = !pLed->estadoFisicoActual;
    escribirPinHardware(pLed, pLed->estadoFisicoActual);

    return LED_STATUS_OK;
}

LedStatus_t LED_SetBlink(LedHandle_t* pLed, uint32_t periodoMs) {
    return LED_SetBlinkAsync(pLed, periodoMs / 2, periodoMs / 2);
}

LedStatus_t LED_SetBlinkAsync(LedHandle_t* pLed, uint32_t onMs, uint32_t offMs) {
    if (pLed == NULL) return LED_STATUS_ERROR_NULL_PTR;
    if (!pLed->inicializado) return LED_STATUS_ERROR_NOT_INITIALIZED;

    pLed->modo = (onMs == offMs) ? LED_MODE_BLINK_STEADY : LED_MODE_BLINK_ASYNC;
    pLed->intervaloEncendido = onMs;
    pLed->intervaloApagado = offMs;
    pLed->tiempoUltimoCambio = millis();
    pLed->estadoFisicoActual = true;
    escribirPinHardware(pLed, true);

    return LED_STATUS_OK;
}

LedStatus_t LED_Update(LedHandle_t* pLed) {
    if (pLed == NULL) return LED_STATUS_ERROR_NULL_PTR;
    if (!pLed->inicializado) return LED_STATUS_ERROR_NOT_INITIALIZED;

    if (pLed->modo == LED_MODE_OFF || pLed->modo == LED_MODE_ON) {
        return LED_STATUS_OK; // No requiere actualización periódica
    }

    uint32_t ahora = millis();
    uint32_t limite = pLed->estadoFisicoActual ? pLed->intervaloEncendido : pLed->intervaloApagado;

    if (ahora - pLed->tiempoUltimoCambio >= limite) {
        pLed->tiempoUltimoCambio = ahora;
        pLed->estadoFisicoActual = !pLed->estadoFisicoActual;
        escribirPinHardware(pLed, pLed->estadoFisicoActual);
    }

    return LED_STATUS_OK;
}
