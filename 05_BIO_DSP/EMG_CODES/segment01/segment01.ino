// arduino-cli compile --upload -p COM5 --fqbn arduino:mbed_nano:nano33ble segment01/segment01.ino

#include <Arduino.h>
#include <mbed.h>

mbed::Ticker timerTicker;

const float fs = 1000.0f;               // 1000 Hz
const float Ts_us = (1.0f / fs) * 1e6f; // 1000 us

const int WINDOW_TIME = 2;              // 2 segundos
const int N_SAMPLES = WINDOW_TIME * (int)fs; // 2000 muestras

volatile bool sampleTick = false;
volatile bool finish_buffer = false;
int buff_emg[N_SAMPLES];
int sampleIndex = 0;

// ISR ultra ligera: NO llama a analogRead ni funciones bloqueantes
void onTimerTick() {
  sampleTick = true;
}

int mean(int *data, int n);

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);

  analogReadResolution(12);

  // Iniciar timer periódico
  timerTicker.attach_us(&onTimerTick, Ts_us);
}

void loop() {
  // 1. Tomar muestra cuando el timer lo indique (fuera de la interrupción)
  if (sampleTick) {
    sampleTick = false;

    if (!finish_buffer) {
      buff_emg[sampleIndex] = analogRead(A6);
      sampleIndex++;

      if (sampleIndex >= N_SAMPLES) {
        sampleIndex = 0;
        finish_buffer = true;
      }
    }
  }

  // 2. Procesar la ventana de datos
  if (finish_buffer) {
    float emg_mean = (float)mean(buff_emg, N_SAMPLES);

    Serial.print("EMG mean: ");
    Serial.println(emg_mean);

    // Reiniciar para la siguiente ventana
    finish_buffer = false;
  }
}

int mean(int *data, int n) {
  long sum = 0;
  for (int i = 0; i < n; i++) {
    sum += data[i];
  }
  return (int)(sum / n);
}