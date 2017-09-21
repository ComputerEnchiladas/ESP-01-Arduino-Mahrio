#ifndef __ESP8266_H__
#define __ESP8266_H__

#include <Arduino.h>
#include <SoftwareSerial.h>
#define _ESP_RXPIN_ 2
#define _ESP_TXPIN_ 3
#define BAUDRATE    115200
String SSID = "<wifi-router-name>";
String PASSWORD = "<wifi-router-password>";
String MODE = "1";
String SERVER = "192.168.0.5";
String PORT = "6085";
String PATH = "/api/monitor";
int count = 0;
boolean isReady = false;
extern SoftwareSerial esp(_ESP_RXPIN_, _ESP_TXPIN_);

void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("DEBUGGER");
  esp.begin(BAUDRATE);
  esp.flush();
  esp.println("AT+RST");
  esp.flush();
  
  ESP_Mode();
  ESP_AccessPoint();
  delay(3000);
  ESP_ObtainIP();
  isReady = true;
}

void loop() {
  String http;
  String json = "[{\"key\": \"" + MODE + "\"}]";
  http = "POST " + PATH + " HTTP/1.1\r\n";
  http += "Host:"+SERVER+"\r\n";
  http += "Content-Length:" + String(json.length()) + "\r\n";
  http += "Connection: close\r\n\r\n";
  http += json;
  
  if( isReady ) {
    ESP_Send( http );
  }
  delay(1000);
}

void ESP_Mode( ) {
  esp.print("AT+CWMODE=");
  esp.println( MODE);
  esp.flush();
  delay(500);
}
void ESP_AccessPoint( ){
  esp.print("AT+CWJAP=");
  esp.print("\"");
  esp.print(SSID);
  esp.print("\"");
  esp.print(",");
  esp.print("\"");
  esp.print(PASSWORD);
  esp.println("\"");
  esp.flush();
  delay(1000);
}
void ESP_ObtainIP( ){
  esp.flush();
  esp.println("AT+CIFSR");
}
void ESP_Send( String val ){
  Serial.println(String(count++) + ": Sending data...");
  esp.flush();
  esp.print("AT+CIPSTART=");
  esp.print("\"TCP\"");
  esp.print(",");
  esp.print("\"");
  esp.print(SERVER);
  esp.print("\"");
  esp.print(",");
  esp.println(PORT);
  delay(400);
  esp.flush();
  esp.print("AT+CIPSEND=");
  esp.println(val.length());
  delay(100);
  esp.print(val);
  esp.flush();
}
#endif
