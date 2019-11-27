#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"
#include <WiFiMulti.h>
WiFiMulti WiFiMulti;
WiFiServer wifiServer(4501); 

SSD1306  display(0x3c, 4, 15);
 
#define SS      18
#define RST     14
#define DI0     26
#define BAND    433E6
 
void setup()
{
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);    
  delay(50); 
  digitalWrite(16, HIGH);
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
   
   Serial.begin(115200);
   WiFiMulti.addAP("sebil", "12345678");  //Alihan iPhone'u  alihan07

   Serial.println();
   Serial.println();
   Serial.print("wifi bekleniyor... ");

   while(WiFiMulti.run() != WL_CONNECTED)
   {
      Serial.print(".");
      delay(500);
   }

    Serial.println("");
    Serial.println("WiFi baglandi!");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(500);

    while (!Serial); 
    delay(1000);
    Serial.println("...LoRa Alici..."); 
    display.drawString(5,5,"...LoRa Alici..."); 
    display.display();
    SPI.begin(5,19,27,18);
    LoRa.setPins(SS,RST,DI0);
   
  if (!LoRa.begin(BAND))
  {
    display.drawString(5,25,"Starting LoRa failed!");
    while (1);
  }
    LoRa.setSpreadingFactor(12);
    LoRa.setSignalBandwidth(62.5E3);
    LoRa.crc();

   Serial.println("LoRa eslesmesi tamamlandi!");
   display.drawString(5,25,"LoRa eslesmesi tamamlandi!");
   display.display();
   delay(10);
}
void loop() 
{
    const uint16_t port = *****;
    const char * host = "***.***.*.***";   // ip or dns

    Serial.print("Bu aga baglaniliyor...");
    Serial.println(host);
    
    WiFiClient client = wifiServer.available();
   if (!client.connect(host, port))
   {
        Serial.println("Sunucu baglantisi koptu!");
        Serial.println("5 sn bekleyin...");
        delay(5000);
        return;
    }

  while (client.connected())
 {
  int packetSize = LoRa.parsePacket();
  if (packetSize)
 {
    
    Serial.print("Veri aliniyor...");
    display.clear();
    display.setFont(ArialMT_Plain_16);
    display.drawString(3, 0, "Veri Aliniyor... ");
    display.display();
    
  while (LoRa.available()+client.available())
      {
      String data = LoRa.readString(); 
      Serial.println(data);
      client.print(data);
       }
   }
   }
}
