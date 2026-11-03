/*
  APDS9960 Proximity Sensor Example
  This example shows how to read the proximity value from the APDS9960 sensor.
  The proximity value is printed to the Serial Monitor.
  Author: Moises Meza

    Board: Arduino Nano 33 BLE
    Library: Arduino_APDS9960
    
  $ arduino-cli: arduino-cli compile --fqbn arduino:mbed_nano:nano33ble -e  exam01.ino
  $ arduino-cli upload -p COM4 --fqbn arduino:mbed_nano:nano33ble  exam01.ino
*/


#include <Arduino.h>
#include <Arduino_APDS9960.h>

void setup(){
    Serial.begin(9600);
    while(!Serial);

    if(!APDS.begin()){
        Serial.println("Error al inicializar el sensor APDS9960");
    }else{
        Serial.println("Sensor ok");
    }
}

void loop(){
    if(APDS.proximityAvailable()){
        int prox = APDS.readProximity();

        Serial.println(prox);
    }

    delay(100);  //Ts=100ms, Fs=10Hz, Fnyquist=5Hz
}