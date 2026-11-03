# Guía Definitiva: Versiones de C++ en Sistemas Embebidos y Microcontroladores

Este documento analiza en profundidad la evolución del estándar C++ (**C++98/03, C++11, C++14, C++17, C++20 y C++23**), su impacto en el desarrollo para microcontroladores (AVR, ESP32, STM32, RP2040, etc.), qué características son **abstracciones de costo cero** recomendadas y cuáles deben evitarse en sistemas con recursos limitados.

---

## 1. El Paradigma de C++ en Sistemas Embebidos

Históricamente, la programación de microcontroladores se realizaba exclusivamente en Ensamblador o ANSI C. Existe aún el mito de que *"C++ es demasiado pesado o lento para microcontroladores"*. 

En la práctica moderna, **C++ bien escrito es tan rápido y compacto como C puro**, e incluso puede generar código de máquina más pequeño y eficiente gracias a:
1. **Comprobación estricta de tipos en tiempo de compilación** (sin costo en tiempo de ejecución).
2. **Metaprogramación y funciones `constexpr`**: cálculos complejos ejecutados por la computadora durante la compilación, ahorrando ciclos de CPU y memoria Flash en el microcontrolador.
3. **Inlining agresivo**: el compilador reemplaza llamadas a métodos pequeños por instrucciones directas a registros de hardware.
4. **Encapsulamiento sin sobrecarga**: una clase sin funciones virtuales (`virtual`) tiene exactamente el mismo tamaño en SRAM que una estructura (`struct`) de C.

### La Regla de Oro del C++ Embebido: "No pagues por lo que no usas"
Bjarne Stroustrup (creador de C++) diseñó el lenguaje bajo este principio. En microcontroladores, adoptamos:
* **Habilitar**: Abstracciones de costo cero (Templates, `constexpr`, `inline`, namespaces, `enum class`, referencias, constructores simples).
* **Deshabilitar en compilador**: Excepciones (`-fno-exceptions`) y RTTI (`-fno-rtti`). En microcontroladores pequeños, el soporte de excepciones puede inflar la memoria Flash en decenas de kilobytes y causar paradas críticas impredecibles.
* **Evitar**: Asignación dinámica continua de memoria en el Heap (`malloc`, `new`, `std::string`, `std::vector`) para prevenir la temida **fragmentación de la SRAM**.

---

## 2. Tabla Comparativa de Estándares C++ en Embebidos

| Estándar | Año | Estado en Embebidos | Soporte AVR (Uno/Nano) | Soporte ESP32 / RP2040 / STM32 | Características Estrella para MCU | Riesgos / Advertencias |
| :--- | :---: | :---: | :---: | :---: | :--- | :--- |
| **C++98 / C++03** | 1998/2003 | Obsoleto | Soportado (Histórico) | Soportado | Clases, herencia básica, plantillas básicas. | Código verboso, falta de tipos explícitos para punteros (`NULL` en vez de `nullptr`). |
| **C++11** | 2011 | Estándar base universal | **Estándar por defecto** en Arduino AVR | Totalmente soportado | `auto`, `nullptr`, `constexpr`, `enum class`, `static_assert`, lambdas sin captura, `override`. | Cuidado con `std::function` (usa Heap internamente si la captura excede el búfer local). |
| **C++14** | 2014 | Maduro y muy estable | Muy buen soporte (`-std=gnu++14`) | Totalmente soportado | `constexpr` relajado, literales binarios (`0b10101010`), `auto` en retornos de funciones, lambdas genéricas. | Prácticamente sin desventajas respecto a C++11; muy recomendado. |
| **C++17** | 2017 | **Punto óptimo actual (Sweet Spot)** | Buen soporte de sintaxis (`-std=gnu++17`) | **Estándar por defecto** en ESP-IDF / Arduino ESP32 core 3.x | `if constexpr`, Structured Bindings, `inline variables`, `std::string_view` (32-bit), atributos `[[nodiscard]]`. | En AVR no hay soporte completo de STL (no hay `<string_view>` nativo en AVR-Libc, pero la sintaxis del lenguaje sí funciona). |
| **C++20** | 2020 | Moderno / Alta eficiencia | Soporte parcial (compiladores recientes) | Soportado con GCC 11+ / 13+ | **Concepts**, `std::span`, `consteval`, operadores de comparación nave espacial `<=>`, `bit_cast`. | El soporte en toolchains AVR muy antiguos (GCC 7.3) es limitado. Excelente para ARM Cortex y ESP32. |
| **C++23** | 2023 | De vanguardia | Soporte experimental/limitado | Soportado en toolchains recientes (GCC 13/14) | `std::expected` (manejo de errores sin excepciones), `std::to_underlying`, `if consteval`. | Ecosistemas y toolchains de producción aún están adoptándolo gradualmente. |

