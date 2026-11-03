# Tutorial 08 (C++): Clases, Encapsulamiento y Abstracción de Costo Cero

## 🎯 Objetivo de Aprendizaje
Aprender a estructurar código de microcontroladores utilizando clases en C++ sin pagar ninguna penalización de rendimiento ni de memoria RAM en comparación con el C clásico. Comprender la optimización `inline` y el uso de métodos `const`.

---

## 🔬 El Mito del Costo de las Clases en C++

Existe la creencia errónea de que programar en C++ hace el binario más pesado y lento que en C.
En este ejercicio demostramos matemáticamente:
```cpp
struct LedDatosC {
    uint8_t pin;   // 1 byte
    bool estado;   // 1 byte
}; // sizeof = 2 bytes

class ControladorLed {
private:
    uint8_t m_pin; // 1 byte
    bool m_estado; // 1 byte
public:
    void encender();
    void apagar();
}; // sizeof = ¡Exactamente 2 bytes!
```

> [!NOTE]
> En C++, una clase es internamente idéntica a una estructura. Las funciones miembro **no ocupan espacio dentro del objeto en la memoria SRAM**. El compilador simplemente transforma `miLed.encender()` en una llamada normal pasando la dirección del objeto como el primer argumento oculto (`this`).

### Métodos `inline`
Al definir las funciones dentro del cuerpo de la clase (o marcar con `inline`), el compilador no genera instrucciones de salto de función (`CALL`/`RET` o `BL`/`BX`). En su lugar, inserta directamente las instrucciones ensamblador en el punto de uso, logrando la misma velocidad que escribir registros a mano.

---

## 📋 Configuración y Ejecución

* **Plataformas**: Arduino Uno, Nano, Mega, ESP32, STM32, RP2040.
* **Velocidad Serial**: 115200 baudios.
* **PlatformIO**: Abrir la carpeta `01_Clases_y_Encapsulamiento`, compilar y subir.
* **Arduino IDE**: Abrir `01_Clases_y_Encapsulamiento.ino`.
