#define DEBUG true

#include <Arduino.h>
#include <SoftwareSerial.h>
#define _ESP_RXPIN_ 2
#define _ESP_TXPIN_ 3
#define BAUDRATE    9600
extern SoftwareSerial esp(_ESP_RXPIN_, _ESP_TXPIN_);

void setup() {
  Serial.begin(BAUDRATE);
  Serial.println("DEBUGGER");
  esp.begin(BAUDRATE);

  pinMode(LED_BUILTIN, OUTPUT);

  sendData("AT+RST\r\n",2000,DEBUG);
  sendData("AT+CWMODE=2\r\n",1000,DEBUG);
  sendData("AT+CIFSR\r\n",1000,DEBUG);
  sendData("AT+CIPMUX=1\r\n",1000,DEBUG);
  sendData("AT+CIPSERVER=1,80\r\n",1000,DEBUG);
}

int connectionId;
void loop() {
  if( esp.available() ){
    if( esp.find("+IPD,") ){
      delay(400);
      connectionId = esp.read() - 48;
      if( esp.find("pin=") ){
        Serial.println("recieving data from web browser");
        int state = (esp.read()-48); 
        Serial.println( state );
        digitalWrite(LED_BUILTIN, state);
      }
      String webpage = "<a href=\"/?pin=1\">ON</a><br/><a href=\"/?pin=0\">OFF</a>";
      espsend(webpage);

      String closeCommand = "AT+CIPCLOSE=";
      closeCommand+=connectionId;
      closeCommand+="\r\n";
      sendData(closeCommand,3000,DEBUG);
    }
  }
}

 void espsend(String d)
 {
   String cipSend = "AT+CIPSEND=";
   cipSend += connectionId; 
   cipSend += ",";
   cipSend +=d.length();
   cipSend +="\r\n";
   sendData(cipSend,1000,DEBUG);
   sendData(d,1000,DEBUG); 
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
