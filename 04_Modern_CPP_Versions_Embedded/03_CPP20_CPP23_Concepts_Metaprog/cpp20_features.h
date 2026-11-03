#ifndef CPP20_FEATURES_H
#define CPP20_FEATURES_H

#include <Arduino.h>
#include <stdint.h>

// ============================================================================
// 1. C++20: CONSTEVAL
// ============================================================================
#if defined(__cpp_consteval) && __cpp_consteval >= 201811L
consteval uint32_t calcularFrecuenciaTimer(uint32_t f_cpu, uint16_t prescaler) {
    return f_cpu / prescaler;
}
#else
constexpr uint32_t calcularFrecuenciaTimer(uint32_t f_cpu, uint16_t prescaler) {
    return f_cpu / prescaler;
}
#endif

// ============================================================================
// 2. C++20: CONCEPTS (Conceptos de Hardware)
// ============================================================================
class LedPinC20 {
private:
    uint8_t m_pin;
public:
    LedPinC20(uint8_t p) : m_pin(p) {}
    void begin() { pinMode(m_pin, OUTPUT); }
    void high()  { digitalWrite(m_pin, HIGH); }
    void low()   { digitalWrite(m_pin, LOW); }
};

#if defined(__cpp_concepts) && __cpp_concepts >= 201907L
// Concepto en C++20
template <typename T>
concept PinSalidaDigital = requires(T pin) {
    { pin.high() } -> std::same_as<void>;
    { pin.low() }  -> std::same_as<void>;
};

template <PinSalidaDigital TPin>
void parpadearRapido(TPin& pinSalida) {
    pinSalida.high();
    delay(100);
    pinSalida.low();
    delay(100);
}
#else
// Fallback para C++11 / C++17
template <typename TPin>
void parpadearRapido(TPin& pinSalida) {
    pinSalida.high();
    delay(100);
    pinSalida.low();
    delay(100);
}
#endif

// ============================================================================
// 3. COMPARACIÓN DE ESTRUCTURAS (Spaceship C++20 vs Fallback)
// ============================================================================
struct PaqueteMuestreo {
    uint16_t idNodo;
    uint16_t lecturaAdc;

#if defined(__cpp_impl_three_way_comparison) && __cpp_impl_three_way_comparison >= 201907L
    auto operator<=>(const PaqueteMuestreo&) const = default;
#else
    bool operator==(const PaqueteMuestreo& otro) const {
        return (idNodo == otro.idNodo) && (lecturaAdc == otro.lecturaAdc);
    }
#endif
};

#endif // CPP20_FEATURES_H
