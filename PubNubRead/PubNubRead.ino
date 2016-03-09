/*
  PubNub sample client

  This sample client will use just the minimal-footprint raw PubNub
  interface where it is your responsibility to deal with the JSON encoding.

  It will just send a hello world message and retrieve one back, reporting
  its deeds on serial console.

  Circuit:
  * Ethernet shield attached to pins 10, 11, 12, 13
  * (Optional.) LED on pin 8 for reception indication.
  * (Optional.) LED on pin 9 for publish indication.

  created 23 October 2012
  by Petr Baudis

  https://github.com/pubnub/pubnub-api/tree/master/arduino
  This code is in the public domain.
  */

#include <SPI.h>
#include <Ethernet.h>
#include <PubNub.h>

// Some Ethernet shields have a MAC address printed on a sticker on the shield;
// fill in that address here, or choose your own at random:
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0x11 };

const int Led = 2;
int analogica = 0;

char pubkey[] = "pub-c-65914541-3bbd-4fa9-979d-ffe4b018be8f";
char subkey[] = "sub-c-12fa7c6c-8534-11e5-83e3-02ee2ddab7fe";
char channel[] = "P1";

void setup()
{
  pinMode(Led, OUTPUT);
  flash();
  
  Serial.begin(9600);
  Serial.println("Serial set up");

  while (!Ethernet.begin(mac)) {
    Serial.println("Ethernet setup error");
    delay(1000);
  }
  Serial.println("Ethernet set up");

  PubNub.begin(pubkey, subkey);
  Serial.println("PubNub set up");
}

void flash()
{
  /* Flash LED three times. */
  for (int i = 0; i < 3; i++) {
    digitalWrite(Led, HIGH);
    delay(100);
    digitalWrite(Led, LOW);
    delay(100);
  }
}

void loop()
{
  Ethernet.maintain();
  EthernetClient *client;

  //if( digitalRead(pulsante) == 1 ) {

    delay(100);
    analogica = analogRead( A0 )/102.4;
    
    String messaggio = "{\"Val\":\"";
    messaggio.concat( analogica );
    messaggio.concat( "\"}" );

    client = PubNub.publish(channel, messaggio.c_str());
    Serial.println(messaggio);
  
    if (!client) {
      Serial.println("publishing error");
      delay(1000);
      return;
    }
    
    // Rilegge il serial code di pubNub  
    while (client->connected()) {
      while (client->connected() && !client->available()) ; // wait
      char c = client->read();
      Serial.print(c);
    }
    
    Serial.println("");
      
    client->stop();
}
