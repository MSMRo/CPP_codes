# Guía Completa de `arduino-cli`: Automatización, Gestión de Tarjetas y C++ Moderno

Guía de referencia para configuración, compilación, exportación de binarios, gestión de librerías y selección de estándares de C/C++ en sistemas embebidos mediante la interfaz de línea de comandos oficial de Arduino (`arduino-cli`).

---

## Índice

1. [Instalación](#1-instalación)
2. [Configuración Inicial y Soporte de Hardware](#2-configuración-inicial-y-soporte-de-hardware)
3. [Instalación de Núcleos (Cores)](#3-instalación-de-núcleos-cores)
4. [Detección de Placas e Identificación de FQBN](#4-detección-de-placas-e-identificación-de-fqbn)
5. [Gestión de Librerías (Oficiales y de Terceros)](#5-gestión-de-librerías)
6. [Flujo de Trabajo: Compilar, Exportar Binarios y Flashear](#6-flujo-de-trabajo-compilar-exportar-binarios-y-flashear)
7. [Selección de Versión de C++ y Banderas de Compilador](#7-selección-de-versión-de-c-y-banderas-de-compilador)
8. [Monitor Serie](#8-monitor-serie)
9. [Automatización con Makefile](#9-automatización-con-makefile)
10. [Diagnóstico de Estándar C++ Activo](#10-diagnóstico-de-estándar-c-activo)

---

## 1. Instalación

### Linux y macOS
Descarga e instala el binario directamente desde el script oficial:

```bash
curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
sudo mv bin/arduino-cli /usr/local/bin/
```

### Windows
Instalación mediante gestores de paquetes oficiales desde PowerShell o CMD:

```cmd
:: Vía Windows Package Manager (winget)
winget install Arduino.ArduinoCLI

:: O vía Chocolatey
choco install arduino-cli
```

Verifica la instalación:
```bash
arduino-cli version
```

---

## 2. Configuración Inicial y Soporte de Hardware

1. **Crear el archivo de configuración base (`arduino-cli.yaml`):**
   ```bash
   arduino-cli config init
   ```

2. **Agregar URLs de tarjetas de terceros:**
   Para compilar en plataformas como ESP32 o Raspberry Pi Pico (RP2040/RP2350), añade los índices de paquetes al gestor:

   ```bash
   # ESP32 (Espressif)
   arduino-cli config add board_manager.additional_urls https://espressif.github.io/arduino-esp32/package_esp32_index.json

   # Raspberry Pi Pico / RP2040 / RP2350 (Core Earle Philhower)
   arduino-cli config add board_manager.additional_urls https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json
   ```

3. **Actualizar el índice de paquetes:**
   ```bash
   arduino-cli core update-index
   ```

---

## 3. Instalación de Núcleos (Cores)

Instala el paquete de herramientas y compilador cruzado correspondiente a la arquitectura de destino:

```bash
# Arduino Nano clásico, Uno, Mega (AVR 8-bit)
arduino-cli core install arduino:avr

# Arduino Nano Every (megaAVR 8-bit)
arduino-cli core install arduino:megaavr

# Arduino Nano 33 BLE / Sense (Nordic nRF52840 / Mbed OS Cortex-M4)
arduino-cli core install arduino:mbed_nano

# ESP32 / ESP32-S3 / ESP32-C3 / ESP32-C6 (Xtensa y RISC-V)
arduino-cli core install esp32:esp32

# Raspberry Pi Pico (RP2040 / RP2350)
arduino-cli core install rp2040:rp2040
```

---

## 4. Detección de Placas e Identificación de FQBN

Conecta tu placa al puerto USB y ejecuta:

```bash
arduino-cli board list
```

Salida esperada:
```text
Port         Protocol Type              Board Name                 FQBN
COM4         serial   Serial Port (USB) Arduino Nano 33 BLE        arduino:mbed_nano:nano33ble
/dev/ttyUSB0 serial   Serial Port (USB) ESP32 Dev Module           esp32:esp32:esp32
/dev/ttyACM0 serial   Serial Port (USB) Raspberry Pi Pico          rp2040:rp2040:rpipico
```

El valor **FQBN** (*Fully Qualified Board Name*) identifica la placa y arquitectura exacta para el compilador.

---

## 5. Gestión de Librerías

### A. Librerías Oficiales / Centralizadas

* **Buscar en el registro oficial:**
  ```bash
  arduino-cli lib search bme280
  ```
* **Instalar librería:**
  ```bash
  arduino-cli lib install "Adafruit BME280 Library"
  ```
* **Instalar una versión exacta:**
  ```bash
  arduino-cli lib install "ArduinoJson"@6.21.3
  ```
* **Listar librerías instaladas:**
  ```bash
  arduino-cli lib list
  ```

### B. Librerías de Terceros o Repositorios Privados

#### Método 1: Archivo comprimido (.ZIP)
```bash
arduino-cli lib install --zip-path /ruta/a/mi_libreria.zip
# En Windows:
arduino-cli lib install --zip-path C:\descargas\LibreriaSensor.zip
```

#### Método 2: Clonado manual en el directorio de librerías
Clona directamente el repositorio en la carpeta `libraries` de tu entorno:

* **Linux:** `~/Arduino/libraries/`
* **macOS:** `~/Documents/Arduino/libraries/`
* **Windows:** `%USERPROFILE%\Documents\Arduino\libraries\`

```bash
cd ~/Arduino/libraries/
git clone https://github.com/usuario/libreria_personalizada.git
```

---

## 6. Flujo de Trabajo: Compilar, Exportar Binarios y Flashear

### 1. Crear un proyecto
```bash
arduino-cli sketch new MiProyecto
```

### 2. Compilar
```bash
arduino-cli compile --fqbn arduino:mbed_nano:nano33ble MiProyecto/MiProyecto.ino
```

### 3. Exportar binarios (`.bin`, `.hex`, `.elf`, `.map`)

Por defecto, la compilación usa rutas temporales volátiles del sistema. Para retener los binarios intermedios y finales en tu espacio de trabajo:

* **Directorio de salida centralizado (Recomendado):**
  ```bash
  arduino-cli compile --fqbn arduino:mbed_nano:nano33ble MiProyecto/MiProyecto.ino --output-dir build
  ```
  Esto generará en la carpeta `build/`:
  * `MiProyecto.ino.bin`: Imagen binaria directa para programadores flash.
  * `MiProyecto.ino.hex`: Intel HEX para cargadores estándar.
  * `MiProyecto.ino.elf`: Ejecutable ELF con símbolos para depurador GDB y desensamblado.
  * `MiProyecto.ino.map`: Mapa de memoria detallado de consumo RAM/ROM.

* **Exportar junto al sketch:**
  ```bash
  arduino-cli compile --fqbn arduino:mbed_nano:nano33ble -e MiProyecto/MiProyecto.ino
  ```
  *(Genera los binarios en `MiProyecto/build/<FQBN>/`)*.

### 4. Flashear / Subir el firmware

* **Opción A (Recomendada): Subir pasando la ruta del archivo `.ino`:**
  ```bash
  arduino-cli upload -p COM4 --fqbn arduino:mbed_nano:nano33ble MiProyecto/MiProyecto.ino
  ```

* **Opción B: Subir directamente desde la carpeta de binarios exportada:**
  ```bash
  # Si exportaste a una carpeta build manual:
  arduino-cli upload -p COM4 --fqbn arduino:mbed_nano:nano33ble --input-dir build

  # Si compilaste con la bandera -e:
  arduino-cli upload -p COM4 --fqbn arduino:mbed_nano:nano33ble --input-dir MiProyecto/build/arduino.mbed_nano.nano33ble
  ```

* **Opción C: Compilar y subir en un único paso:**
  ```bash
  arduino-cli compile --upload -p COM4 --fqbn arduino:mbed_nano:nano33ble MiProyecto/MiProyecto.ino
  ```

---

## 7. Selección de Versión de C++ y Banderas de Compilador

Arduino no expone un selector gráfico del estándar de lenguaje, pero `arduino-cli` permite inyectar o sobreescribir banderas de GCC mediante el parámetro `--build-property`.

### Forzar C++17, C++20 o C++23
Utiliza la propiedad `compiler.cpp.extra_flags` para añadir el estándar sin borrar las configuraciones de optimización originales de la arquitectura:

```bash
# Compilar forzando C++20
arduino-cli compile --fqbn arduino:mbed_nano:nano33ble --build-property "compiler.cpp.extra_flags=-std=gnu++20" MiProyecto/MiProyecto.ino

# Compilar forzando C++23 (si el GCC de la plataforma lo soporta, e.g., ESP32 Core v3.x)
arduino-cli compile --fqbn esp32:esp32:esp32 --build-property "compiler.cpp.extra_flags=-std=gnu++23" MiProyecto/MiProyecto.ino
```

> **Nota:** Se utiliza `-std=gnu++XX` en lugar de `-std=c++XX` porque los núcleos embebidos dependen de extensiones del compilador de GNU (atributos de interrupción, periféricos mapeados y bits intrínsecos).

---

## 8. Monitor Serie

Abre la terminal serie integrada sin necesidad de abrir herramientas externas:

```bash
arduino-cli monitor -p COM4 -c baudrate=115200
```
*(Para salir de la consola presiona `Ctrl + C`).*

---

## 9. Automatización con Makefile

Para proyectos de ingeniería reproducible, este `Makefile` automatiza el ciclo de vida completo:

```makefile
FQBN       := arduino:mbed_nano:nano33ble
PORT       := COM4
SKETCH     := MiProyecto/MiProyecto.ino
OUT_DIR    := build
BAUD       := 115200
CPP_STD    := -std=gnu++20

all: compile

compile:
	arduino-cli compile --fqbn $(FQBN) --build-property "compiler.cpp.extra_flags=$(CPP_STD)" $(SKETCH) --output-dir $(OUT_DIR)

upload:
	arduino-cli upload -p $(PORT) --fqbn $(FQBN) --input-dir $(OUT_DIR)

flash: compile upload

monitor:
	arduino-cli monitor -p $(PORT) -c baudrate=$(BAUD)

clean:
	rm -rf $(OUT_DIR)

.PHONY: all compile upload flash monitor clean
```

---

## 10. Diagnóstico de Estándar C++ Activo

Para verificar en tiempo de ejecución qué estándar está aplicando activamente el compilador en cualquier tarjeta, carga este sketch:

```cpp
void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.print("Valor macro __cplusplus: ");
  Serial.println(__cplusplus);

  #if __cplusplus >= 202302L
    Serial.println("Estándar activo: C++23");
  #elif __cplusplus >= 202002L
    Serial.println("Estándar activo: C++20");
  #elif __cplusplus >= 201703L
    Serial.println("Estándar activo: C++17");
  #elif __cplusplus >= 201402L
    Serial.println("Estándar activo: C++14");
  #elif __cplusplus >= 201103L
    Serial.println("Estándar activo: C++11");
  #else
    Serial.println("Estándar activo: C++98 / C++03");
  #endif
}

void loop() {}
````

## FQBN

FQBN significa Fully Qualified Board Name (Nombre de Placa Totalmente Calificado).

Es el identificador único y estandarizado que utiliza el ecosistema de Arduino (arduino-cli y Arduino IDE 2.x) para saber con qué arquitectura, microcontrolador, banderas de compilación y variantes físicas debe compilar el código.

1. Estructura base de un FQBNLa sintaxis obligatoria se compone de tres campos separados por dos puntos:

$$\text{vendor} : \text{architecture} : \text{board\_id}$$

- Vendor (fabricante / mantenedor): La entidad o comunidad que mantiene el soporte (por ejemplo, arduino, esp32, rp2040).
- Architecture (familia de procesador): La arquitectura de CPU del paquete (por ejemplo, avr, mbed_nano, megaavr).
- Board ID (identificador de la placa): El modelo específico registrado en el archivo boards.txt del núcleo.


Ejemplos comunes:
- arduino:avr:uno $\rightarrow$ Arduino Uno clásico (ATmega328P).
- arduino:avr:nano $\rightarrow$ Arduino Nano clásico.
- arduino:megaavr:nona4809 $\rightarrow$  Arduino Nano Every (ATmega4809).
- arduino:mbed_nano:nano33ble $\rightarrow$ Arduino Nano 33 BLE / BLE Sense (nRF52840).
- esp32:esp32:esp32 $\rightarrow$ ESP32 Dev Module genérico.
- rp2040:rp2040:rpipico $\rightarrow$ Raspberry Pi Pico con el core de Earle Philhower.

2. Parámetros de menú y variantes de configuraciónMuchas placas físicas tienen subvariantes de procesador, frecuencias de reloj, esquemas de partición o tipos de flash. El FQBN permite configurar estas opciones anexándolas con dos puntos ``:`` y pares ``clave=valor`` separados por comas:$$\text{vendor} : \text{architecture} : \text{board\_id} : \mathbf{opcion1=valor1,opcion2=valor2}$$
