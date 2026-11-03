/**
 * ============================================================================
 * ARCHIVO DE CABECERA: led_driver.h (Librería en ANSI C)
 * ============================================================================
 * Define la interfaz pública del controlador de LED / actuador.
 * Sigue las mejores prácticas de ANSI C:
 * 1. Guardas de inclusión (#ifndef / #define) para evitar definiciones duplicadas.
 * 2. Bloque 'extern "C"' para que pueda ser llamado tanto desde C como desde C++.
 * 3. Códigos de retorno fuertemente tipados con enum.
 * 4. Paso de puntero a estructura de contexto para soportar múltiples instancias.
 * ============================================================================
 */

#ifndef LED_DRIVER_H
#define LED_DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

// Códigos de error de la librería
typedef enum {
    LED_STATUS_OK = 0,
    LED_STATUS_ERROR_NULL_PTR,
    LED_STATUS_ERROR_INVALID_PIN,
    LED_STATUS_ERROR_NOT_INITIALIZED
} LedStatus_t;

// Modos de operación del LED
typedef enum {
    LED_MODE_OFF = 0,
    LED_MODE_ON,
    LED_MODE_BLINK_STEADY,
    LED_MODE_BLINK_ASYNC
} LedMode_t;

// Estructura de contexto del LED (almacena el estado individual de cada instancia)
typedef struct {
    uint8_t pin;                  // Pin GPIO asignado
    LedMode_t modo;               // Modo de operación actual
    bool invertido;               // True si se activa en nivel bajo (Active-Low)
    bool inicializado;            // Bandera de seguridad
    uint32_t tiempoUltimoCambio;  // Marca de tiempo para parpadeo no bloqueante
    uint32_t intervaloEncendido;  // Tiempo en ms en estado HIGH
    uint32_t intervaloApagado;    // Tiempo en ms en estado LOW
    bool estadoFisicoActual;      // Estado actual del pin
} LedHandle_t;

// --- API PÚBLICA DEL DRIVER ---

/**
 * Inicializa el hardware y configura los valores por defecto del LED.
 */
LedStatus_t LED_Init(LedHandle_t* pLed, uint8_t pin, bool activeLow);

/**
 * Enciende el LED de forma permanente.
 */
LedStatus_t LED_TurnOn(LedHandle_t* pLed);

/**
 * Apaga el LED de forma permanente.
 */
LedStatus_t LED_TurnOff(LedHandle_t* pLed);

/**
 * Conmuta (invierte) el estado actual del LED.
 */
LedStatus_t LED_Toggle(LedHandle_t* pLed);

/**
 * Configura un parpadeo simétrico no bloqueante.
 */
LedStatus_t LED_SetBlink(LedHandle_t* pLed, uint32_t periodoMs);

/**
 * Configura un parpadeo asimétrico (tiempos distintos de encendido y apagado).
 */
LedStatus_t LED_SetBlinkAsync(LedHandle_t* pLed, uint32_t onMs, uint32_t offMs);

/**
 * Actualiza la lógica del LED. Debe llamarse periódicamente en loop().
 */
LedStatus_t LED_Update(LedHandle_t* pLed);

#ifdef __cplusplus
}
#endif

#endif // LED_DRIVER_H
