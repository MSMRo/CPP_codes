# 🛠️ Microcontroller API Cheat Sheet: ESP32, Raspberry Pi Pico & Arduino Nano 33 BLE Sense

Guía condensada de referencia técnica y sintaxis de la API de Arduino para tres de las arquitecturas de 32 bits más populares: **ESP32** (Tensilica Xtensa), **Raspberry Pi Pico** (RP2040 ARM Cortex-M0+) y **Arduino Nano 33 BLE Sense Rev 1** (Nordic nRF52840 ARM Cortex-M4F).

---

## 📌 Diagramas de Pinout (Imágenes Públicas)

### 1. ESP32 DevKit V1 (30 / 36 pines)
![ESP32 Pinout](https://esp32.implrust.com/esp32-intro/images/ESP32-DevKit-V1-Pinout-Diagram.png)
*(Esquema alternativo público: `https://www.mischianti.org/wp-content/uploads/2020/11/ESP32-DOIT-DEV-KIT-v1-pinout-mischianti.png`)*

### 2. Raspberry Pi Pico (RP2040)
![Raspberry Pi Pico Pinout](https://www.raspberrypi.com/documentation/microcontrollers/images/pico-pinout.svg)

### 3. Arduino Nano 33 BLE / BLE Sense
![Arduino Nano 33 BLE Sense Pinout](https://www.cnx-software.com/wp-content/uploads/2023/02/Arduino-Nano-33-BLE-Sense-Rev2-Pinout-Diagram.png)

---

## 📊 2. Tabla Comparativa de Capacidades de Hardware

| Característica | ESP32 (WROOM-32) | Raspberry Pi Pico (RP2040) | Nano 33 BLE Sense (nRF52840) |
| :--- | :--- | :--- | :--- |
| **Voltaje Lógico I/O** | 3.3 V (No tolerante a 5 V) | 3.3 V (No tolerante a 5 V) | 3.3 V (No tolerante a 5 V) |
| **Frecuencia CPU Base** | 240 MHz | 133 MHz | 64 MHz |
| **Resolución ADC** | 12 bits ($0 - 4095$) | 12 bits ($0 - 4095$) | Hasta 12 bits SAADC ($0 - 4095$) |
| **DAC de Hardware** | Sí (2 canales, 8 bits en GPIO 25/26) | No | No |
| **Módulos Wi-Fi** | Wi-Fi 802.11 b/g/n nativo | No nativo (solo versión Pico W) | No integrado |
| **Bluetooth** | BLE 4.2 / BR/EDR | No (solo versión Pico W) | Bluetooth 5.0 LE nativo |
| **Modelo de Timers** | Hardware Timers (TimerGroup) | Alarmas de hardware / SDK Alarms | Mbed OS `Ticker` / Nordic `nrfx` |

---

## 💻 3. Sintaxis Completa de Periféricos y Retornos

### 3.1. GPIO Digital

```cpp
// Prototipos de funciones
void pinMode(pin_size_t pin, PinMode mode);
// mode: INPUT, OUTPUT, INPUT_PULLUP, INPUT_PULLDOWN (en ESP32 y Pico)

void digitalWrite(pin_size_t pin, PinStatus val);
// val: HIGH (1) o LOW (0)

PinStatus digitalRead(pin_size_t pin);
// Retorna: PinStatus (HIGH o LOW)
```

---

### 3.2. Entrada Analógica (ADC)

```cpp
int analogRead(pin_size_t pin);
// Retorna: int (0 a 1023 por defecto; 0 a 4095 si se configura a 12 bits)

void analogReadResolution(int bits);
// Configura la resolución del convertidor (ej. 10 o 12 bits)
```

#### Uso por Placa:
* **ESP32:**
  ```cpp
  analogReadResolution(12);       // Escala 0 a 4095
  analogSetAttenuation(ADC_11db); // Rango de lectura hasta ~3.1 V
  int raw = analogRead(34);       // Usar pines ADC1 (GPIO 32 a 39) si el Wi-Fi está encendido
  ```
* **RP2040 (Raspberry Pi Pico):**
  ```cpp
  analogReadResolution(12);
  int raw = analogRead(26);       // Canales disponibles: GPIO 26, 27, 28 y sensor interno (pin 4)
  ```
* **Nano 33 BLE Sense:**
  ```cpp
  analogReadResolution(12);
  int raw = analogRead(A0);
  float voltage = raw * (3.3f / 4095.0f);
  ```

---

### 3.3. Salida Analógica: PWM y DAC

#### Modulación por Ancho de Pulsos (PWM)

* **Raspberry Pi Pico y Nano 33 BLE Sense:**
  ```cpp
  void analogWriteResolution(int bits); // Configura resolución (ej. 8 a 16 bits)
  void analogWrite(pin_size_t pin, int value);
  // Ejemplo:
  analogWrite(LED_BUILTIN, 128); // 50% de ciclo de trabajo
  ```

* **ESP32 (Core v3.x API):**
  ```cpp
  bool ledcAttach(uint8_t pin, uint32_t freq, uint8_t resolution);
  // Retorna: bool (true si se asignó un canal libre exitosamente)
  bool ledcWrite(uint8_t pin, uint32_t duty);
  // Retorna: bool (true si el valor fue escrito)

  // Ejemplo:
  ledcAttach(18, 5000, 8); // Pin 18, 5 kHz, resolución de 8 bits (0-255)
  ledcWrite(18, 128);      // 50% ciclo de trabajo
  ```

#### DAC de Hardware (Solo ESP32)
Entrega voltajes continuos reales en `GPIO 25` (Canal 1) y `GPIO 26` (Canal 2):
```cpp
void dacWrite(uint8_t pin, uint8_t value);
// pin: 25 o 26
// value: 0 a 255 (0 V a 3.3 V reales)
dacWrite(25, 128); // Genera ~1.65 V reales
```

---

### 3.4. Interrupciones Externas (GPIO)

```cpp
void attachInterrupt(pin_size_t interruptNum, void (*userFunc)(void), InterruptMode mode);
// interruptNum: digitalPinToInterrupt(pin)
// mode: RISING, FALLING, CHANGE, LOW
void detachInterrupt(pin_size_t interruptNum);
```

#### Código de Implementación Universal:
```cpp
volatile bool eventTriggered = false;

#if defined(ESP32)
void IRAM_ATTR gpioISR() {
#else
void gpioISR() {
#endif
  eventTriggered = true;
}

void setup() {
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), gpioISR, FALLING);
}
```

---

### 3.5. Temporizadores Internos y Hardware Timers

#### Arduino Nano 33 BLE Sense (`mbed::Ticker`)
```cpp
#include <mbed.h>

mbed::Ticker timerTicker;
volatile bool tickFlag = false;

void isrTimer() {
  tickFlag = true;
}

void setup() {
  // Configuración en microsegundos (.attach_us) o segundos (.attach)
  timerTicker.attach_us(&isrTimer, 100000); // Dispara cada 100 ms
}
```

#### ESP32 (Hardware Timer v3.x)
```cpp
hw_timer_t *timer = NULL;
volatile bool timerFlag = false;

void IRAM_ATTR onTimer() {
  timerFlag = true;
}

void setup() {
  timer = timerBegin(1000000);          // Base de tiempo a 1 MHz (1 tick = 1 µs)
  timerAttachInterrupt(timer, &onTimer); 
  timerAlarm(timer, 500000, true, 0);   // Alarma cada 500 ms con autorrecarga
}
```

#### Raspberry Pi Pico (SDK Repeating Timer)
```cpp
struct repeating_timer timer;
volatile bool timerFlag = false;

bool timer_callback(struct repeating_timer *t) {
  timerFlag = true;
  return true; // Retornar true para reiniciar el ciclo
}

void setup() {
  // Intervalo negativo para temporización precisa de borde a borde
  add_repeating_timer_ms(-500, timer_callback, NULL, &timer);
}
```

---

### 3.6. Puerto Serie (UART)

```cpp
void Serial.begin(unsigned long baud);
int Serial.available(void);           // Retorna: int (número de bytes disponibles)
int Serial.read(void);                // Retorna: int (byte leído o -1 si el búfer está vacío)
size_t Serial.write(uint8_t val);     // Retorna: size_t (cantidad de bytes transmitidos)
size_t Serial.print(const String &s); // Retorna: size_t (caracteres transmitidos)
size_t Serial.println(...);           // Retorna: size_t
```

#### Puertos UART Secundarios:
* **ESP32:** Reasignación flexible de pines:
  ```cpp
  HardwareSerial SerialPort(1); // Instancia UART1
  SerialPort.begin(115200, SERIAL_8N1, /*RX=*/16, /*TX=*/17);
  ```
* **RP2040:** `Serial1` utiliza por defecto `GP0` (TX) y `GP1` (RX). `Serial2` utiliza `GP4` (TX) y `GP5` (RX).
* **Nano 33 BLE Sense:** `Serial` conecta al USB nativo (CDC). `Serial1` mapea a los pines físicos `D1` (TX) y `D0` (RX).

---

### 3.7. Bus I2C (`Wire.h`)

```cpp
#include <Wire.h>

void Wire.begin();
void Wire.setClock(uint32_t frequency); // Ej: 100000 (Standard), 400000 (Fast Mode)

void Wire.beginTransmission(uint8_t address);
size_t Wire.write(uint8_t data);        // Retorna: size_t (bytes almacenados en búfer)
uint8_t Wire.endTransmission(bool sendStop = true);
// Retorna uint8_t:
// 0: Éxito (ACK recibido)
// 1: Búfer de transmisión desbordado
// 2: NACK en transmisión de dirección
// 3: NACK en transmisión de datos
// 4: Error en bus de datos

uint8_t Wire.requestFrom(uint8_t address, uint8_t quantity); 
// Retorna uint8_t: Cantidad de bytes devueltos por el periférico esclavo
int Wire.read(); // Retorna: int (-1 o el byte recibido)
```

---

### 3.8. Bus SPI (`SPI.h`)

```cpp
#include <SPI.h>

void SPI.begin();
void SPI.beginTransaction(SPISettings(uint32_t clock, BitOrder bitOrder, DataMode dataMode));
// bitOrder: MSBFIRST, LSBFIRST
// dataMode: SPI_MODE0, SPI_MODE1, SPI_MODE2, SPI_MODE3

uint8_t SPI.transfer(uint8_t data);        // Transfiere y lee 1 byte. Retorna: uint8_t
void SPI.transfer(void *buf, size_t count);// Transfiere bloques de memoria continuos
void SPI.endTransaction();
```

---

### 3.9. Wi-Fi (Exclusivo ESP32)

```cpp
#include <WiFi.h>

const char* ssid = "NOMBRE_RED";
const char* pass = "CONTRASENA";

void setup() {
  Serial.begin(115200);
  WiFi.mode(WIFI_STA); // Modo cliente (Station)
  WiFi.begin(ssid, pass);

  // Verificación de estado de conexión
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  // Métodos de consulta de red
  IPAddress ip = WiFi.localIP();      // Retorna: objeto IPAddress
  int8_t rssi  = WiFi.RSSI();         // Retorna: int8_t (potencia de señal en dBm)
  String mac   = WiFi.macAddress();   // Retorna: String
}
```

---

## 📦 4. Tipos de Archivos y Entregables de Compilación

| Extensión | Formato / Rol | Placas Asociadas |
| :--- | :--- | :--- |
| **`.ino`** | Código fuente en sintaxis de Arduino (preprocesado a C++). | Todas |
| **`.elf`** | *Executable and Linkable Format*. Binario con cabeceras completas y tablas de símbolos de depuración. | Salida intermedia común de GCC |
| **`.bin`** | Volcado binario crudo del ejecutable para memoria Flash. | **ESP32** (vía `esptool.py`), **Nano 33 BLE** |
| **`.uf2`** | *USB Flashing Format*. Formato de 512 bytes por bloque diseñado para programación mediante emulación USB Mass Storage. | **Raspberry Pi Pico** (arrastrar directamente al volumen `RPI-RP2`) |
| **`.hex`** | Formato de texto Intel HEX con direcciones y datos en codificación ASCII. | **Nano 33 BLE** / Plataformas AVR clásicas |