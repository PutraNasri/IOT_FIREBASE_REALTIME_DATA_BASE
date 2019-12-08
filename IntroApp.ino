
#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include "DHTesp.h"
DHTesp dht;


#define FIREBASE_HOST "dcage-e0208.firebaseio.com"
#define FIREBASE_AUTH "ggUPoYjRMEFBnY8TxkI8Vbk0Tyjm8GK0q8Se72TP"  //buat key baru di firebase,jangan pakai yang lama.
const char* ssid     = "Labodiaj Jkt 2";
const char* password = "labodiajkt2014";
const int pin = 5;
int ledState = LOW;


WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);


String formattedDate;
String dayStamp;
String timeStamp;



void setup() {
  Serial.begin(115200);
  
  pinMode(pin, OUTPUT);
  digitalWrite(pin, ledState);
  
  dht.setup(14, DHTesp::DHT22);
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
//    delay(500);
//    digitalWrite(pin, ledState);
      
      delay(100);
      digitalWrite(pin,HIGH);
      delay(100);
      digitalWrite(pin,LOW);
      Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
//connect to firebase
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);

// Initialize a NTPClient to get time
  timeClient.begin();
  // Set offset time in seconds to adjust for your timezone, for example:
  // GMT +1 = 3600
  // GMT +8 = 28800
  // GMT -1 = -3600
  // GMT 0 = 0
  timeClient.setTimeOffset(25200);

  if(ledState == LOW){
    ledState = HIGH;
  }
  digitalWrite(pin, ledState);
}
void loop() {
  float h = dht.getHumidity();
  float t = dht.getTemperature();
  
  while(!timeClient.update()) {
    timeClient.forceUpdate();
  }
//  Serial.print("TEMP: ");
//  Serial.println(t);
//  Serial.print("HUMI: ");
//  Serial.println(h);
  formattedDate = timeClient.getFormattedDate();
  int splitT = formattedDate.indexOf("T");
  dayStamp = formattedDate.substring(0, splitT);
//  Serial.print("DATE: ");
//  Serial.println(dayStamp);
  timeStamp = formattedDate.substring(splitT+1, formattedDate.length()-1);
//  Serial.print("HOUR: ");
//  Serial.println(timeStamp);
  
  String data = String(t)+"@"+String(h)+"@"+String(dayStamp)+"@"+String(timeStamp);
  Firebase.pushString("/IntroAPP/DHT22/room2",data);
  Serial.println(data);
  delay(300000);
  
}
