/*
    $ arduino-cli compile --fqbn arduino:mbed:nano33ble -e segment01/segment01.ino   
    $ arduino-cli upload -p COM4 --fqbn arduino:mbed:nano33ble segment01/segment01.ino
*/

#include <Arduino.h>
#include <mbed.h>

mbed::Ticker timerTicker;

float fs = 1000; //Hz
float Ts = 1/fs; //s
float Ts_us = Ts*1e6; //us

volatile bool finish_timer = false;
volatile int adcValue = 0;
volatile int buff_emg[100];

void onTimerTick(){
    adcValue = analogRead(A6); //dummy read to wake up the ADC
    finish_timer = true;
}

/*********************************/

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);

    analogResolution(12); //12 bits
    timerTicker.attach_us(&onTimerTick, Ts_us); //5s
    
}

void loop()
{
    if(finish_timer){
        finish_timer = false;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        
    }
}