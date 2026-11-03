# Tutorial 11 (C++11): La Base del C++ Moderno en Sistemas Embebidos

## 🎯 Objetivo de Aprendizaje
Aprender y aplicar las características de **C++11** que revolucionaron el firmware de microcontroladores: evaluación previa en tiempo de compilación con `constexpr`, enumeraciones con límite de tamaño de memoria y espacio de nombres con `enum class`, validaciones arquitectónicas con `static_assert`, punteros seguros con `nullptr`, inferencia con `auto` y lambdas de costo cero como callbacks.

---

## 🔬 Características Clave de C++11 en Microcontroladores

### 1. `constexpr`: Cero Ciclos de CPU en Runtime
A diferencia de `const` (que crea una variable de solo lectura que sigue existiendo en memoria), `constexpr` garantiza que la expresión se evalúa durante la compilación en la PC:
```cpp
constexpr uint16_t calcularUartBaud(uint32_t f_cpu, uint32_t baud) {
    return static_cast<uint16_t>((f_cpu / (16UL * baud)) - 1);
}
constexpr uint16_t UBRR_VAL = calcularUartBaud(16000000UL, 115200);
```
El microcontrolador no ejecuta divisiones de 32 bits en su ciclo de reloj; el binario ya contiene directamente el número final precalculado.

### 2. `enum class` con Tipo Subyacente (`: uint8_t`)
Los enums de C clásico tienen dos grandes problemas:
* Contaminan el espacio de nombres global (dos enums no pueden compartir nombres de estados como `ERROR`).
* El compilador suele asignarles tamaño de `int` (2 o 4 bytes).

En C++11:
```cpp
enum class EstadoSensor : uint8_t {
    DESCONECTADO = 0,
    LISTO        = 1,
    ERROR_CRC    = 2
};
```
* Ocupa **exactamente 1 byte** de SRAM.
* Requiere prefijo (`EstadoSensor::LISTO`), evitando colisiones de nombres.
* No permite conversiones implícitas peligrosas a enteros.

### 3. `static_assert`: Verificaciones en Tiempo de Compilación
Permite verificar condiciones críticas antes de generar el binario:
```cpp
static_assert(sizeof(EstadoSensor) == 1, "EstadoSensor debe medir 1 byte");
```
Si cambias de placa o modificas un tipo y rompes una restricción de memoria, el compilador aborta inmediatamente con tu mensaje personalizado.

### 4. Lambdas sin Captura como Callbacks C
Una función lambda declarada como `[](uint8_t pin) { ... }` (sin corchetes de captura `[]`) tiene la propiedad mágica de decaer automáticamente a un puntero de función normal de C (`void (*)(uint8_t)`). No consume memoria dinámica (*heap*) ni añade sobrecarga.

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **PlatformIO**: Abrir la carpeta `01_CPP11_Embedded`, compilar y subir.
* **Arduino IDE**: Abrir `01_CPP11_Embedded.ino`.
