# Módulo 04: C++ Moderno en Sistemas Embebidos (C++11 a C++23)

Este módulo aborda la revolución del C++ moderno aplicado a microcontroladores. Demuestra con ejemplos prácticos cómo los estándares **C++11, C++14, C++17, C++20 y C++23** transforman el firmware embebido en código más seguro, más legible y más rápido, eliminando cálculos en tiempo de ejecución y trasladándolos a la fase de compilación.

---

## 📚 Contenido del Módulo

1. **[`01_CPP11_Embedded/`](file:///e:/PROJECTS/CPP_codes/04_Modern_CPP_Versions_Embedded/01_CPP11_Embedded)**:
   * `constexpr`: Cálculo matemático previo de tablas y factores en compilación (0 ciclos de CPU en el micro).
   * `enum class`: Enumeraciones fuertemente tipadas con almacenamiento explícito de 1 byte (`: uint8_t`).
   * `static_assert`: Pruebas unitarias de arquitectura en tiempo de compilación.
   * `nullptr`, `auto` y lambdas sin captura como callbacks de costo cero.

2. **[`02_CPP14_CPP17_Embedded/`](file:///e:/PROJECTS/CPP_codes/04_Modern_CPP_Versions_Embedded/02_CPP14_CPP17_Embedded)**:
   * **C++14**: Literales binarios (`0b10110001`), separadores de dígitos (`16'000'000UL`) y `constexpr` relajado con bucles.
   * **C++17**:
     * `if constexpr`: Eliminación completa de ramas de código en tiempo de compilación (cero Flash desperdiciada).
     * Structured Bindings: Desempaquetado de lecturas múltiples de sensores (`auto [x, y, z]`).
     * `[[nodiscard]]`: Obligatoriedad de revisar códigos de error en llamadas de comunicación.
     * `inline variables`: Constantes globales limpias en archivos de cabecera.

3. **[`03_CPP20_CPP23_Concepts_Metaprog/`](file:///e:/PROJECTS/CPP_codes/04_Modern_CPP_Versions_Embedded/03_CPP20_CPP23_Concepts_Metaprog)**:
   * **C++20 Concepts**: Validación elegante de tipos de hardware en compilación (reemplazo de SFINAE).
   * `consteval`: Garantía estricta de cómputo en compilación.
   * Operador `<=>` (Spaceship) para ordenamiento automático.
   * Vista previa de **C++23**: `std::to_underlying`, `if consteval` y el impacto de `std::expected` frente al desuso de excepciones.

---

## 🎯 Placas Recomendadas y Toolchains
* **Arduino Uno / Nano (AVR-GCC)**: Excelente para C++11, C++14 y gran parte de C++17 a nivel de lenguaje.
* **ESP32 / Raspberry Pi Pico / STM32 (ARM/Xtensa GCC 11+)**: Soportan la totalidad de C++17, C++20 y características de C++23.
