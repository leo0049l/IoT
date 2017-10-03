#include <SoftwareSerial.h>
#include <coap.h>
#include <WiFiUdp.h>
#define _baudrate   115200
#define _rxpin      3
#define _txpin      2
SoftwareSerial debug( _rxpin, _txpin ); // RX, TX
//*-- IoT Information
#define SSID "YI"//"葉家瑋的 MacBook Pro"//"D-Link_DIR-612"
#define PASS "55555555"//"0912414832"//"70316062"
#define IP "192.168.43.250" //"140.113.66.136"
WiFiUDP Udp;
Coap coap(Udp);
int inputPin = 10;//button
int buttonstate=0;
void setup() {
    Serial.begin( 9600 );
    debug.begin( _baudrate );
    sendDebug("AT");
    Loding("sent AT");
    connectWiFi();


    // LED State
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  pinMode(inputPin, INPUT);     // declare pushbutton as input

    
}
void loop() {
    delay(1000);   // 60 second
   
   int  val = analogRead(1);
float mv = ( val/1024.0)*5000; 
float cel = mv/10;
float farh = (cel*9)/5 + 32;
   int threshold = 0;
  int val2 = digitalRead(inputPin);  // read input value

  if (val2 == HIGH) {            // check if the input is HIGH
   buttonstate=1;
  } else {
    buttonstate=0;
  }
   if(buttonstate == 0)
  {
     if(cel>=30)
      {
        threshold = 1;
        digitalWrite(9, HIGH);
        delay(1000);
        digitalWrite(9, LOW);
        }
      SentOnCloud( String(cel), String(threshold) );
     
      Serial.print("TEMPRATURE = ");
      Serial.print(cel);
      Serial.print("*C");
      Serial.println();
      delay(1000);
  }
   else 
   {
     if(cel>=30)
      {
        threshold = 1;
        digitalWrite(9, HIGH);
        delay(1000);
        digitalWrite(9, LOW);
        }
      SentOnCloud( String(farh), String(threshold) );
     
      Serial.print("TEMPRATURE = ");
      Serial.print(farh);
      Serial.print("*F");
      Serial.println();
      delay(1000);
  }
}
boolean connectWiFi()
{
    debug.println("AT+CWMODE=1");
    Wifi_connect();
}

void SentOnCloud( String T, String H )
{
    // 設定 ESP8266 作為 Client 端
    String cmd = "AT+CIPSTART=\"UDP\",\"";
    cmd += IP;
    cmd += "\",5683";
    sendDebug(cmd);
    if( debug.find( "Error" ) )
    {
        return;
    }
    if(buttonstate == 0)
    {   
      cmd = T + "*C" + H ;// + "&field1=" + T + "&field2=" + H +"\r\n";
    }
    else
    {
       cmd = T + "*F"+ H ;//
    }
    debug.print( "AT+CIPSEND=" );
    debug.println( cmd.length() );
    if(debug.find( ">" ) )
    {
        Serial.print(">");
        Serial.print(cmd);
        debug.print(cmd);
    }
    else
    {
      
        debug.print( "AT+CIPCLOSE" );
    }
}


void Wifi_connect()
{
    String cmd="AT+CWJAP=\"";
    cmd+=SSID;
    cmd+="\",\"";
    cmd+=PASS;
    cmd+="\"";
    sendDebug(cmd);
    Loding("Wifi_connect");
}
void Loding(String state){
    for (int timeout=0 ; timeout<10 ; timeout++)
    {
      if(debug.find("OK"))
      {
          Serial.println("RECEIVED: OK");
          break;
      }
      else if(timeout==9){
        Serial.print( state );
        Serial.println(" fail...\nExit2");
      }
      else
      {
        Serial.print("Wifi Loading...");
        delay(500);
      }
    }
}
void sendDebug(String cmd)
{
    Serial.print("SEND: ");
    Serial.println(cmd);
    debug.println(cmd);
}

