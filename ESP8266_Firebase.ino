#include <ESP8266WiFi.h>
#include <Firebase_ESP_Client.h>
const int ldrpin= A0;
#define relay D5

 
#define WIFI_SSID "Redmi9" //Change
#define WIFI_PASSWORD "87654321" //Change

// Database legacy token 
#define DATABASE_SECRET "BFtgLebfvrVJ0X9XJyjelnLcQrv0wqBCN5vHYWQy" // Found at Project settings -> Service account -> DB secrets -> Secret Key
#define DATABASE_URL "https://ldr-demo-1-default-rtdb.firebaseio.com"

// Authentication with email 
#define USER_EMAIL "iotfirebase907@gmail.com"
#define USER_PASSWORD "iotfirebase1995"

// Define Firebase Data object
FirebaseData fbdo;

// Firebase auth and config
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;

// Records in database
unsigned long count = 0;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting"); 

  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
}
  Serial.println();
  Serial.print("Connected: ");
  Serial.println(WiFi.localIP());
  Serial.println();

  // Firebase Client Version
  Serial.printf("Firebase Client v%s\n\n", FIREBASE_CLIENT_VERSION);

  /* Assign the user sign in credentials */
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;

  /* Assign the RTDB URL (required) */
  config.database_url = DATABASE_URL;
  config.signer.tokens.legacy_token = DATABASE_SECRET;

  // Connect to Firebase
  Firebase.begin(&config, &auth);
  Serial.println("Configured!");
  
  //ESP8266 SETUP
  pinMode(ldrpin,INPUT);
  pinMode(relay, OUTPUT);

}
 
void loop() {
 
  int ldr_val1=analogRead(ldrpin); 
  Serial.println(ldr_val1);
  String mode=Firebase.RTDB.getString(&fbdo, F("/mode")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  String status=Firebase.RTDB.getString(&fbdo, F("/powerStatus")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
  Serial.println(mode);
  Serial.println(status);
  if(mode=="automatic"){
    if(ldr_val1<70){
      digitalWrite(relay, LOW);
      Firebase.RTDB.setString(&fbdo, F("/powerStatus/"),"On");
    }
    else
    {
       digitalWrite(relay, HIGH);
       Firebase.RTDB.setString(&fbdo, F("/powerStatus/"),"Off");
    }
    delay(100);
  }
  else{
    status=Firebase.RTDB.getString(&fbdo, F("/powerStatus")) ? fbdo.to<const char *>() : fbdo.errorReason().c_str();
    if(status=="On"){
      digitalWrite(relay, LOW);
      Firebase.RTDB.setString(&fbdo, F("/powerStatus/"),"On");
    }
    else{
      digitalWrite(relay, HIGH);
      Firebase.RTDB.setString(&fbdo, F("/powerStatus/"),"Off");
    }
  }

}