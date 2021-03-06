/*
 Copyright (C) 2012 James Coliz, Jr. <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 
 Update 2014 - TMRh20
 */

/**
 * Simplest possible example of using RF24Network,
 *
 * RECEIVER NODE
 * Listens for messages from the transmitter and prints them out.
 */

#define BASENODE 00
#define THISNODE 01
// The Radiochannel we use
#define RADIOCHANNEL 10
// The CE Pin of the Radio module
#define RADIO_CE_PIN 9
// The CS Pin of the Radio module
#define RADIO_CSN_PIN 10

#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include "printf.h"

// Usage: radio(CE_pin, CS_pin)
RF24 radio(RADIO_CE_PIN,RADIO_CSN_PIN);

RF24Network network(radio);      // Network uses that radio

struct payload_t {                 // Structure of our payload
  unsigned long ms;
  unsigned long counter;
};


void setup(void)
{
  Serial.begin(57600);
  Serial.println("sensorhub/testtools/receiver");
  printf_begin(); 
 
  SPI.begin();
  radio.begin();
  network.begin(RADIOCHANNEL, THISNODE);
  Serial.println("---------------------------------------");
  Serial.print("This Node: ");
  Serial.print(THISNODE, OCT);
  Serial.print("  other Node: ");
  Serial.println(BASENODE, OCT);
  Serial.println("---------------------------------------");
//  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_MAX);
  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
  pinMode(10, HIGH);
}

void loop(void){
  long int i=0;
  int j=0;
  i++;
  network.update();                  // Check the network regularly
  if (i>1000) {
    Serial.println("going power down ...");
    radio.powerDown();
    delay(1000);
    radio.powerUp();
    radio.startListening();
    Serial.println("going power up ...");
    i=1;
    j++;
  } 
//  radio.powerDown();
//  radio.powerUp();
//  radio.begin();
//  network.begin(RADIOCHANNEL, THISNODE);
//  radio.setPALevel(RF24_PA_MAX);
//  radio.startListening();
//beginTransaction();
//SPI.transfer(0xFF);
//endTransaction();
//radio.getDataRate();
//  radio.isPVariant();
//  radio.getCRCLength();
//  radio.getPALevel();
//  radio.printDetails();                   // Dump the configuration of the rf unit for debugging
//  delay(100);
//  radio.powerUp();
//  delay(200);
//  radio.startListening();
//  radio.powerDown();
//  radio.begin();
//  network.begin(RADIOCHANNEL, THISNODE);
  while ( network.available() ) {     // Is there anything ready for us?
    RF24NetworkHeader header;        // If so, grab it and print it out
    payload_t payload;
    network.read(header,&payload,sizeof(payload));
    Serial.print("Received packet #");
    Serial.print(payload.counter);
    Serial.print(" from node: ");
    Serial.print(header.from_node);
    Serial.print(" to node: ");
    Serial.print(header.to_node);
    Serial.print(" at ");
    Serial.println(payload.ms);
    header.to_node = BASENODE;
    header.from_node = THISNODE;
    network.write(header,&payload,sizeof(payload));
    Serial.println("going power down ...");
    radio.powerDown();
    delay(10000);
    radio.powerUp();
    radio.startListening();
    Serial.println("going power up ...");
//    network.write(header,&payload,sizeof(payload));
  }
}

