# Tutorial 02: Punteros y Direcciones de Memoria en C Embebido

## 🎯 Objetivo de Aprendizaje
Comprender la naturaleza física de las direcciones de memoria en microcontroladores, dominar la aritmética de punteros para procesamiento eficiente de flujos de datos (UART, SPI, sensores) y aprender por qué pasar parámetros por puntero previene el desbordamiento de pila (*Stack Overflow*) en microcontroladores con escasa SRAM.

---

## 🔬 Explicación de la Sintaxis

### 1. Operador Dirección (`&`) vs Operador Desreferenciación (`*`)
* `&variable`: Devuelve la posición física (en hexadecimal) donde reside el primer byte de la variable en la memoria SRAM.
* `*puntero`: Accede al contenido ubicado en esa dirección para leerlo o sobreescribirlo.

```c
uint8_t sensorLectura = 42;
uint8_t* pSensor = &sensorLectura; // pSensor almacena la dirección

*pSensor = 75; // Modifica sensorLectura indirectamente
```

### 2. Ahorro de Memoria: Paso por Valor vs Paso por Puntero
En un microcontrolador como el ATmega328P de Arduino Uno, solo dispones de **2048 bytes de SRAM**, compartidos entre variables globales, el montón (*Heap*) y la pila de llamadas (*Stack*).
* Si pasas una estructura o array de 64 bytes por valor:
  ```c
  void procesarBuffer(struct Paquete datos); // ¡COPIA 64 bytes completos en el Stack!
  ```
* Si pasas por puntero:
  ```c
  void procesarBuffer(const struct Paquete* pDatos); // Solo ocupa 2 bytes de Stack (la dirección)
  ```

### 3. Aritmética de Punteros
Cuando incrementas un puntero con `pByte++`, el compilador no le suma necesariamente 1 byte, sino `sizeof(*puntero)`.
* Para `uint8_t*`: `p++` avanza 1 byte.
* Para `uint16_t*`: `p++` avanza 2 bytes.
* Para `uint32_t*`: `p++` avanza 4 bytes.

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, RP2040, STM32.
* **Velocidad Serial**: 115200 baudios.
* **PlatformIO**: Abrir la carpeta `02_Punteros_y_Direcciones`, compilar y subir mediante `env:uno` o `env:esp32`.
* **Arduino IDE**: Abrir el archivo `02_Punteros_y_Direcciones.ino`.
