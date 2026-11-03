# Tutorial 01: Tipos de Datos y Memoria en C Embebido

## 🎯 Objetivo de Aprendizaje
Aprender por qué la elección estricta del tipo de dato es el factor más determinante para la estabilidad y eficiencia en memoria SRAM en sistemas embebidos, y comprender cómo el modificador `volatile` previene errores silenciosos en interrupciones y periféricos de hardware.

---

## 🔬 Explicación de la Sintaxis y Conceptos Clave

### 1. El Peligro del tipo `int` en Código Portátil
En C tradicional, el estándar sólo define que `sizeof(short) <= sizeof(int) <= sizeof(long)`.
* En un **Arduino Uno / Nano (ATmega328P de 8 bits)**, `int` ocupa **2 bytes** (16 bits, rango: -32,768 a 32,767).
* En un **ESP32, STM32 o Raspberry Pi Pico (32 bits)**, `int` ocupa **4 bytes** (32 bits, rango: -2,147,483,648 a 2,147,483,647).

> [!WARNING]
> Si defines un array de 500 enteros como `int datos[500];`:
> * En Arduino Uno ocupará 1,000 bytes (¡el 50% de toda su SRAM disponible!).
> * En ESP32 ocupará 2,000 bytes.
> Por ello, en sistemas embebidos **nunca debes usar `int` genérico** para variables que requieran un rango predecible.

### 2. El Estándar `<stdint.h>`
Se debe incluir siempre `<stdint.h>` o utilizar las definiciones estándar de ANSI C99:
* `uint8_t`: Entero sin signo de 8 bits (0 a 255). Ideal para pines, estados, bytes de comunicación UART/SPI/I2C.
* `int8_t`: Entero con signo de 8 bits (-128 a 127).
* `uint16_t`: Entero sin signo de 16 bits (0 a 65,535). Ideal para lecturas de ADC (10-bit o 12-bit).
* `int16_t`: Entero con signo de 16 bits.
* `uint32_t`: Entero sin signo de 32 bits (0 a 4,294,967,295). Obligatorio para marcas de tiempo con `millis()` y `micros()`.

### 3. El Modificador `volatile`
La palabra clave `volatile` le advierte al optimizador del compilador:
> *"Esta variable puede cambiar su valor por causas externas a este hilo de ejecución (por ejemplo, una rutina de interrupción ISR o un registro de hardware que cambia físicamente)"*.

Sin `volatile`, el compilador puede optimizar un bucle como:
```c
while (banderaInterrupcion == 0) {
    // El compilador asume que como nadie modifica 'banderaInterrupcion' aquí dentro,
    // puede cargarla en un registro de CPU una sola vez y nunca volver a leer la SRAM,
    // creando un bucle infinito aunque la interrupción física sí la modifique en RAM.
}
```
Con `volatile uint8_t banderaInterrupcion = 0;`, el procesador está forzado a leer la dirección de memoria física en SRAM en cada iteración.

---

## 📋 Configuración y Ejecución

### Placas Compatibles
* **Arduino Uno / Nano / Mega (AVR)**: Velocidad Serial `115200` baudios.
* **ESP32 Dev Module**: Velocidad Serial `115200` baudios.
* **Raspberry Pi Pico / STM32**: Velocidad Serial `115200` baudios.

### En PlatformIO
Abre la carpeta `01_Tipos_y_Memoria` en VS Code. En la barra inferior, selecciona el entorno `env:uno` o `env:esp32` según tu hardware y presiona **Upload & Monitor**.

### En Arduino IDE
Abre `01_Tipos_y_Memoria.ino`, selecciona tu placa y puerto, y abre el Monitor Serie configurado a **115200 baudios**.

---

## 📊 Salida Esperada en Monitor Serial (Ejemplo en Arduino Uno)
```text
==================================================
   TUTORIAL C EMBEBIDO: TIPOS EXACTOS Y MEMORIA   
==================================================

--- 1. Tamanos de tipos genericos en C (sizeof) ---
sizeof(char):      1 byte(s)
sizeof(short):     2 byte(s)
sizeof(int):       2 byte(s) [En AVR=2B, en ESP32/ARM=4B!]
sizeof(long):      4 byte(s)
sizeof(long long): 8 byte(s)
sizeof(float):     4 byte(s)
sizeof(double):    4 byte(s) [En AVR es igual a float (4B)]
sizeof(void*):     2 byte(s) [Punteros: AVR=2B, ESP32/ARM=4B]

--- 2. Tipos de ancho fijo (<stdint.h>) ---
uint8_t   (0 a 255):                 1 byte
int8_t    (-128 a 127):              1 byte
uint16_t  (0 a 65,535):              2 bytes
int16_t   (-32,768 a 32,767):        2 bytes
uint32_t  (0 a 4,294,967,295):       4 bytes
uint64_t  (Entero de 64 bits):       8 bytes
```
