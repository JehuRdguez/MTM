#include <SoftwareSerial.h>

char op;

int autoLuz = 1;
int autoGas = 1;
int autoUltra = 1;
int autoFlama = 1;
int autoTemp = 1;


//GAS
int smoke = A2;
int E1 = 3;
int E2 = 2;
int ENA = 5;

//ULTRASÓNICO
long dis;
long tiempo;
int buzzer=6;

//TEMP. Y HUM.
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <dht.h>
#define dht_apin A0
dht DHT;
LiquidCrystal_I2C lcd(0x27, 16,2);

//FLAMA
#include <PWMServo.h>
PWMServo myservo;
int flama = A12;
int pos = 0;

//FOTORRESISTENCIA
int led=46;
int led2=48;
int foto=A4;
int umbral=5;



SoftwareSerial BT(12,13); //RX,TX puertos
void setup() {
  Serial.begin(9600);
    setupBluetooth();
  BT.begin(9600);

  //GAS
    pinMode(smoke, INPUT);
  pinMode(E1, OUTPUT);
pinMode(E2, OUTPUT);

  //ULTRASÓNICO
pinMode(buzzer, OUTPUT);
pinMode(51, OUTPUT); //TRIG
pinMode(53, INPUT); //ECHO


//TEMP. Y HUM.
  lcd.init();   
  lcd.begin(16, 2);
  lcd.backlight();

//FLAMA
myservo.attach(SERVO_PIN_A); //PIN 11

//FOTORRESITENCIA
  pinMode(led, OUTPUT);
  pinMode(foto,INPUT);

}



void sendData() {

    int valorgas= analogRead(smoke);
    int fuego = analogRead(flama);
    int luz = analogRead(foto);

    BT.print(valorgas); //0
    BT.print("  ");//1
    BT.print(dis);//2
    BT.print("  ");//3
    BT.print(DHT.humidity);//4
    BT.print("  ");//5
    BT.print(DHT.temperature);//6
    BT.print("  ");//7
    BT.print(fuego);//8
    BT.print("  ");//9
    BT.print(luz);//10
    BT.print("  ");//11
    BT.print(autoLuz);//12
    BT.print("  ");//13
    BT.print(autoGas);//14
    BT.print("  ");//15
    BT.print(autoUltra);//16
    BT.print("  ");//17
    BT.print(autoFlama);//18
    BT.print("  ");//19
    BT.print(autoTemp);//20
    
    delay(150);
    BT.write(0xA);
    BT.flush();  
  
} 



