#include <SoftwareSerial.h>

int smokeA0 = A0;


SoftwareSerial BT(2,4); //RX,TX puertos
void setup() {
  Serial.begin(9600);
  pinMode(smokeA0, INPUT);
    setupBluetooth();
  BT.begin(9600);
}



void sendData() {
    int valuegas= analogRead(smokeA0);
    Serial.println(valuegas);

    BT.print(valuegas);
    BT.print("  ");
    
  delay(1000);
  BT.write(0xA);
  BT.flush();  
  
} 



void loop() {

if (BT.available()) {
    Serial.write("");
    while (BT.available()) {
      Serial.write(BT.read());
    }
  }
     sendData(); 
}

  

void setupBluetooth() {
  BT.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  BT.print("$$$");  // Enter command mode
  delay(100);  // Short delay, wait for the Mate to send back CMD
  BT.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  BT.begin(9600);  // Start bluetooth serial at 9600
}
