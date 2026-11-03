# Tutorial 03: Manipulación de Bits y Registros de Hardware

## 🎯 Objetivo de Aprendizaje
Dominar las operaciones a nivel de bit y el acceso directo a los registros de entrada y salida (GPIO) del microcontrolador. Entender cómo una sola instrucción ensamblador (`SBI`/`CBI`) de 1 ciclo de reloj (62.5 nanosegundos a 16 MHz) reemplaza las más de 50 instrucciones que ejecuta `digitalWrite()`.

---

## 🔬 Los 4 Modismos de Bits Esenciales en Sistemas Embebidos

En firmware profesional, nunca se "adivina" el valor de un registro completo; se modifican únicamente los bits específicos que controlan el periférico deseado sin alterar los demás.

| Operación | Modismo en C | Explicación |
| :--- | :--- | :--- |
| **Poner a 1 (SET)** | `REG \|= (1 << BIT);` | Usa OR binario. Convierte el bit en 1 sin tocar los demás. |
| **Poner a 0 (CLEAR)** | `REG &= ~(1 << BIT);` | Usa AND con el inverso (NOT `~`). Fuerza el bit a 0. |
| **Conmutar (TOGGLE)** | `REG ^= (1 << BIT);` | Usa XOR binario. Si estaba en 0 pasa a 1; si estaba en 1 pasa a 0. |
| **Comprobar (TEST)** | `if (REG & (1 << BIT))` | Aplica una máscara AND. Es distinto de 0 si el bit está en 1. |

---

## ⚡ Rendimiento: Registros Directos vs Capa Arduino

### En Arduino Uno (ATmega328P):
Para conmutar el Pin 13 (PB5):
```c
// Forma Arduino estándar:
digitalWrite(13, HIGH);
// Costo: ~55 ciclos de reloj (~3.5 microsegundos)
// Motivo: Busca en tablas en Flash qué puerto y qué bit corresponden al pin 13,
// desactiva temporizadores PWM asociados y realiza validaciones.

// Forma Embebida Directa:
PORTB |= (1 << PORTB5);
// Costo: 1 ciclo de reloj (62.5 nanosegundos)
// ¡Es más de 50 veces más rápido y consume mucho menos espacio en Flash!

// Alternativa AVR ultra-rápida (Hardware Toggle):
PINB = (1 << PINB5); // Escribir un 1 en el registro PIN invierte el pin automáticamente por hardware
```

---

## 📋 Configuración y Ejecución

* **Placa Óptima**: Arduino Uno / Nano (ATmega328P) para observar la manipulación de registros `DDRB`, `PORTB` y `PINB`.
* **Compatibilidad 32 bits**: También compatible con ESP32 / RP2040 / STM32 gracias a guardas condicionales (`#if defined(__AVR_...)`).
* **PlatformIO**: Abrir la carpeta `03_Manipulacion_Bits_Registros` y seleccionar `env:uno` o `env:esp32`.
* **Arduino IDE**: Abrir `03_Manipulacion_Bits_Registros.ino`.
