#include <Firebase.h>
#include <FirebaseArduino.h>
#include <FirebaseCloudMessaging.h>
#include <FirebaseError.h>
#include <FirebaseHttpClient.h>
#include <FirebaseObject.h>

#include <DHT.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>

 
#define FIREBASE_HOST "nextgenfarmautomationfinal-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "Bdm3Fg7YYtBndWK7DYBCUqgXuFHkcOCn8W7Cl06X"
#define WIFI_SSID "Subham@123" 
#define WIFI_PASSWORD "12345678"  
 
#define DHTPIN D2   
#define DHTTYPE DHT11   // DHT 11
DHT dht(DHTPIN, DHTTYPE);

const int ledPin = D1;               //Relaypin
const int moisturePin = A0;

 
void setup() 
{
  Serial.begin(115200);
  dht.begin();                                                  //reads dht sensor data          
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
  pinMode(moisturePin, INPUT);
  
}
 
void loop() 
{
 
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // Reading temperature as Celsius (the default)
  float moisturePercentage;
  String motorStatus;
  String temp = String(t);
  String humid = String(h);

 
  Serial.print("Humidity: ");  
  Serial.print(humid);
  Serial.print("%  Temperature: ");  
  Serial.print(temp);  
  Serial.println("°C ");
  delay(1000);


  moisturePercentage = abs( 100.00 - ( (analogRead(moisturePin) / 1023.00) * 100.00 ) );
  
 
  Serial.print("Soil Moisture is  = ");
  String moisture = String(moisturePercentage);
  Serial.print(moisturePercentage);
  Serial.println("%");
  delay(1000);
    
 

  if(moisturePercentage >=0 && moisturePercentage <= 45)
  {
		digitalWrite(ledPin,HIGH);
		motorStatus = "ON";
		Serial.println(motorStatus);    
  }
  else
  {
		digitalWrite(ledPin,LOW);
		motorStatus = "OFF";
		Serial.println(motorStatus);
  }
 
  //--------------- Sending Temperature Data to Firebase
  Firebase.setString("SensorData/tempValue",temp);
  if (Firebase.failed())
  {
    Serial.print("Setting /tempValue failed :");
    Serial.println(Firebase.error());
    delay(500);
    return;
  }
  
  //-------------- Sending Humidity Data to Firebase
  Firebase.setString("SensorData/humidityValue",humid);
  if (Firebase.failed())
  {
    Serial.print("Setting /humidityValue failed :");
    Serial.println(Firebase.error());
    delay(500);
    return;
  }
  
  //-------------- Sending Humidity Data to Firebase
  Firebase.setString("SensorData/moisturePercentage",moisture);
  if (Firebase.failed())
  {
    Serial.print("Setting /moisturePercentage failed :");
    Serial.println(Firebase.error());
    delay(500);
    return;
  }
  
  //-------------- Sending MotorStatus Data to Firebase
  Firebase.setString("SensorData/motorStatus",motorStatus);
  if (Firebase.failed())
  {
    Serial.print("Setting /motorStatus failed :");
    Serial.println(Firebase.error());
    delay(500);
    return;
  }
  
  
  delay(500);
}
