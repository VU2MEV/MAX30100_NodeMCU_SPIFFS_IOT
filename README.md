# MAX30100_NodeMCU_SPIFFS_IOT
Monitoring Heart rate and Blood oxygen saturation online with MAX30100 on NodeMCU - ESP8266 and storing data in onboard flash memory. Inexpensive way of monitoring health in COVID pandemic and of atheletes.

In this project we will connect MAX30100, an intergrated pulse oximetry and heart-rate monitor sensor from Maxim Integrated with NodeMCU, an ESP8266 board. Readings are taken at preset intervals and stored in SPIFFS flash memory of the NodeMCU, which can be read later. The readings are also pushed online to IOT platform Cayenne so as to monitor it remotely from anywhere.
The device can be powered from mobile via OTG cable and in-situ SpO2 can be monitored while performing outdoor activities like running, cycling, hiking, skiing, etc. As the data is stored onboard, we do not loose any readings even if no internet connection is available. The stored file can be read later via SPIFFS read code.

# Materials required:

1) ESP8266 like NodeMCU.
2) MAX30100 integrated heartrate and plse oximeter.
3) Connecting wires.
4) OTG cable (if want to power via mobile later)

# Connections:
  
    NodeMCU --->>  MAX30100
    -----------------------
     3.3V/Vin --->   Vin
     GND    --->   GND
     D1     --->   SCL
     D2     --->   SDA
     D0     --->   INT
      
Prerequesites:

1) Installed Arduino IDE. 
2) Loaded ESP8266 core. 
3) SPIFFS initiated in IDE. (https://github.com/esp8266/arduino-esp8266fs-plugin)
4) Cayenne login. (https://cayenne.mydevices.com/)

# Main Libraries used:

1) Wire.h
2) MAX30100_PulseOximeter.h ( https://github.com/oxullo/Arduino-MAX30100 )
3) CayenneMQTTESP8266.h ( https://github.com/myDevicesIoT/Cayenne-MQTT-ESP )
4) FS.h

# The code:

Put your wifi network credentials here. Followed by the MQTT credentials you got for your device in mydevices.com/cayenne
    
    // WiFi network info.
    char ssid[] = "wifi_ssid";
    char wifiPassword[] = "wifi_password";
    
    // Cayenne authentication info. This should be obtained from the Cayenne Dashboard.
    char username[] = "MQTT-NAME";
    char password[] = "MQTT-PASSWORD";
    char clientID[] = "MQTT-CLIENTID";

IR LED power setting: To get reliable readings try different current settings. Since different people have different skin thickness, color, blood density, it is better to adjust the value to where you get stable heart rate reading. Acceptable values are: CURR_0MA, CURR_4_4MA, CURR_7_6MA, CURR_11MA, CURR_14_2MA, CURR_17_4MA, CURR_20_8MA, CURR_24MA, CURR_27_1MA, CURR_30_6MA, CURR_33_8MA, CURR_37MA, CURR_40_2MA, CURR_43_6MA, CURR_46_8MA, CURR_50MA.

     pox.setIRLedCurrent(MAX30100_LED_CURR_24MA);

Set File name for the file that you will store in the flash memory of NodeMCU. The mode is "a" for append, so that you retain the data after replugging the device.

     File file = SPIFFS.open("/HR_test.txt", "a");

Data is sent to channel 0 and channel 1 in Cayenne.

    Cayenne.virtualWrite(0, BPM, "counter","null" );
    Cayenne.virtualWrite(1, SpO2, "O2", "p");
    
Data is stored in Cayenne project which can be visualised as graph or values. The data in flash memory can be printed on the serial monitor by uploading the following small read file code:

    #include "FS.h"
 
    void setup() {
    Serial.begin(115200);
  
    if(!SPIFFS.begin()){
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
    }
 
    File file = SPIFFS.open("/HR_test.txt", "r");     // replace by your file name.
    
    if(!file){
    Serial.println("Failed to open file for reading");
    return;
    }
  
    Serial.println();
    Serial.println("File Content:");
    Serial.println(file.size());       // Displays the file size in bytes.
    
    while(file.available()){          // Reading and writing the data in serial monitor.
    Serial.write(file.read());
    }
    file.close();
    }
 
    void loop() {
    }

Done. Additionally in the main code, one can attach a I2C display and read the values on it locally.

# Footnote:

If yor MAX30100 doesn't turn on even with the basic MAX30100_Minimal example sketch, please check online the hack to pull SDA and SCL pins to 3.3V via the on board 4.7Kohms resistors. Ther's no need to remove the smd resistors, only disconnect the regulator o/p and connect 3.3V there.
