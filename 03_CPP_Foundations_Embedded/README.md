# Módulo 03: Fundamentos de C++ para Microcontroladores

Este módulo desmitifica el uso de la Programación Orientada a Objetos (POO) en microcontroladores y explica cómo aplicar las características de C++ respetando las limitaciones físicas de memoria y determinismo temporal.

---

## 📚 Contenido del Módulo

1. **[`01_Clases_y_Encapsulamiento/`](file:///e:/PROJECTS/CPP_codes/03_CPP_Foundations_Embedded/01_Clases_y_Encapsulamiento)**:
   * Anatomía de una clase en C++ embebido.
   * Modificadores de acceso: `private`, `public`.
   * Demostración empírica de costo cero: `sizeof(Clase)` == `sizeof(struct)`.
   * Métodos `inline` para optimización directa por el compilador sin salto de subrutina.

2. **[`02_Constructores_y_Hardware_Init/`](file:///e:/PROJECTS/CPP_codes/03_CPP_Foundations_Embedded/02_Constructores_y_Hardware_Init)**:
   * **El error #1 del C++ en Arduino**: ¿Por qué nunca se debe configurar hardware dentro de un constructor global?
   * El problema del orden de inicialización estática (*Static Initialization Order Fiasco*).
   * El patrón canónico de la industria: Constructor para datos + método `begin()` para periféricos.
   * RAII (*Resource Acquisition Is Initialization*): Implementación de un `InterruptLock` para proteger secciones críticas sin riesgo de olvidar reactivar interrupciones.

3. **[`03_Herencia_vs_Templates_Vtable/`](file:///e:/PROJECTS/CPP_codes/03_CPP_Foundations_Embedded/03_Herencia_vs_Templates_Vtable)**:
   * Herencia clásica con funciones virtuales (`virtual`, `override`).
   * Qué es la tabla virtual (`vtable`) y el puntero oculto (`__vptr`): costo en bytes de SRAM y ciclos de reloj.
   * Alternativa embebida de costo cero: Polimorfismo estático mediante Plantillas (Templates / CRTP) que resuelve interfaces en compilación sin gastar ni un solo byte de RAM.

---

## 🎯 Placas Recomendadas
* **Arduino Uno / Nano (ATmega328P)**: Ideal para comprobar el impacto del puntero de vtable en los 2 KB de SRAM.
* **ESP32 / RP2040 / STM32**: Para evaluar el rendimiento de llamadas directas inlined vs llamadas virtuales indirectas.