---

## 3. Análisis Detallado por Estándar

### 3.1. C++98 / C++03
* **Contexto**: El primer estándar formal.
* **Pros**: Introduce programación orientada a objetos estricta, namespaces y plantillas.
* **Contras**:
  * Ambigüedad de `NULL` (definido como `0` entero, susceptible a sobrecargas erróneas).
  * No existían constantes evaluadas en compilación (`constexpr`), dependiendo de macros `#define` propensas a errores.
  * Código muy verboso al declarar iteradores o tipos complejos.
* **Uso actual**: No recomendado para proyectos nuevos.

---

### 3.2. C++11: La Revolución del Embebido
C++11 transformó por completo la seguridad y eficiencia del código en sistemas embebidos.

#### Características Clave:
1. `nullptr`: Puntero fuertemente tipado que evita bugs al sobrecargar funciones con enteros y punteros.
2. `constexpr`: Expresiones evaluadas en tiempo de compilación. Las tablas de senos, factores de conversión y máscaras de bits se calculan en la PC del desarrollador, no en el ciclo de reloj del microcontrolador.
3. `enum class` (Scoped Enums): Enums con tipo de almacenamiento explícito (`uint8_t`), sin colisión de nombres en el scope global y sin conversión implícita a enteros.
4. `static_assert`: Comprobación de condiciones en tiempo de compilación (ej. validar que un búfer no supere el tamaño de la página de memoria antes de generar el binario).
5. `auto`: Deducción de tipos sin sobrecarga de runtime.
6. `override` y `final`: Evita errores al sobreescribir métodos polimórficos.
7. Delegación de constructores y miembros inicializados en la declaración de clase.

```cpp
// Ejemplo C++11 Embebido:
enum class EstadoLed : uint8_t { APAGADO = 0, ENCENDIDO = 1 };

constexpr uint16_t calcularBaudRate(uint32_t f_cpu, uint32_t baud) {
    return (f_cpu / (16UL * baud)) - 1;
}

static_assert(sizeof(EstadoLed) == 1, "EstadoLed debe ocupar solo 1 byte en SRAM");
```

---

### 3.3. C++14: Refinamiento y Flexibilidad
C++14 pule las restricciones de C++11 sin agregar sobrecarga.

#### Características Clave:
1. **Literales Binarios**: Permite escribir directamente valores binarios legibles para registros de hardware:
   ```cpp
   uint8_t mascara = 0b00101100; // Extremadamente claro para configurar registros (DDRB, PORTB, etc.)
   ```
2. **Separadores de Dígitos**: Uso de comilla simple para legibilidad:
   ```cpp
   constexpr uint32_t FRECUENCIA_CPU = 16'000'000UL; // 16 MHz
   ```
3. **`constexpr` relajado**: En C++11 una función `constexpr` solo podía contener una sentencia `return`. En C++14 puede contener bucles `for`, sentencias `if`, y variables locales evaluadas en compilación.
4. **Lambdas Genéricas**: Permite escribir callbacks reutilizables con parámetros `auto`.

---

