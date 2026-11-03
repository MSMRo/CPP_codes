# Módulo 02: ANSI C (C89/C99) y Arquitectura de Librerías Modulares

Este módulo profundiza en la programación estructurada en **ANSI C estándar** aplicada a sistemas embebidos, diseño de controladores de hardware (drivers), empaquetamiento binario de protocolos de comunicación y desarrollo de librerías portables compatibles con C y C++.

---

## 📚 Contenido del Módulo

1. **[`01_Structs_Unions_Bitfields/`](file:///e:/PROJECTS/CPP_codes/02_ANSI_C_Embedded_Libraries/01_Structs_Unions_Bitfields)**:
   * Definición de estructuras (`struct`) y alineación en memoria (Padding de bytes).
   * Empaquetamiento forzado con `__attribute__((packed))` para tramas UART/I2C/SPI sin bytes fantasma.
   * Uniones (`union`) y *Type Punning*: cómo descomponer un `float` o `uint32_t` en un array de 4 bytes sin llamadas a funciones.
   * Campos de bits (*Bit-fields*) para modelar registros de hardware y banderas de estado con precisión de 1 bit.

2. **[`02_Punteros_Funciones_Callbacks/`](file:///e:/PROJECTS/CPP_codes/02_ANSI_C_Embedded_Libraries/02_Punteros_Funciones_Callbacks)**:
   * Sintaxis de punteros a funciones en ANSI C (`typedef void (*Callback_t)(...)`).
   * Patrón Callback: cómo desacoplar la lógica de un periférico (ej. pulsador, temporizador) de la lógica de aplicación.
   * Tablas de despacho (*Dispatch Tables*): sustitución de `switch-case` gigantes por tablas de punteros en memoria Flash/RAM para ejecución en tiempo constante $O(1)$.

3. **[`03_Maquina_Estados_FSM/`](file:///e:/PROJECTS/CPP_codes/02_ANSI_C_Embedded_Libraries/03_Maquina_Estados_FSM)**:
   * Diseño de Máquinas de Estados Finitas (FSM) no bloqueantes en C puro.
   * Arquitectura basada en eventos y estados (`enum` + funciones controladoras).
   * Eliminación definitiva de llamadas a `delay()` que congelan el microcontrolador.

4. **[`04_Libreria_Modular_C/`](file:///e:/PROJECTS/CPP_codes/02_ANSI_C_Embedded_Libraries/04_Libreria_Modular_C)**:
   * Creación de un driver/librería en C puro siguiendo estándares profesionales:
     * Separación de interfaz (`.h`) e implementación (`.c`).
     * Compatibilidad universal con C++ mediante `extern "C"`.
     * Encapsulamiento en C: funciones privadas mediante `static` y patrón de puntero opaco (*Opaque Handle*).
