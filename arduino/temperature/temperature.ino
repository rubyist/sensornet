#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"

int serial_putc( char c, FILE * ) 
{
  Serial.write( c );

  return c;
} 

void printf_begin(void)
{
  fdevopen( &serial_putc, 0 );
}


RF24 radio(9, 10);

//const uint64_t pipes[2] = {0xe7d3f03578LL, 0xc2c2c2c2c2LL};

const uint64_t sensor_pipes[2] = {0x3a3a3a3ad2LL, 0x3a3a3a3ac3LL};
const uint64_t base_pipes[2] = {0xf0f0f0f0d2LL, 0xf0f0f0f0c3LL};


void setup(void) {
  Serial.begin(57600);
  printf_begin();
  printf("\n\rBegin\n\r");
  
  radio.begin();
  radio.setRetries(15, 15);
  radio.setPayloadSize(8);
  
  // Open our pipe for writing, other pipe for reading
  //radio.openWritingPipe(pipes[1]);
  //radio.openReadingPipe(1, pipes[0]); 
  
  radio.openWritingPipe(sensor_pipes[0]);
  radio.openReadingPipe(1, base_pipes[0]);
  
  radio.startListening();
  radio.printDetails();
}

void loop() {
  radio.stopListening();
  
  float voltage, degreesF;
  voltage = analogRead(0) * 0.004882814;
  degreesF = (voltage - 0.5) * 100.0 * (9.0/5.0) + 32;
  int degrees = (int)degreesF;
  
  Serial.print("Sending temp ");
  Serial.print(degrees);
  bool ok = radio.write(&degrees, sizeof(int));
  if (ok)
    Serial.println(" ok");
  else
    Serial.println(" failed");
  
  radio.startListening();
  
  if (radio.available()) {
    Serial.println("AVAILABLE");
  }
 
  delay(1000); 
}