### 3.4. C++17: El Punto Óptimo para Embebidos Modernos
C++17 es ampliamente considerado el mejor balance entre madurez de compiladores y expresividad de costo cero para microcontroladores.

#### Características Clave:
1. `if constexpr`: Condicional evaluado en **tiempo de compilación**. El compilador descarta completamente el código de la rama falsa antes de generar el binario, ahorrando Flash y eliminando saltos de ejecución:
   ```cpp
   template <typename T>
   void enviarDato(T valor) {
       if constexpr (sizeof(T) == 1) {
           UART_TransmitByte(valor);
       } else {
           UART_TransmitBuffer(reinterpret_cast<uint8_t*>(&valor), sizeof(T));
       }
   }
   ```
2. **Structured Bindings** (`auto [x, y] = funcion();`): Permite retornar múltiples valores (como lecturas de sensores X/Y/Z) empaquetados en tuplas o structs sin punteros de salida.
3. `inline variables`: Permite definir constantes estáticas o instancias globales dentro de archivos `.h` sin violar la Regla de Definición Única (ODR).
4. `[[nodiscard]]`: Atributo del compilador que genera una advertencia si el llamador ignora el valor de retorno (vital para funciones que retornan códigos de error como `Status::ERROR_I2C`).
5. `std::string_view` (en arquitecturas de 32 bits): Pasa cadenas de texto constantes sin duplicar memoria ni usar asignación dinámica.

---

### 3.5. C++20: Metaprogramación con Seguridad Extrema
C++20 introduce cambios profundos en el sistema de tipos y la compilación.

#### Características Clave:
1. **Concepts (Conceptos)**: Permiten restringir plantillas de manera legible, sustituyendo el críptico `std::enable_if` (SFINAE):
   ```cpp
   template <typename T>
   concept PinDigital = requires(T pin) {
       { pin.setHigh() } -> std::same_as<void>;
       { pin.setLow() } -> std::same_as<void>;
   };
   ```
2. `std::span`: Vista segura y no propietaria sobre un array contiguo de datos (búfer de comunicación, array ADC), evitando pasar `uint8_t* ptr, size_t size` por separado.
3. `consteval`: Garantiza de forma estricta que una función **DEBE** evaluarse en compilación; si no puede resolverse en tiempo de compilación, el compilador arroja un error.
4. Operador Nave Espacial `<=>` (Three-way comparison): Genera automáticamente todos los operadores relacionales (`<`, `<=`, `==`, `!=`, `>`, `>=`).
5. Operaciones de bits nativas (`<bit>`: `std::rotl`, `std::rotr`, `std::countl_zero`, `std::popcount`): Optimizadas directamente en instrucciones de hardware en arquitecturas ARM y ESP32.

---

### 3.6. C++23: Hacia la Seguridad de Memoria y Manejo de Errores Limpio
C++23 continúa reduciendo la necesidad de excepciones y código inseguro.

#### Características Clave:
1. `std::expected<T, E>`: La alternativa moderna perfecta a las excepciones en embebidos. Permite retornar el valor deseado o un código de error de manera fuertemente tipada:
   ```cpp
   std::expected<float, ErrorSensor> leerTemperatura();
   ```
2. `std::to_underlying`: Convierte un `enum class` a su tipo entero subyacente sin casts manuales propensos a errores.
3. `if consteval`: Diferencia claramente la ejecución en tiempo de compilación versus tiempo de ejecución dentro de una misma función.

---

## 4. Diferencias Arquitectónicas: 8-bit (AVR) vs 32-bit (ESP32, STM32, RP2040)

Al aplicar C++ en microcontroladores, la arquitectura física determina qué herramientas de C++ tienen sentido:

