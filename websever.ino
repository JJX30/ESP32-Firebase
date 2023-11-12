#include <WiFi.h>
#include <Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define WIFI_SSID 
#define WIFI_PASSWORD 
#define API_KEY 
#define DATABASE_URL 

#define BUTTON_PIN 2
#define LED1_PIN 12
#define LED2_PIN 14
#define LDR_PIN 35
#define PWM_CHANNEL 0

const int freq = 5000;
const int resolution = 8;


FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
float voltage = 0.0;
int pwmValue = 0;
bool ledStatus = false;
bool buttonPressed = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED2_PIN, OUTPUT);
  pinMode(BUTTON_PIN,INPUT_PULLUP); // Set the Tilt Switch as an input
  ledcSetup(PWM_CHANNEL, freq, resolution);
  ledcAttachPin(LED1_PIN, PWM_CHANNEL);

  Serial.begin(115200);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("Connecting to WiFi");
  while(WiFi.status() != WL_CONNECTED){
    Serial.print("."); delay(300);
  }
  Serial.println();
  Serial.print("Connected with IP: ");
  Serial.print(WiFi.localIP());
  Serial.println();

  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  if(Firebase.signUp(&config, &auth, "", "")){
    Serial.print("signUp OK");
    signupOK = true;
  } else {
    Serial.printf("%s\n", config.signer.signupError.message.c_str());
  }
  config.token_status_callback = tokenStatusCallback;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
}

void loop() {
  


  // put your main code here, to run repeatedly:
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0)){
    sendDataPrevMillis = millis();
    // WRITING DATA TO RTDB
    ldrData = analogRead(LDR_PIN);
    voltage = (float)analogReadMilliVolts(LDR_PIN)/1000;
    buttonPressed = false;


    // Check if button is pressed
    if(digitalRead(BUTTON_PIN) == HIGH){
      buttonPressed = true;
      if(Firebase.RTDB.setBool(&fbdo, "Sensor/switch", buttonPressed)){
        Serial.print(buttonPressed);
        Serial.print(" - successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }
    } else {
      buttonPressed= false;
      if(Firebase.RTDB.setBool(&fbdo, "Sensor/switch", buttonPressed)){
        Serial.print(buttonPressed);
        Serial.print(" - successfully saved to: " + fbdo.dataPath());
        Serial.println(" (" + fbdo.dataType() + ")");
      }
    }

    // LDR DATA WRITING 
    if(Firebase.RTDB.setInt(&fbdo, "Sensor/ldr_data", ldrData)){
      Serial.print(ldrData);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    // VOLTAGE DATA WRITING 
    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/voltage", voltage)){
      Serial.print(voltage);
      Serial.print(" - successfully saved to: " + fbdo.dataPath());
      Serial.println(" (" + fbdo.dataType() + ")");
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.getInt(&fbdo, "/LED/analog/")){
      if(fbdo.dataType() == "int"){
        pwmValue = fbdo.intData();
        Serial.println("Successful READ from " + fbdo.dataPath() + ": " + pwmValue + " (" + fbdo.dataType() + ")");
        ledcWrite(PWM_CHANNEL, pwmValue);
      }
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }

    if(Firebase.RTDB.getBool(&fbdo, "/LED/digital/")){
      if(fbdo.dataType() == "boolean"){
        ledStatus = fbdo.boolData();
        Serial.println("Successful READ from " + fbdo.dataPath() + ": " + ledStatus + " (" + fbdo.dataType() + ")");
        digitalWrite(LED2_PIN, ledStatus);
      }
    } else {
      Serial.println("FAILED: " + fbdo.errorReason());
    }
  }
}
