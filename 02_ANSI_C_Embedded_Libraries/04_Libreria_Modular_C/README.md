# Tutorial 07 (ANSI C): Arquitectura de Librería Modular y Drivers en C

## 🎯 Objetivo de Aprendizaje
Aprender la arquitectura profesional de controladores de hardware (drivers) y librerías en **ANSI C estándar** reutilizables en cualquier proyecto de C o C++. Comprender el uso de la guarda `extern "C"`, el encapsulamiento de funciones privadas con `static`, y el manejo de múltiples instancias mediante estructuras de contexto (*Handle Pattern*, el mismo patrón usado por **FreeRTOS** y el **Kernel de Linux**).

---

## 🔬 Anatomía de una Librería Profesional en ANSI C

```
04_Libreria_Modular_C/
├── led_driver.h           <-- Interfaz Pública: Structs, Enums y Declaración de Funciones
├── led_driver.c           <-- Implementación Privada: Lógica y funciones static
├── 04_Libreria_Modular_C.ino <-- Aplicación de Usuario: Consumo del driver
└── platformio.ini         <-- Configuración multiplataforma (AVR / ESP32)
```

### 1. Guardas de Inclusión (`#ifndef`)
Previenen errores de compilación por redefinición cuando múltiples archivos `.c` o `.cpp` incluyen la misma cabecera:
```c
#ifndef LED_DRIVER_H
#define LED_DRIVER_H
// ... declaraciones ...
#endif
```

### 2. El Bloque `extern "C"`
Los compiladores de C++ aplican un proceso llamado *Name Mangling* (decoración de nombres) para permitir sobrecarga de funciones. Si compilas un archivo `.c` junto a un sketch `.ino` o `.cpp`, el enlazador fallará buscando el símbolo decorado a menos que envuelvas las declaraciones en:
```c
#ifdef __cplusplus
extern "C" {
#endif

// Funciones en C

#ifdef __cplusplus
}
#endif
```

### 3. Encapsulamiento con `static`
En C no existen las palabras clave `private` o `public`. Para ocultar una función de ayuda y que solo sea visible dentro de `led_driver.c`, se declara como `static`:
```c
static void escribirPinHardware(const LedHandle_t* pLed, bool encender);
```
Esto evita la contaminación del espacio de nombres global y previene colisiones con funciones de otras librerías.

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **Conexión de Hardware Opcional**: Puedes conectar un LED externo con su resistencia al pin 12 (en Uno) o pin 4 (en ESP32) para ver ambos LEDs parpadear a ritmos completamente distintos de forma asíncrona.