| Aspecto | Microcontroladores 8-bit (AVR: Uno, Nano, Mega) | Microcontroladores 32-bit (ESP32, RP2040, STM32) |
| :--- | :--- | :--- |
| **Memoria SRAM** | Muy escasa (0.5 KB a 8 KB). Arduino Uno tiene **2 KB**. | Abundante (128 KB a 8 MB). ESP32 tiene **320+ KB**. |
| **Memoria Flash** | 16 KB a 256 KB (Uno: 32 KB). | 1 MB a 16 MB. |
| **Soporte de STL** | Mínimo en `avr-libc`. No incluye cabeceras pesadas como `<vector>`, `<string>`, `<thread>`. | Completo (GNU libstdc++ incluida en toolchain de ESP32 y ARM). |
| **Punteros (Tamaño)** | 16 bits (2 bytes). | 32 bits (4 bytes). |
| **Funciones Virtuales** | Cada puntero de vtable (`__vptr`) cuesta 2 bytes por objeto; llamadas indirectas consumen varios ciclos. | Muy viables si la arquitectura lo amerita, pero se prefieren plantillas para periféricos críticos. |
| **Estándar C++ Recomendado** | **C++14 / C++17** (usando características del lenguaje, sin librerías estándar pesadas). | **C++17 / C++20** (aprovechando `std::span`, `string_view`, concepts). |

---

## 5. Cómo Configurar el Estándar de C++

### 5.1. En PlatformIO (`platformio.ini`)
PlatformIO permite especificar exactamente la bandera del compilador GCC mediante `build_flags`:

```ini
[env:uno_cpp17]
platform = atmelavr
board = uno
framework = arduino
build_unflags = -std=gnu++11
build_flags = 
    -std=gnu++17
    -fno-exceptions
    -fno-rtti
    -Wall
    -Wextra

[env:esp32_cpp20]
platform = espressif32
board = esp32dev
framework = arduino
build_unflags = -std=gnu++11
build_flags = 
    -std=gnu++20
    -fno-exceptions
    -fno-rtti
```

### 5.2. En Arduino IDE
Por defecto, Arduino IDE compila con el estándar fijado en la definición de la placa (por ejemplo, `-std=gnu++11` en AVR Boards, y `-std=gnu++17` o `-std=gnu++2b` en ESP32 Core v3).

Para cambiar el estándar en Arduino IDE manualmente:
1. Localiza el archivo `platform.txt` o crea un archivo de anulación local llamado `platform.local.txt` en la carpeta del core (por ejemplo, en `.../packages/arduino/hardware/avr/1.8.x/platform.local.txt`).
2. Agrega la siguiente línea para cambiar a C++17:
   ```properties
   compiler.cpp.flags=-c -g -Os -w -std=gnu++17 -fpermissive -fno-exceptions -ffunction-sections -fdata-sections -fno-threadsafe-statics -Wno-error=narrowing -MMD -flto
   ```

---

## 6. Resumen: Qué Usar y Qué Evitar en Embebidos

###  Prácticas Fuertemente Recomendadas:
* Usar `constexpr` para cualquier cálculo constante o mapeo matemático.
* Usar `enum class` con tipo subyacente explícito (`: uint8_t`).
* Usar `auto` para tipos de iteradores complejos o tipos derivados.
* Usar `nullptr` siempre en lugar de `NULL` o `0`.
* Usar `static_assert` para garantizar en compilación que tamaños de buffers o configuraciones sean válidas.
* Usar estructuras y clases por valor o por referencia constante (`const Tipo&`).
* Inicializar hardware en un método explícito (ej. `begin()`) y nunca en el constructor de objetos globales.

### 🚫 Prácticas Desaconsejadas o Prohibidas en MCUs Pequeños:
* **Excepciones (`try`, `catch`, `throw`)**: Inaceptable overhead de memoria e indeterminismo temporal.
* **RTTI (`dynamic_cast`, `typeid`)**: Aumenta el tamaño del ejecutable y requiere tablas de tipos en Flash.
* **Asignación dinámica libre (`new`, `delete`, `malloc`, `free`) dentro del bucle principal (`loop()` o tareas de alta frecuencia)**: Causa fragmentación y eventual bloqueo del sistema.
* **`std::vector` o `std::string` sin reserva previa**: Provocan constantes realocaciones de memoria heap.