void loop() {


//////////////////////////////////////////////////////////////////////MANUAL///////////////////////////////////////////////////////////////
//////////////////////////////////////////////////BLUETOOTH A ARDUINO
op=BT.read();


//ULTRASONICO - BUZZER
if(op =='1'){
 tone(buzzer, 525, 200);
  delay(300);
  noTone(buzzer);
  delay(300);
  tone(buzzer, 525, 200);
  delay(300);
  noTone(buzzer);
  delay(300);
  tone(buzzer, 525, 200);
  delay(300);
Serial.println(op);
}
if(op =='2'){
  noTone(buzzer);
  delay(2000);
Serial.println(op);
}

//FLAMA - SERVO
if(op=='3'){
  myservo.write(0);
Serial.println(op);
}
if(op=='4'){
  myservo.write(120);
Serial.println(op);
}

//FOTORRESISTENCIA - LED
if(op=='5'){
analogWrite(led, 255);
analogWrite(led2, 255);
Serial.println(op);
}

if(op=='6'){
digitalWrite(led, LOW);
digitalWrite(led2, LOW);
Serial.println(op);
}

//GAS - VENTI
if(op=='7'){
    digitalWrite(E1, HIGH);
digitalWrite(E2, LOW);
analogWrite(ENA,200);
Serial.println(op);
}
if(op=='8'){
analogWrite(ENA,0);
Serial.println(op);
}

//GAS - VENTI
if(op=='a'){
      autoLuz = 1;
Serial.println(op);
}
if(op=='b'){
    autoLuz = 0;
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
Serial.println(op);
}
if(op=='c'){
      autoGas = 1;
Serial.println(op);
}
if(op=='d'){
    autoGas = 0;
analogWrite(ENA,0);
Serial.println(op);
}

///////////////////////////////////////////////////////ARDUINO A BLUETOOTH
//ULTRASONICO - BUZZER
op=Serial.read();


//ULTRASONICO - BUZZER
if(op=='1'){
tone(buzzer, 525, 200);
  delay(300);
  noTone(buzzer);
  delay(300);
  tone(buzzer, 525, 200);
  delay(300);
  noTone(buzzer);
  delay(300);
  tone(buzzer, 525, 200);
  delay(300);

Serial.println(op);
BT.println(op);
}

if(op=='2'){
noTone(buzzer);
  delay(2000);

Serial.println(op);
BT.println(op);
}

//FLAMA - SERVO
if(op=='3'){ //ABRIR PUERTA
  myservo.write(0);
Serial.println(op);
BT.println(op);
}
if(op=='4'){ //CERRAR PUERTA
  myservo.write(120);
Serial.println(op);
BT.println(op);
}

//FOTORRESISTENCIA - LED
if(op=='5'){
digitalWrite(led, HIGH);
digitalWrite(led2, HIGH);
  Serial.println(op);
BT.println(op);
}
if(op=='6'){
digitalWrite(led, LOW);
digitalWrite(led2, LOW);
Serial.println(op);
BT.println(op);
}

//GAS - VENTI
if(op=='7'){
  digitalWrite(E1, HIGH);
digitalWrite(E2, LOW);
analogWrite(ENA,200);
delay(5000);
Serial.println(op);
BT.println(op);
}
if(op=='8'){
digitalWrite(E1, HIGH);
digitalWrite(E2, LOW);
analogWrite(ENA,0);
Serial.println(op);
BT.println(op);
}

if(op=='a'){ //ENCENDER AUTOMATICO
  autoLuz = 1;
Serial.println(op);
BT.println(op);
}
if(op=='b'){ //APAGAR AUTOMATICO 
  autoLuz = 0;
  digitalWrite(led, LOW);
  digitalWrite(led2, LOW);
Serial.println(op);
BT.println(op);
}

if(op=='c'){ //ENCENDER AUTOMATICO 
  autoLuz = 1;
Serial.println(op);
BT.println(op);
}
if(op=='d'){ //APAGAR AUTOMATICO
  autoLuz = 0;
  analogWrite(ENA,0);
Serial.println(op);
BT.println(op);
}

///////////////////////////////////////////////////////////////////////AUTOMÁTICO///////////////////////////////////////////////////////////////////
//ULTRASÓNICO
if (autoUltra == 1){
  digitalWrite(51, LOW);
  delayMicroseconds(10);
  
  digitalWrite(51, HIGH);
  delayMicroseconds(10);
  
  tiempo=pulseIn(53,HIGH);
  dis=long(0.017*tiempo);
  
  if(dis<6 || dis<=1){
    Serial.print("Distancia peligrosa: ");
    Serial.print(dis);
    Serial.println("cm");

  tone(buzzer, 525, 200);
  delay(150);
  } else {
    noTone(buzzer);
   }
}

//GAS
if (autoGas == 1){
int analogSensor = analogRead(smoke);
  digitalWrite(E1, HIGH);
digitalWrite(E2, LOW);

if(analogSensor  > 300){
  analogWrite(ENA,255);
  Serial.print("Gas: ");
  Serial.println(analogSensor);
  delay(1000);
} else {
    analogWrite(ENA,0);
  }
}

//TEMP. Y HUM.

DHT.read11(dht_apin);

if(op=='t'){
   Serial.print("Humedad= ");
   Serial.print(DHT.humidity);
   Serial.print("% ");
   Serial.print("Temperatura = ");
   Serial.print(DHT.temperature);
   Serial.println("C ");
}

//TEMP. Y HUM.
lcd.setCursor(0, 0);
  lcd.print("Humedad: ");
  lcd.print(DHT.humidity); 
  lcd.print("%");

  lcd.setCursor(0, 1);
  lcd.print("Temp.: ");
  lcd.print(DHT.temperature);
  lcd.print("*C");


//FLAMA
if (autoFlama == 1){
int fuego = analogRead(flama);

if (fuego < 100){
  myservo.write(0);
  Serial.println(fuego);
  delay(15);
  } 
}



if (autoLuz == 1){
//FOTORRESISTENCIA
  int luz = analogRead(foto);
  if(luz<=10){
    Serial.print("Luz: ");
      Serial.println(luz);
analogWrite(led, 255);
analogWrite(led2, 255);
} else if(luz>10) {
    digitalWrite(led, LOW);
        digitalWrite(led2, LOW);
  }
}
  sendData(); 

}

  
//FUNCIÓN DE CONEXIÓN A BT
void setupBluetooth() {
  BT.begin(115200);  // The Bluetooth Mate defaults to 115200bps
  BT.print("$$$");  // Enter command mode
  millis();  // Short delay, wait for the Mate to send back CMD
  BT.println("U,9600,N");  // Temporarily Change the baudrate to 9600, no parity
  // 115200 can be too fast at times for NewSoftSerial to relay the data reliably
  BT.begin(9600);  // Start bluetooth serial at 9600
}