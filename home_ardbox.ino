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
  Serial.println("Hello from ARDBox");
  Serial.println(F("Usage: #relay,#period_milli\n"));
#endif
  ardbox.setup(callback);
}

void loop() {
  ardbox.loop();
  String buf = "";
  int relay_number = 0;
  int period = 0;
  int cmd = -1;
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
      if(strncmp(buf.c_str(), "false", 5)==0) {
        cmd = 0;
      } else if(strncmp(buf.c_str(), "true", 4)==0) {
        cmd = 1;
      } else {
        period = atoi(buf.c_str());
      }
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

  if (relay_number > 0 && cmd > -1) {
    if(cmd == 0)
      ardbox.switchRelay(relay_number, false);
    else if(cmd == 1)
      ardbox.switchRelay(relay_number, true);
  }

}
