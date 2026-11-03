# Tutorial 13 (C++20 & C++23): Conceptos (Concepts) y Metaprogramación

## 🎯 Objetivo de Aprendizaje
Aprender la vanguardia de C++ para sistemas embebidos de 32 bits (ESP32, RP2040, STM32): cómo los **Concepts (Conceptos)** de C++20 revolucionan la programación genérica sustituyendo el críptico SFINAE (`std::enable_if`), cómo `consteval` garantiza que los cálculos ocurran exclusivamente en tiempo de compilación, el operador nave espacial `<=>`, y cómo **C++23** transforma el manejo de errores en microcontroladores mediante `std::expected`.

---

## 🔬 Características de Vanguardia en Firmware

### 1. C++20 Concepts: Restricciones de Hardware Legibles
En C++ tradicional, si creabas una función de plantilla para controlar un pin y el usuario le pasaba un tipo incorrecto, el compilador generaba decenas de líneas de errores crípticos e incomprensibles.

Con C++20 defines un **Concepto**:
```cpp
template <typename T>
concept PinSalidaDigital = requires(T pin) {
    { pin.high() } -> std::same_as<void>;
    { pin.low() }  -> std::same_as<void>;
};

// La función ahora exige explícitamente que T cumpla el concepto:
template <PinSalidaDigital TPin>
void parpadearRapido(TPin& pinSalida);
```
Si alguien intenta pasar un sensor de temperatura a `parpadearRapido`, el compilador detiene la compilación inmediatamente indicando con claridad meridiana:
> *"Error: El tipo 'SensorTemp' no satisface el concepto 'PinSalidaDigital' porque le falta el método .high()"*.

### 2. `consteval`: Evaluación Estricta en Compilación
A diferencia de `constexpr` (que puede ejecutarse tanto en compilación como en tiempo de ejecución si los argumentos son dinámicos), `consteval` produce una función inmediata (*Immediate Function*). Si la función no puede resolverse al 100% en tiempo de compilación, el código **no compila**. Es la garantía absoluta de que el microcontrolador no gastará ciclos de CPU calculando el valor.

### 3. Operador Nave Espacial `<=>` (Three-way Comparison)
En sistemas embebidos se transmiten miles de paquetes de telemetría y lecturas de sensores que deben ser ordenados en buffers o comparados.
En C++20:
```cpp
struct PaqueteMuestreo {
    uint16_t idNodo;
    uint16_t lecturaAdc;

    auto operator<=>(const PaqueteMuestreo&) const = default;
};
```
Con una sola línea `= default`, el compilador genera de forma óptima los operadores `<`, `<=`, `==`, `!=`, `>=`, `>`.

### 4. C++23: `std::expected` (El Fin Definitivo de las Excepciones)
En sistemas embebidos, las excepciones (`throw`, `try-catch`) están prohibidas (`-fno-exceptions`) porque causan sobrecarga masiva de Flash e indeterminismo.
C++23 introduce formalmente `std::expected<T, E>`:
```cpp
// Retorna un float (el dato deseado) O un enum de error de hardware:
std::expected<float, ErrorI2C> leerSensorPresion();
```
Permite un control de errores tipado, expresivo, seguro y con costo de ejecución idéntico a retornar un entero tradicional.

---

## 📋 Configuración y Ejecución

* **Plataformas Óptimas**: ESP32, ESP32-S3, Raspberry Pi Pico (RP2040), Arduino Uno R4 (compiladores GCC 10+).
* **Compatibilidad 8-bit**: En Arduino Uno / Nano (GCC 7.3), el código detecta las capacidades del compilador mediante macros estándar (`__cpp_concepts`) y activa automáticamente fallbacks elegantes para compilar sin errores.
* **Archivos**:
  * `cpp20_features.h`: Implementación de plantillas, conceptos y estructuras.
  * `03_CPP20_CPP23_Concepts_Metaprog.ino`: Sketch ejecutable.
  * `platformio.ini`: Configurado con `build_flags = -std=gnu++20` para ESP32.
* **Velocidad Serial**: 115200 baudios.
