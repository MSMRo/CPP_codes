# 🚀 Master Tutorial: C, ANSI C y C++ para Sistemas Embebidos y Arduino

Bienvenido a la colección exhaustiva de tutoriales prácticos de programación en **C, ANSI C y C++ Moderno (C++11 hasta C++23)** orientada a **sistemas embebidos y microcontroladores**.

Este repositorio está diseñado para llevarte desde la manipulación directa de bits y registros en C clásico hasta las técnicas más avanzadas de metaprogramación en tiempo de compilación y abstracciones de costo cero en C++ moderno, utilizando como plataformas principales **Arduino Uno / Nano (AVR ATmega328P)**, **ESP32**, **Raspberry Pi Pico (RP2040)** y **Arduino Uno R4 (ARM Cortex-M4)**.

---

## 🧭 Mapa de Aprendizaje (Ruta Pedagógica)

```
[01. C Embebido Básico] ────────► [02. ANSI C y Librerías] ────────► [03. C++ Embebido Clásico] ────────► [04. C++ Moderno (11/14/17/20/23)]
  • Tipos exactos (stdint)         • Structs y Packing                • Clases vs Structs (costo 0)       • constexpr / consteval
  • Punteros y Mapa SRAM           • Uniones y Type Punning           • Regla de oro de Constructores     • enum class / auto / nullptr
  • Registros DDRx / PORTx         • Punteros a Funciones / ISR       • begin() pattern vs Init           • if constexpr / [[nodiscard]]
  • Máscaras y Operaciones Bit     • Máquinas de Estado (FSM)         • Vtables vs Polimorfismo Estático  • Concepts y Templates
                                   • Opaque Pointers / Handles C                                          • std::span / Structured Bindings
```

---

## 📁 Estructura del Repositorio

| Directorio | Temas Principales | Compatibilidad Principal |
| :--- | :--- | :--- |
| **[`01_C_Basics_Embedded/`](file:///e:/PROJECTS/CPP_codes/01_C_Basics_Embedded)** | Fundamentos del hardware, tipos de datos exactos (`stdint.h`), punteros en memoria SRAM, manipulación directa de registros GPIO con máscaras binarias. | Arduino Uno, Nano, Mega, ESP32, STM32, RP2040 |
| **[`02_ANSI_C_Embedded_Libraries/`](file:///e:/PROJECTS/CPP_codes/02_ANSI_C_Embedded_Libraries)** | Sintaxis estricta ANSI C (C89/C99): `struct`, empaquetamiento de bits (`bit-fields`), uniones para comunicación serie, punteros a funciones (callbacks de interrupciones), máquinas de estado finitas (FSM) y arquitectura modular con archivos `.h` y `.c` (Punteros opacos / Encapsulamiento estilo FreeRTOS). | Cualquier microcontrolador de 8, 16 o 32 bits |
| **[`03_CPP_Foundations_Embedded/`](file:///e:/PROJECTS/CPP_codes/03_CPP_Foundations_Embedded)** | C++ orientado a objetos para bare-metal: clases con costo de memoria idéntico a `struct`, por qué **nunca** inicializar hardware en constructores globales (patrón `begin()`), el costo oculto de las funciones virtuales (`__vptr` / Vtable) y cómo reemplazarlo con polimorfismo estático. | Arduino Uno, ESP32, RP2040, STM32 |
| **[`04_Modern_CPP_Versions_Embedded/`](file:///e:/PROJECTS/CPP_codes/04_Modern_CPP_Versions_Embedded)** | Guía práctica y código de C++11, C++14, C++17, C++20 y C++23: `constexpr` para cálculos en compilador, `enum class`, `static_assert`, `if constexpr`, templates sin sobrecarga de runtime, y conceptos (Concepts) en microcontroladores modernos. | AVR (C++11/C++14/C++17 parcial), ESP32 / RP2040 (C++17/C++20/C++23) |
| **[`GUIA_VERSIONES_CPP_EMBEBIDO.md`](file:///e:/PROJECTS/CPP_codes/GUIA_VERSIONES_CPP_EMBEBIDO.md)** | **Documento Maestro**: Análisis profundo de todas las versiones de C++, tabla de compatibilidad de compiladores, pros y contras para microcontroladores y cómo habilitar cada estándar. | Documentación técnica |

---

## 🛠️ Cómo Utilizar los Ejemplos

Cada subcarpeta de este repositorio es un proyecto autónomo que contiene:
1. Un archivo fuente principal `.ino` (abrible directamente en el **Arduino IDE**).
2. Un archivo `platformio.ini` (abrible como proyecto en **PlatformIO / VS Code**).
3. Un archivo `README.md` específico con la explicación de la sintaxis, análisis de memoria y guía paso a paso.

### Opción A: Usando PlatformIO (Recomendado para desarrollo profesional)
1. Abre VS Code con la extensión **PlatformIO IDE** instalada.
2. Selecciona **File -> Open Folder** y elige cualquiera de las subcarpetas del tutorial (por ejemplo: `01_C_Basics_Embedded/01_Tipos_y_Memoria`).
3. PlatformIO detectará automáticamente el archivo `platformio.ini`.
4. Conecta tu placa (por ejemplo Arduino Uno o ESP32) mediante USB.
5. Haz clic en el icono de compilar (✓) o subir (➔) en la barra inferior de PlatformIO.

### Opción B: Usando Arduino IDE (v1.8.x o v2.x)
1. Abre Arduino IDE.
2. Ve a **Archivo -> Abrir** y selecciona el archivo `.ino` dentro de la subcarpeta que desees estudiar.
3. Si el proyecto incluye archivos `.h` o `.c`/`.cpp` adicionales (como en `02_ANSI_C_Embedded_Libraries/04_Libreria_Modular_C`), el Arduino IDE los abrirá automáticamente como pestañas contiguas.
4. Selecciona tu placa en **Herramientas -> Placa** (ej. *Arduino Uno*) y el puerto serial correspondiente.
5. Haz clic en **Subir**.

### Opción C: Validando con Arduino CLI
Puedes compilar y verificar cualquier código desde la terminal usando `arduino-cli`:
```bash
# Compilar para Arduino Uno (ATmega328P):
arduino-cli compile --fqbn arduino:avr:uno 01_C_Basics_Embedded/01_Tipos_y_Memoria

# Compilar para ESP32:
arduino-cli compile --fqbn esp32:esp32:esp32 04_Modern_CPP_Versions_Embedded/02_CPP14_CPP17_Embedded
```

---

## 🎯 Filosofía de Código Embebido en Este Repositorio

1. **Determinismo**: Cada ciclo de instrucción y cada byte de memoria importa.
2. **Costo Cero**: Usar el compilador para hacer el trabajo pesado antes de que el microcontrolador despierte.
3. **Seguridad de Tipos**: Usar `stdint.h` (`uint8_t`, `int16_t`, `uint32_t`) en lugar de `int` o `long` genéricos cuyos tamaños varían entre arquitecturas de 8 y 32 bits.
4. **Cero Heap Innecesario**: Evitar la fragmentación de la memoria SRAM prescindiendo de llamadas arbitrarias a `malloc` o `new` en tiempo de ejecución.
