#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>

SoftwareSerial mySerial(2,3);
LiquidCrystal_I2C lcd(0x27,16,2); 

String data;
char c;
const int relay = 7;

int pot = A0;
float valPot; //variabel hasil pembacaan sensor
long in_min = 1023; // variabel ketika sensor tidak kena air
long in_max = 250; // variabel ketika sensor tercelum semua kedalam air
String val;
unsigned long waktu = 0;
const long periode = 1000;
int value;

void Kelembaban(){
  valPot= analogRead(pot);
  valPot = (valPot - in_min)/(in_max-in_min);
  valPot = valPot *100;
  Serial.println(valPot);
}

void Display(){
   lcd.clear();
   lcd.setCursor(0, 0);
   lcd.print("Kelembaban:");
   lcd.setCursor(0, 1);
   lcd.print(valPot);
   lcd.print("%");
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(115200);
  pinMode(relay, OUTPUT);
  digitalWrite(relay, LOW);
  delay(10);
  lcd.init();
  lcd.backlight();
}
void serialEvent(){
  
}
void loop() {
  // put your main code here, to run repeatedly:
  unsigned long waktunow = millis(); // store the current time
  if (waktunow - waktu >= periode) { // check if 1000ms passed
   waktu = waktunow;   // save the last time you blinked the LED
   Kelembaban();
   Display();
  
  }
  else{
  while (mySerial.available()) {
      delay(10);
      c = mySerial.read();
      data += c;
    }
    if (data.length()>0) {
    Serial.println(data);
    if(data == "2"){
       digitalWrite(relay, HIGH);
       //Serial.println("Menyiram");
       delay(100);
       data = "";
     }
    else if ( data == "1"){
       digitalWrite(relay, LOW);
       //Serial.println("Berhenti");
       delay(100);
       data = "";
     } 
    else if ( data =="r"){
      Kelembaban();
      mySerial.print(valPot,0);
      delay(100);
      data = "";
    }
   }
  }
}
