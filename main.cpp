#include <Arduino.h>
#include "dac81404.h"

#define DAC_CS 10
#define DAC_RST 28
#define DAC_LDAC -1

DAC81404 dac(DAC_CS, DAC_RST, DAC_LDAC, &SPI, 30000000);

void setup() {
  Serial.begin(115200);
  delay(1000);

  Serial.println("init");

  int res = dac.init();
  if(res==0) Serial.printf("DAC81404 init success\n");

  // int ref
  dac.set_int_reference(false);
  Serial.printf("internal ref stat = %d\n", dac.get_int_reference());

  // power up ch0,1,2,3
  dac.set_ch_enabled(0, true);
  dac.set_ch_enabled(1, true);
  dac.set_ch_enabled(2, true);
  dac.set_ch_enabled(3, true);
  
  // check channel power status
  for(int i=0; i<4; i++) Serial.printf("ch %d power -> %d\n", i, dac.get_ch_enabled(i));
  Serial.println();

  // Now set range, U = Unipolar, B=Bipolar; 
  dac.set_range(0, DAC81404::U_10); //   0 -> +10V
  dac.set_range(1, DAC81404::B_5);  //  -5 -> +5V
  dac.set_range(2, DAC81404::U_6);  //   0 -> +6V
  dac.set_range(3, DAC81404::B_10); // -10 -> +10V
  
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

}
