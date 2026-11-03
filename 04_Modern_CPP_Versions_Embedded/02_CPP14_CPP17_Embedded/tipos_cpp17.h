#ifndef TIPOS_CPP17_H
#define TIPOS_CPP17_H

#include <stdint.h>

// Estructura para Structured Bindings
struct DatosClima {
    float temperatura;
    float humedad;
    uint16_t presionHPa;
};

// Enum para [[nodiscard]]
enum class CodigoRetorno : uint8_t {
    EXITO = 0,
    ERROR_HARDWARE,
    ERROR_TIMEOUT
};

// Clase con inline static variable (C++17)
class ConfiguracionSistema {
public:
    inline static const uint16_t TIMEOUT_MAXIMO_MS = 5000;
};

#endif // TIPOS_CPP17_H
