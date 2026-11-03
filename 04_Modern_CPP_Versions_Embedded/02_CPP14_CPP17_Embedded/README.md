# Tutorial 12 (C++14 & C++17): Características Avanzadas de Costo Cero

## 🎯 Objetivo de Aprendizaje
Dominar las herramientas más potentes de **C++14 y C++17** para desarrollo embebido: optimización de ramas muertas en compilación mediante `if constexpr`, desempaquetado limpio con *Structured Bindings*, verificación obligatoria de errores con `[[nodiscard]]`, y constantes globales de cabecera con `inline static`.

---

## 🔬 Las Características Más Relevantes en Firmware

### 1. `if constexpr`: Poda de Ramas en Tiempo de Compilación
En C tradicional, si escribes un `if (sizeof(T) == 1)` dentro de una función genérica o macro, el compilador compila ambas ramas y genera instrucciones de salto condicional en tiempo de ejecución.
Con `if constexpr`:
```cpp
template <typename T>
void transmitirDatoSerial(T valor) {
    if constexpr (sizeof(T) == 1) {
        UART_SendByte(valor);
    } else {
        UART_SendBuffer(&valor, sizeof(T));
    }
}
```
El compilador evalúa la condición durante la compilación. Para un `uint8_t`, la rama `else` es **eliminada por completo del binario**. No existe ningún salto ni un solo byte de código sobrante en la memoria Flash.

### 2. Structured Bindings (`auto [x, y, z] = funcion();`)
Permite retornar múltiples lecturas de un sensor empaquetadas en un `struct` y asignarlas de manera legible a variables locales sin punteros de salida ni llamadas repetidas:
```cpp
auto [temp, hum, pres] = leerEstacionClima();
```

### 3. El Atributo `[[nodiscard]]`
En sistemas embebidos, omitir la revisión de un código de error retornado por un bus I2C o SPI suele ser la causa principal de bloqueos intermitentes. Al marcar la función con `[[nodiscard]]`:
```cpp
[[nodiscard]] CodigoRetorno iniciarPeriferico(uint8_t pin);

// Si alguien escribe:
iniciarPeriferico(5); // El compilador arroja un WARNING/ERROR inmediato advirtiendo que ignoró el retorno
```

### 4. `inline variables`
En C++ antes de C++17, declarar `const uint16_t TIMEOUT = 500;` dentro de un archivo `.h` provocaba que cada archivo `.cpp` que lo incluyera creara su propia copia duplicada en memoria. Con `inline static const uint16_t TIMEOUT = 500;`, el estándar garantiza una única instancia compartida por todo el ejecutable sin violar la ODR (One Definition Rule).

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, ESP32, STM32, RP2040.
* **Archivos**:
  * `tipos_cpp17.h`: Contiene las estructuras y tipos necesarios para C++17.
  * `02_CPP14_CPP17_Embedded.ino`: Código principal.
  * `platformio.ini`: Configurado con `build_flags = -std=gnu++17`.
* **Velocidad Serial**: 115200 baudios.
