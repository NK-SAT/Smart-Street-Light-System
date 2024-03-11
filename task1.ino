#include <Arduino.h>
#if defined(ESP32)
  #include <WiFi.h>
#elif defined(ESP8266)
  #include <ESP8266WiFi.h>
#endif
#include <Firebase_ESP_Client.h>


#include "addons/TokenHelper.h"


#include "addons/RTDBHelper.h"

// Insert your network credentials
#define WIFI_SSID "Akashhh"
#define WIFI_PASSWORD "05062004"

// Insert Firebase project API Key
#define API_KEY "AIzaSyAuPQ4gOjrvMvB0RTqJV2mQLf3wnxkljt0"

// Insert RTDB URLefine the RTDB URL */
#define DATABASE_URL "https://ldr-demo-1-default-rtdb.firebaseio.com" 

//Define Firebase Data object
FirebaseData fbdo;

FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
int count = 0;
bool signupOK = false;

void setup(){
  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to Wi-Fi");
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;

  config.database_url = DATABASE_URL;

  if (Firebase.signUp(&config, &auth, "iotfirebase907@gmail.com", "iotfirebase1995")){
    Serial.println("ok");
    signupOK = true;
  }
  else{
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }

  config.token_status_callback = tokenStatusCallback; 
  
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop(){
  float tc = 100.5;  //Read temperature in C
  float hu = 200.0;    //Read Humidity

  // if (Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 15000 || sendDataPrevMillis == 0)){
  //   sendDataPrevMillis = millis();
  //   // Write an Int number on the database path test/int
  //   if (Firebase.RTDB.setInt(&fbdo, "Temperature/Data", tc)){
  //     Serial.println("PASSED");
  //     Serial.println("PATH: " + fbdo.dataPath());
  //     Serial.println("TYPE: " + fbdo.dataType());
  //   }
  //   else {
  //     Serial.println("FAILED");
  //     Serial.println("REASON: " + fbdo.errorReason());
  //   }
  //   count++;
    
  //   // Write an Float number on the database path test/float
  //   if (Firebase.RTDB.setFloat(&fbdo, "Humidity/Data", hu)){
  //     Serial.println("PASSED");
  //     Serial.println("PATH: " + fbdo.dataPath());
  //     Serial.println("TYPE: " + fbdo.dataType());
  //   }
  //   else {
  //     Serial.println("FAILED");
  //     Serial.println("REASON: " + fbdo.errorReason());
  //   }
  // }
  Firebase.RTDB.setFloat(&fbdo, "Temperature/Data", tc);
  // Serial.println("Temperature: ");
  // Serial.println(tc);

  Firebase.RTDB.setFloat(&fbdo, "Humidity/Data", hu);
  delay(1000);
}
