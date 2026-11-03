# Tutorial 10 (C++): Polimorfismo Dinámico (Vtable) vs Polimorfismo Estático

## 🎯 Objetivo de Aprendizaje
Comprender el costo en memoria SRAM y velocidad que introduce el polimorfismo dinámico (`virtual` y Vtables) en microcontroladores, y aprender a utilizar el **Polimorfismo Estático con Plantillas (Templates / CRTP)** para lograr una arquitectura orientada a interfaces completamente genérica con **cero bytes de sobrecarga** y llamadas directas optimizadas (*inlined*).

---

## 🔬 ¿Qué sucede bajo el capó con `virtual`?

Cuando agregas `virtual` a un método:
1. El compilador crea una **Tabla Virtual (Vtable)** en la memoria Flash que contiene las direcciones de memoria de cada función virtual.
2. En **CADA objeto o instancia**, el compilador inserta un puntero oculto llamado `__vptr` que apunta a esa Vtable.
3. Para ejecutar `puntero->encender()`, el procesador debe:
   * Leer el valor de `__vptr` desde la SRAM.
   * Calcular la dirección de salto sumando el offset en la tabla.
   * Ejecutar una instrucción de salto indirecto (`ICALL` en AVR o `BLX` en ARM).
   * **Consecuencia**: El compilador jamás puede hacer *inline* de la función.

### Comparativa de Memoria
Para un objeto que solo contiene un pin (`uint8_t m_pin` de 1 byte):
* **Sin `virtual` (Polimorfismo Estático)**: Ocupa **1 byte** en SRAM.
* **Con `virtual` en AVR (8-bit)**: Ocupa 1 byte de pin + 2 bytes de `__vptr` = **3 bytes** (¡300% del tamaño original!).
* **Con `virtual` en ESP32 / ARM (32-bit)**: Ocupa 1 byte de pin + 3 bytes de padding + 4 bytes de `__vptr` = **8 bytes** (¡800% del tamaño original!).

---

## 🚀 La Solución Embebida: Polimorfismo Estático con Templates

En lugar de derivar de una clase base abstracta con punteros virtuales:
```cpp
template <typename TDispositivo>
void dispararCicloAlerta(TDispositivo& dev) {
    dev.encender(); // Resuelto en tiempo de compilación. Cero vtable. Cero indirect call.
}
```
El compilador genera código de máquina idéntico a una llamada directa normal, permitiendo inlining agresivo y optimizaciones de registro sin pagar por punteros virtuales en la SRAM.

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **PlatformIO**: Abrir la carpeta `03_Herencia_vs_Templates_Vtable`, compilar y subir.
* **Arduino IDE**: Abrir `03_Herencia_vs_Templates_Vtable.ino`.
