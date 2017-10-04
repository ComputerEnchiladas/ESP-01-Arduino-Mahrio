
#include <Arduino.h>
#include <SoftwareSerial.h>
#define _ESP_RXPIN_ 2
#define _ESP_TXPIN_ 3
#define BAUDRATE    9600
String MODE = "1";
String SSID = "<wifi-router-name>";
String PASSWORD = "<wifi-router-password>";
String SERVER = "192.168.X.XXX";
String PORT = "6085";
String PATH = "/api/monitor";
int count = 0;
boolean isReady = false;
extern SoftwareSerial esp(_ESP_RXPIN_, _ESP_TXPIN_);

void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("DEBUGGER");
  esp.begin(BAUDRATE);
  sendData("AT+RST\r\n",2000, 1);
  sendData("AT+CWMODE=" + MODE + "\r\n",1000, 1);
  sendData("AT+CWJAP=\"" + SSID + "\",\"" + PASSWORD + "\"\r\n", 3000, 1);
  sendData("AT+CIFSR\r\n", 5000, 1);
  isReady = true;
}

void loop() {
  String http;
  String json = "[{\"key\": \"" + MODE + "\"}]\r\n";
  http = "POST " + PATH + " HTTP/1.1\r\n";
  http += "Host:"+SERVER+"\r\n";
  http += "Content-Length:" + String(json.length()) + "\r\n";
  http += "Connection: close\r\n\r\n";
  http += json;
  
  if( isReady ) {
    Serial.println( "\n\n\n\n" + String(count++) + ": Sending data...");
    sendData( "AT+CIPSTART=\"TCP\",\"" + SERVER + "\"," + PORT + "\r\n", 1000, 1 );
    sendData( "AT+CIPSEND=" + String( http.length() ) + "\r\n", 500, 1);
    sendData( http, 1000, 1);
  }
  delay(1000);
}
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  esp.print(command);
  long int time = millis();
  while( (time+timeout) > millis())
  {
    while(esp.available())
    {
      char c = esp.read(); // read the next character.
      response+=c;
    }  
  }
  
  if(debug)
  {
    Serial.print(response); //displays the esp response messages in arduino Serial monitor
  }
  return response;
}
