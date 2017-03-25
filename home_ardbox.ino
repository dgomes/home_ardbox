#include <relaybox.h>

void callback(uint8_t relay_number, bool value) {
  DEBUG_PRINT(relay_number); DEBUG_PRINT(","); DEBUG_PRINTLN(value?"true":"false");
  Serial1.print(relay_number); Serial1.print(","); Serial1.println(value?"true":"false");
}

RelayBox ardbox(_20IO);

void setup() {
  Serial.begin(9600);
  Serial1.begin(2400);
#ifdef DEBUG
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }  
  Serial.println(F("ARDBOX HOMIE v1"));
#endif
  Serial1.println("Hello from ARDBox");
  Serial1.println(F("Usage: #relay,#period_milli\n"));
  ardbox.setup(callback);
}

void loop() {
  ardbox.loop();
  String buf = "";
  int relay_number = 0;
  int period = 0;
  size_t len = 0;

  while (Serial1.available()) {
    char in = Serial1.read();
    len++;

    buf += in;

    if(in == ',') {
      relay_number = atoi(buf.c_str());
      buf = "";
    }
    if(in == '\n') {
      period = atoi(buf.c_str());
      buf = "";
      break;
    }   
    if(len>8)
      break;
    delay(20);
  }
  
  if (relay_number > 0 && period > 0) {
    ardbox.switchRelay(relay_number, period);
  }


}
