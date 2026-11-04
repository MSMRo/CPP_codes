// arduino-cli compile --upload -p COM8 --fqbn arduino:mbed_nano:nano33ble segment02/segment02.ino

#include <Arduino.h>
#include <mbed.h>
#include <math.h>

// 1. Declarar el struct PRIMERO
struct EmgMetrics {
    float mean_raw;
    float mean_volts;
    float rms_volts;
};

// 2. Prototipo explícito (opcional, pero buena práctica)
EmgMetrics compute_emg_metrics(const int16_t *data, uint16_t n);

// --- Parámetros de Adquisición ---
const float FS = 1000.0f;                    // 1000 Hz
const float TS_US = (1.0f / FS) * 1.0e6f;    // 1000 us
const uint16_t WINDOW_SAMPLES = 1000;        // 1000 muestras

const float V_REF = 3.3f;
const float ADC_MAX = 4095.0f;

mbed::Ticker sampleTicker;
volatile bool tickReady = false;

void onTimerTick() {
    tickReady = true;
}

// 3. Implementación de la función usando struct EmgMetrics
EmgMetrics compute_emg_metrics(const int16_t *data, uint16_t n) {
    EmgMetrics res = {0.0f, 0.0f, 0.0f};
    if (n == 0) return res;

    int64_t sum_raw = 0;
    for (uint16_t i = 0; i < n; i++) {
        sum_raw += data[i];
    }
    float mean_raw = (float)sum_raw / (float)n;

    const float scale = V_REF / ADC_MAX;
    float sum_sq_diff = 0.0f;

    for (uint16_t i = 0; i < n; i++) {
        float ac_sample_volts = ((float)data[i] - mean_raw) * scale;
        sum_sq_diff += ac_sample_volts * ac_sample_volts;
    }

    res.mean_raw = mean_raw;
    res.mean_volts = mean_raw * scale;
    res.rms_volts = sqrtf(sum_sq_diff / (float)n);

    return res;
}

int16_t emg_buffer[WINDOW_SAMPLES];
uint16_t sample_index = 0;
bool buffer_ready = false;

void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(115200);

    analogReadResolution(12);
    sampleTicker.attach_us(&onTimerTick, TS_US);
}

void loop() {
    if (tickReady) {
        tickReady = false;

        if (!buffer_ready) {
            emg_buffer[sample_index] = (int16_t)analogRead(A6);
            sample_index++;

            if (sample_index >= WINDOW_SAMPLES) {
                sample_index = 0;
                buffer_ready = true;
            }
        }
    }

    if (buffer_ready) {
        EmgMetrics metrics = compute_emg_metrics(emg_buffer, WINDOW_SAMPLES);

        Serial.print("DC Offset: ");
        Serial.print(metrics.mean_volts, 3);
        Serial.print(" V | EMG RMS: ");
        Serial.print(metrics.rms_volts * 1000.0f, 2);
        Serial.println(" mV");

        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        buffer_ready = false;
    }
}