# Tutorial 04 (ANSI C): Structs, Unions y Bit-fields en Sistemas Embebidos

## 🎯 Objetivo de Aprendizaje
Aprender a estructurar paquetes de comunicación binarios sin bytes fantasma utilizando `__attribute__((packed))`, implementar *Type Punning* mediante `union` para transferir datos complejos (como `float` o `uint32_t`) a través de buses serie (UART, SPI, I2C), y optimizar la memoria SRAM utilizando campos de bits (*bit-fields*).

---

## 🔬 Explicación de la Sintaxis y Conceptos Clave

### 1. El Peligro del Padding (Alineación de Memoria)
Los microprocesadores de 32 bits (ESP32, ARM Cortex-M) prefieren leer palabras de 32 bits alineadas a direcciones divisibles por 4.
Si declaras:
```c
struct Telemetria {
    uint8_t id;      // 1 byte
    uint32_t tiempo; // 4 bytes (el compilador inserta 3 bytes de "relleno" o padding antes de este miembro)
    uint8_t crc;     // 1 byte (+ 3 bytes de padding al final)
};
```
* **En memoria de un ESP32 o ARM**: Ocupará **12 bytes** en lugar de los 6 bytes que tú esperabas.
* Si envías este struct por radio o UART a un microcontrolador de 8 bits (como Arduino Uno), la recepción fallará porque los bytes estarán desfasados.

### Solución: `__attribute__((packed))`
Indica al compilador que desactive el padding y coloque cada variable exactamente en el byte contiguo siguiente:
```c
struct __attribute__((packed)) TelemetriaEmpaquetada {
    uint8_t id;     // offset 0
    uint32_t tiempo;// offset 1
    uint8_t crc;    // offset 5
}; // sizeof = exactamente 6 bytes en cualquier microcontrolador del mundo
```

---

### 2. Uniones y Type Punning
Una `union` comparte la misma dirección de memoria física para todos sus miembros:
```c
union ConvertidorFloat {
    float valorDecimal; // 4 bytes
    uint8_t bytes[4];   // Mismos 4 bytes
};
```
Permite leer directamente los 4 bytes individuales de un número flotante para enviarlos por el bus I2C o guardarlos en EEPROM sin necesidad de usar librerías de conversión o casts costosos.

---

### 3. Campos de Bits (Bit-Fields)
Permite especificar el número exacto de bits que ocupa cada variable dentro de un byte o palabra:
```c
struct RegistroEstado {
    uint8_t habilitado    : 1; // 1 bit (0 o 1)
    uint8_t modoOperacion : 2; // 2 bits (0 a 3)
    uint8_t alertaError   : 1; // 1 bit (0 o 1)
    uint8_t velocidad     : 4; // 4 bits (0 a 15)
}; // Total: 8 bits = 1 byte en SRAM
```
El compilador genera automáticamente las instrucciones de desplazamiento (`<<`) y máscaras (`&`, `|`) necesarias para leer o escribir en cada campo, permitiendo un código limpio y legible sin sobrecarga de memoria.

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **PlatformIO**: Abrir la carpeta `01_Structs_Unions_Bitfields`, seleccionar `env:uno` o `env:esp32` y presionar **Upload & Monitor**.
* **Arduino IDE**: Abrir `01_Structs_Unions_Bitfields.ino`.
