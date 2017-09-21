# Collecting data with Arduino and transmitting to Mahrio server

## Hardware
* Wifi Router
* Laptop
* Arduino Uno
* ESP-01 (ESP8266)

## Summary
This repo shows the minimum amount of code to run on Arduino connected to ESP-01. 
The Arduino runs an infifite loop which sends a json payload to Mahrio server. 
The Mahrio server simply ouputs the data to the console.

## How-To
1. Connect the Arduino and ESP-01 according to the diagram in this repo.
2. Use Shell application and locate server folder. Run `npm install`, then `node index`
3. Connect Arduino to computer, use Arduino IDE to open .ino file, then load program into board.
4. Open Serial Monitor (Tools > Serial Monitor) and set baud to 115200
