# Tutorial 05 (ANSI C): Punteros a Funciones, Callbacks y Dispatch Tables

## 🎯 Objetivo de Aprendizaje
Aprender la sintaxis y uso de punteros a funciones en ANSI C para implementar el patrón de diseño **Callback** (esencial para librerías de sensores, pulsadores y drivers de interrupciones) y diseñar **Tablas de Despacho (Dispatch Tables)** para decodificar protocolos y comandos en tiempo constante $O(1)$ sin cadenas ineficientes de `if-else` o `switch-case`.

---

## 🔬 Explicación de la Sintaxis y Conceptos Clave

### 1. Declaración de un Puntero a Función
En C, las funciones también residen en una dirección física de memoria (en la memoria Flash/ROM de código). Un puntero a función almacena la dirección de inicio de dicha rutina.

Sintaxis recomendada con `typedef`:
```c
// Define el tipo 'EventoBotonCallback_t' como un puntero a una función que recibe
// (uint8_t, uint32_t) y no retorna nada (void):
typedef void (*EventoBotonCallback_t)(uint8_t idBoton, uint32_t duracionMs);

// Declarar una variable de ese tipo:
EventoBotonCallback_t miCallback = NULL;

// Asignarle una función real:
miCallback = miManejador;

// Invocarla indirectamente:
miCallback(1, 250);
```

### 2. ¿Por qué usar Callbacks en Firmware?
Permite desacoplar totalmente las capas del sistema:
* El **Driver del Pulsador** solo sabe cómo detectar el rebote físico y medir el tiempo. No debe saber qué hace el LED o la pantalla.
* La **Capa de Aplicación** registra su función en el driver. Cuando ocurre el evento físico, el driver llama al puntero. El driver se vuelve 100% portable y reutilizable en cualquier proyecto futuro.

### 3. Tablas de Despacho (Dispatch Tables) vs `switch-case`
Cuando recibes paquetes por UART con códigos de comando (ej: 0 a 10):
* Un `switch-case` genera múltiples instrucciones condicionales de salto (`CPI`, `BREQ` en ensamblador AVR), cuyo tiempo de ejecución depende de qué tan abajo esté el caso en la lista ($O(N)$).
* Una tabla de punteros a funciones:
  ```c
  const ComandoHandler_t TABLA[] = { cmd0, cmd1, cmd2, cmd3 };
  TABLA[codigo](); // Ejecución instantánea O(1) calculando la dirección mediante desplazamiento
  ```

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **Interactividad**: Abre el Monitor Serie y envía los caracteres `0`, `1` o `2` para disparar los comandos de la Dispatch Table en tiempo real.
