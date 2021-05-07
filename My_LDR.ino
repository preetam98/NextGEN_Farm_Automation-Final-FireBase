#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

#define FIREBASE_HOST "nextgenfarmautomationfinal-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "Bdm3Fg7YYtBndWK7DYBCUqgXuFHkcOCn8W7Cl06X"

#define WIFI_SSID "Subham@123" 
#define WIFI_PASSWORD "12345678"

const int ledPin = D2;         // D8 pin of NodeMCU
const int ldrPin = A0;          // D1 pin of NodeMCU

void setup() {
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);                                  
  Serial.print("Connecting to ..");
  Serial.print(WIFI_SSID);
  
  //------------- Wait for connection 
  
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print("Connecting ...\n");
    delay(500);
  }
 
  Serial.println("");
  Serial.print("Connected");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP()); 
  
  //------- Firebase Realtime database Configuration
  
  Firebase.begin(FIREBASE_HOST,FIREBASE_AUTH);
  

  pinMode(ledPin, OUTPUT);
  pinMode(ldrPin,INPUT);
}
void loop(){
  int rawData = analogRead(ldrPin);
  String ldrStatus = String(rawData);
  
  if (rawData < 150){
    digitalWrite(ledPin, HIGH);
    ldrStatus = "ON";
    Serial.println(ldrStatus);
  }
  else{
    digitalWrite(ledPin, LOW);
    ldrStatus = "OFF";
    Serial.println(ldrStatus);
  }
  Serial.println(rawData);
  
  Firebase.setString("SensorData/ldrStatus",ldrStatus);
  if (Firebase.failed())
  {
    Serial.print("Setting /motorStatus failed :");
    Serial.println(Firebase.error());
    delay(500);
    return;
  }
  delay (1000);
}
