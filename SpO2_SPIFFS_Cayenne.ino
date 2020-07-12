// https://github.com/VU2MEV/MAX30100_NodeMCU_SPIFFS_IOT 


#include "FS.h"
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"
#define CAYENNE_PRINT Serial
#include <CayenneMQTTESP8266.h>
#define REPORTING_PERIOD_MS 10000         // set to 10secs. Set as appropriate.


PulseOximeter pox;
float BPM, SpO2;
uint32_t tsLastReport = 0;
String hrData = "";
unsigned long timems =0;          // variable for time of reading in milliseconds.
 
                                  // WiFi network credentials.
char ssid[] = "wifi_ssid";      
char wifiPassword[] = "wifi_password";

                                  // Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
char username[] = "MQTT-NAME";
char password[] = "MQTT-PASSWORD";
char clientID[] = "MQTT-CLIENTID";

                      // Callback (registered below) fired when a pulse is detected
void onBeatDetected()
{
    Serial.println("Beat!");
}

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  Cayenne.begin(username, password, clientID, ssid, wifiPassword);
  Serial.print("Initializing Pulse Oximeter..");
 pinMode(16, OUTPUT);
    if (!pox.begin())
    {
         Serial.println("FAILED");
         for(;;);
    }
    else
    {
         Serial.println("SUCCESS");
         digitalWrite(2, HIGH);   //Turn off in-built LED
     }
   pox.setOnBeatDetectedCallback(onBeatDetected);
                                // The default current for the IR LED is 50mA and it could be changed by uncommenting the following line.
   pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);

    if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }
 
}
 
void loop() {

  pox.update();
  
  BPM = pox.getHeartRate();
  SpO2 = pox.getSpO2();

  if (BPM < 40 || SpO2 == 0) {                                      // Neglects low readings and starts loop again.
  Serial.println(F("low value from sensor!"));
 
  return;
  }
  
   if (millis() - tsLastReport > REPORTING_PERIOD_MS)
  {
  digitalWrite(2, LOW);                                              // Turn ON LED everytime reading is saved
   // Cayenne.loop();
  Serial.print("Heart rate: ");  Serial.print(BPM);
  timems = millis();
  hrData = String(timems) + String(",") + String(BPM) + String(",") + String(SpO2);                  //convert variable from integer to string
  Serial.print(" bpm , SpO2:");  Serial.print(SpO2);  Serial.println(" %");
  Serial.println (hrData);

  File file = SPIFFS.open("/HR_test.txt", "a");                    // replace by your file name. Open file as w-overwrite/ a-append
  Cayenne.virtualWrite(0, BPM, "counter","null" );
  Cayenne.virtualWrite(1, SpO2, "O2", "p");
  
  if(!file){
    Serial.println("Failed to open file for writing");
    return;
  }
  file.println (hrData);
  file.close();
 
  digitalWrite(2, HIGH);                                             //Turn OFF
  
  tsLastReport = millis();
  }
  
}
