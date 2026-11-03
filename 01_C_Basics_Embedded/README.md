# Módulo 01: Fundamentos de C en Sistemas Embebidos

Este módulo sienta las bases del lenguaje C clásico y su relación directa con el hardware y la arquitectura física de los microcontroladores.

---

## 📚 Contenido del Módulo

1. **[`01_Tipos_y_Memoria/`](/01_C_Basics_Embedded/01_Tipos_y_Memoria)**:
   * Por qué el tipo `int` es peligroso en código portátil para microcontroladores.
   * Uso del estándar `<stdint.h>` (`uint8_t`, `int16_t`, `uint32_t`, `uint64_t`).
   * Distribución de la memoria física: Flash (código/constantes), SRAM (.data, .bss, Stack, Heap) y EEPROM.
   * Modificadores clave: `const`, `volatile` (y por qué es imprescindible en interrupciones).

2. **[`02_Punteros_y_Direcciones/`](/CPP_codes/01_C_Basics_Embedded/02_Punteros_y_Direcciones)**:
   * ¿Qué es una dirección de memoria física en un microcontrolador?
   * Operadores `&` (dirección) y `*` (desreferenciación).
   * Punteros y buffers: paso de argumentos por referencia para evitar clonar arrays en la escasa SRAM.
   * Punteros a memoria mapeada por hardware (`reinterpret_cast` / punteros a registros).

3. **[`03_Manipulacion_Bits_Registros/`](/01_C_Basics_Embedded/03_Manipulacion_Bits_Registros)**:
   * Operaciones a nivel de bit: AND (`&`), OR (`|`), XOR (`^`), NOT (`~`), desplazamientos (`<<`, `>>`).
   * Cómo crear máscaras de bits (`(1 << PIN)`).
   * Control directo de GPIO sin la sobrecarga de `digitalWrite()` y `pinMode()` accediendo directamente a registros como `DDRB`, `PORTB`, `PINB`.
   * Rendimiento: comparación de ciclos de reloj entre `digitalWrite` (decenas de ciclos) vs instrucción directa `SBI`/`CBI` (1 ciclo de reloj = 62.5 ns en un micro a 16MHz).

---

## 🎯 Placas Recomendadas
* **Arduino Uno / Nano (ATmega328P)**: Arquitectura Harvard de 8 bits donde la diferencia entre Flash y SRAM es evidente.
* **ESP32 / Raspberry Pi Pico / Arduino Uno R4**: Arquitecturas de 32 bits para observar cómo el tamaño de punteros y de tipos nativos cambia.

