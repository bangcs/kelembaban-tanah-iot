//--------------------- Setting Blynk -------------------------------------
#define BLYNK_TEMPLATE_ID "TMPLbHJ8OvRp" // diganti sesuai ID
#define BLYNK_DEVICE_NAME "Quickstart Template" // diganti sesuai tempalte

//--------------------- Library yang digunakan ----------------------------
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include "ThingSpeak.h"

//--------------------- Setting Blynk -------------------------------------

#define BLYNK_PRINT Serial
char auth[] = "dxFxwGy2J4U1W78gf27y1r8y8wDc2_gG"; // diganti sesuai Token


//--------------------- Setting Thingspeak --------------------------------
unsigned long myChannelNumber = 1962124; // diganti sesuai channel
const char * myWriteAPIKey = "9QSUNSXCUQNUFB4N"; // diganti sesuai API

//--------------------- Setting WiFi --------------------------------------
WiFiClient  client;
const char* ssid = "Galaxy Neutron"; // untuk thingspeak
const char* pass = "joker1998"; // untuk thingspeak

char ssid1[] = "Galaxy Neutron"; // untuk blynk
char pass1[] = "joker1998" ;// untuk blynk

//--------------------- Variabel Global -----------------------------------
BlynkTimer timer;
int waktu = 5000L;
int timerID;
String sensorValue;
int value;
int batas = 0;
char c;

void setup() {
  // put your setup code here, to run once:
  WiFi.begin(ssid1, pass1);
  Blynk.begin(auth, ssid1, pass1);
  Serial.begin(115200);
  timer.setInterval(2000L, sendSensor);
  timer.setInterval(3000L, checkhumidity);
  timerID = timer.setInterval(waktu, catatan);
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  delay(100);
}

void loop() {
  // put your main code here, to run repeatedly:
  Blynk.run();
  timer.run();
  delay(100);
}

//-------------------- Fungsi Fungsi --------------------------------------
//-------------------- Fungsi ambil data ke arduino -----------------------
void sendSensor() {
  Serial.print("r");
  delay(100);
  while (Serial.available() > 0) {
    delay(10);
    c = Serial.read();
    sensorValue += c;
    
  }
  if (sensorValue.length() > 0) {   
   delay(10);
    value = sensorValue.toInt();
    }
  sensorValue = "";
}

void catatan(){
  //---------- mencatat ke thingspeak ------------------------------------
   int x = ThingSpeak.writeField(myChannelNumber, 1, value, myWriteAPIKey);
    
   if(x == 200){
      //Serial.println("Channel update successful.");
    }
   else{
     //Serial.println("Problem updating channel. HTTP error code " + String(x));
    }
   //-------- mencatat ke Blynk ------------------------------------------
    Blynk.virtualWrite(V3, value);
}

//------------------- Fungsi untuk menjaga kelembaban ---------------------
//-------------------- dan mengechek kelembaban ---------------------------
void checkhumidity(){
  Blynk.virtualWrite(V8, value);
  if (value <= batas){
    Serial.print("2");
    delay(100);
    Blynk.virtualWrite(V6, 2);
  }
  else if (value > batas){
    Serial.print("1");
    Blynk.virtualWrite(V6, 1);
    delay(100);
  }
}
//------------------ DATA STREAM BLYNK ------------------------------------

BLYNK_WRITE(V1){
  int virtual_pin_value = param.asInt();
  waktu = virtual_pin_value;
  timer.deleteTimer(timerID);
  timerID = timer.setInterval(waktu * 1000L, sendSensor);
}

BLYNK_WRITE(V7){
  int virtual_pin_value = param.asInt();
  batas = virtual_pin_value;
  delay(100);
}
