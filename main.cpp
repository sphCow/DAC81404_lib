#include <Arduino.h>
#include "dac81404.h"

#define DAC_CS 10
#define DAC_RST 28
#define DAC_LDAC -1

DAC81404 dac(DAC_CS, DAC_RST, DAC_LDAC, &SPI, 30000000);

// Try AD5262
#define _SCK_PIN 35
#define _MOSI_PIN 34
#define _CS_PIN 33
void writeDigiPOT(byte address, byte value) {

  //Check to make sure address is valid, if not return without doing anything
  if (address > 1) {
	return;
  }
	
  //Make sure communication lines are initialized
  digitalWrite(_SCK_PIN, LOW);
  digitalWrite(_CS_PIN, LOW);
  //digitalWrite(_MOSI_PIN, LOW);
  
  uint16_t data = word(address,value);
  //_val[address] = value;
    
  int send_bit;
	for (int i = 8; i >= 0; i--){
		send_bit = (data >> i) & 0x01;	// mask out i_th bit
		digitalWrite(_MOSI_PIN, send_bit); // start MSB first
		delayMicroseconds(1);
	
		digitalWrite(_SCK_PIN, HIGH);
		delayMicroseconds(1);
		digitalWrite(_SCK_PIN, LOW);
		delayMicroseconds(1);
	}
  //Set chip select pin back to high
  digitalWrite(_CS_PIN,HIGH);
}


void setup() {
  Serial.begin(115200);
  delay(1000);

  // TRY Dpot
  pinMode(_MOSI_PIN, OUTPUT);
  pinMode(_SCK_PIN, OUTPUT);
  pinMode(_CS_PIN, OUTPUT);
  writeDigiPOT(0, 100);


  Serial.println("init");

  int res = dac.init();
  if(res==0) Serial.printf("DAC81404 init success\n");

  // int ref
  dac.set_int_reference(false);
  Serial.printf("internal ref stat = %d\n", dac.get_int_reference());

  // power up ch0
  dac.set_ch_enabled(0, true);
  dac.set_ch_enabled(1, true);
  dac.set_ch_enabled(2, true);
  dac.set_ch_enabled(3, true);
  
  // check channel power status
  for(int i=0; i<4; i++) Serial.printf("ch %d power -> %d\n", i, dac.get_ch_enabled(i));
  Serial.println();

  // Now set range
  dac.set_range(0, DAC81404::U_10);
  dac.set_range(1, DAC81404::B_5);
  dac.set_range(2, DAC81404::U_6);
  dac.set_range(3, DAC81404::B_10);
  
  // try to read range
  for(int i=0; i<4; i++) Serial.printf("ch %d range -> %d\n", i, dac.get_range(i));

  //dac.set_sync(1, DAC81404::SYNC_LDAC);
  //dac.set_sync(0, DAC81404::SYNC_SS);


}

uint16_t i = 0;
bool done = true;

void loop() {
  dac.set_out(0, (i)%65535 );
  dac.set_out(1, (2*i)%65535 );
  dac.set_out(2, (3*i)%65535 );
  dac.set_out(3, (4*i)%65535);

  i = (i+64)%65535;  
  delayMicroseconds(1);

  
  /*
  if(millis()>10000 && done) {
    dac.set_ch_enabled(2, false);
    Serial.println("ch 2 shutdown");
    for(int i=0; i<4; i++) Serial.printf("ch %d power -> %d\n", i, dac.get_ch_enabled(i));
    Serial.println();
    done = false;
  } */

  for(int i=0; i<256; i++) {
    writeDigiPOT(0, i);
    delay(1);
  }

}