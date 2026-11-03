# Tutorial 06 (ANSI C): Máquina de Estados Finita (FSM) No Bloqueante

## 🎯 Objetivo de Aprendizaje
Aprender a diseñar sistemas embebidos profesionales reactivos y no bloqueantes utilizando **Máquinas de Estados Finitas (FSM)** en ANSI C. Este patrón sustituye por completo las llamadas a `delay()`, permitiendo que el microcontrolador atienda comunicaciones serie, lea pulsadores y ejecute animaciones en paralelo sin pausar la CPU.

---

## 🔬 Arquitectura de la FSM en C

Una máquina de estados finita consta de:
1. **Un conjunto de Estados Discretos**: Modelados mediante `typedef enum`.
2. **Estructura de Contexto**: Almacena el estado actual y variables de soporte (marcas de tiempo, contadores).
3. **Función de Transición**: Controla el cambio de estado y actualiza el cronómetro interno.
4. **Función de Actualización (`FSM_Update`)**: Se invoca en cada pasada de `loop()`. Evalúa condiciones temporales (`millis() - tiempoEntrada >= duracion`) o eventos externos.

```
       ┌───────────┐  2000 ms   ┌───────────┐  2500 ms   ┌──────────────┐
       │   ROJO    ├───────────►│   VERDE   ├───────────►│   AMARILLO   │
       └─────▲─────┘            └───────────┘            └──────┬───────┘
             │                                                  │
             └────────────────── 1000 ms ───────────────────────┘
                    ▲                               ▲
            'R'     │                               │ 'E'
        (Restaurar) │                               │ (Comando Serial)
             ┌──────┴──────────────┐                │
             │  ESTADO_EMERGENCIA  │◄───────────────┘
             └─────────────────────┘
```

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **Interactividad en Tiempo Real**:
  * Envía `E` en el Monitor Serie para interrumpir el ciclo normal y pasar de inmediato al estado de parpadeo de emergencia.
  * Envía `R` para restaurar el ciclo de semáforo normal.
