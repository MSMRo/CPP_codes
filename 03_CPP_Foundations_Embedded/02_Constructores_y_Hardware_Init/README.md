# Tutorial 09 (C++): Constructores, Patrón begin() y RAII en Embebidos

## 🎯 Objetivo de Aprendizaje
Comprender el ciclo de arranque de un microcontrolador y evitar el error más común en C++ embebido: configurar periféricos dentro del constructor de objetos globales. Aprender el patrón `begin()` y dominar el paradigma **RAII** (*Resource Acquisition Is Initialization*) para crear secciones críticas y candados de interrupción a prueba de fugas y bloqueos.

---

## 🔬 ¿Por qué NUNCA configurar hardware en un Constructor Global?

En C++, cuando declaras un objeto global:
```cpp
MiSensor sensor(10); // Objeto global en SRAM
```
El código de su constructor se ejecuta durante la fase de inicialización del runtime de C (`__do_global_ctors`), **ANTES** de que comience a ejecutarse la función `main()` del microcontrolador.

En el framework de Arduino:
```cpp
int main(void) {
    init();          // <- Aquí se configuran los timers (millis), el ADC y el PLL
    initVariant();
    setup();         // <- Tu código
    while (1) {
        loop();
    }
}
```
Si llamas a `pinMode()`, `Wire.begin()`, `Serial.print()` o `delay()` dentro del constructor de un objeto global:
* `init()` todavía no ha configurado los registros de hardware del microcontrolador.
* Los timers aún no están corriendo, por lo que `delay()` causará un **bucle infinito** congelando la placa antes de que encienda.
* El bus I2C o SPI fallará silenciosamente.

### La Solución Canónica: El patrón `begin()`
* El **Constructor**: Únicamente inicializa variables miembro en SRAM (pines, factores de calibración, flags).
* El método **`begin()`**: Se invoca dentro de `setup()` para inicializar registros físicos del microcontrolador.

---

## 🔒 RAII (Resource Acquisition Is Initialization) en Embebidos

RAII vincula el ciclo de vida de un recurso de hardware al ámbito (*scope*) de un objeto C++.

Si usas el estilo C tradicional:
```c
noInterrupts(); // Desactiva interrupciones
if (error) {
    return; // ¡ERROR FATAL! Saliste de la función olvidando llamar a interrupts(), congelando el sistema.
}
interrupts();
```
Con RAII:
```cpp
class InterruptLock {
public:
    InterruptLock() { noInterrupts(); }
    ~InterruptLock() { interrupts(); }
};

// Uso:
{
    InterruptLock lock; // Desactiva interrupciones al crearse
    if (error) return;  // El destructor de 'lock' se ejecuta AUTOMÁTICAMENTE, reactivando interrupciones!
} // Al salir de la llave, se reactivan interrupciones
```

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **PlatformIO**: Abrir la carpeta `02_Constructores_y_Hardware_Init`, compilar y subir.
* **Arduino IDE**: Abrir `02_Constructores_y_Hardware_Init.ino`.
