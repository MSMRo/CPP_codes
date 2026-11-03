/*
    $ arduino-cli compile --fqbn arduino:mbed:nano33ble -e segment01/segment01.ino   
    $ arduino-cli upload -p COM4 --fqbn arduino:mbed:nano33ble segment01/segment01.ino
*/

#include <Arduino.h>
#include <mbed.h>

mbed::Ticker timerTicker;

volatile bool toggleLed = false;

void onTimerTick(){
    toggleLed = true;
}

/*********************************/

void setup()
{
    pinMode(LED_BUILTIN,OUTPUT);

    timerTicker.attach_us(&onTimerTick, 5000000); //5s
}

void loop()
{
    if(toggleLed){
        toggleLed = false;
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
        
    }
}