
#include <Arduino.h>
#include <WiFi.h>
#include <ESPmDNS.h>
#include <SPIFFS.h>
#include <ESPAsyncWebServer.h>  // https://github.com/me-no-dev/ESPAsyncWebServer/tree/63b5303880023f17e1bca517ac593d8a33955e94
#include <AsyncTCP.h>           // https://github.com/me-no-dev/AsyncTCP
#include <DS18B20Events.h>
#include <HTTPClient.h>
#include <MenuLib.h>

#include "mDef.h"
#include "settings.h"
#include "file_sys.h"
#include "wifi_conf.h"
#include "serverHost.h"
#include "timerThermo.h"
#include "webPages.h"

#include "menu.h"

#include "countTimer.h"

#include "relay.h"
#include "Pid.h"

#include "btns.h"



#ifdef ENC_S
  #include "Enc.h"
#endif




void thingSpeakSend(float temperature)
{

    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

  
  if(WiFi.status() == WL_CONNECTED) {
      WiFiClient client; 
      HTTPClient http;

      String url = "http://" + String(serverLinkApi) + "/update";
      http.begin(client, url);
      http.addHeader("Content-Type", "application/x-www-form-urlencoded");
      String httpRequestData = "api_key=" + apiKey + "&field1=" + String(temperature);           


      int httpResponseCode = http.POST(httpRequestData);

      #ifdef DEBUG
        Serial.print("HTTP Response code: ");
        Serial.println(httpResponseCode);
      #endif

      
      http.end();
    }else {
      #ifdef DEBUG
        Serial.println("WiFi Disconnected");
      #endif
      
    }
}



void setup()
{

  setupSystem();

  #ifdef WIFI_S
    initWiFi();
  #endif

  setupTime();

  startSPIFFS();

  #ifdef MENU_S
    initSection();
  #endif

  #ifdef TIMER_S
    timer_setup();
  #endif

  #ifdef RELAY_S
    relaySetup();  
  #endif  
  initDaysArray(); // Initialise the array for storage and set some values
  recoverSettings();  // Recover settings from LittleFS

  #ifdef WEB_S
    startServerHost();
  #endif

  #ifdef TEMP_S
    startSensor();
  #endif

  actuateHeating(OFF);
  lastTimerSwitchCheck = millis() + timerCheckDuration; 

  #ifdef BTN_S
    btnsSetup();
  #endif
  


  #ifdef ENC_S
    encoder_setup();
  #endif


}

void loop()
{ 
  #ifdef BTN_S
    btnsLoop(); 
  #endif

  #ifdef RELAY_S
    relayTick();
  #endif

  #ifdef ENC_S

  read_encoder();
  #endif

  #ifdef MENU_S
    lcdLoop();
  #endif

  #ifdef TIMER_S
    timer_loop();
  #endif



  #ifdef TEMP_S
    if ((millis() - lastTimerSwitchCheck) > timerCheckDuration)
    {
      lastTimerSwitchCheck = millis(); // Reset time
      Temperature = readSensorComplete();                   

      //httpRequest(Temperature);
      
      UpdateLocalTime();               // Updates Time UnixTime to 'now'
      CheckTimerEvent();               // Check for schedules actuated
    }

    if ((millis() - LastReadingCheck) > (lastReadingDuration * 60 * 100))//1000
    {
      LastReadingCheck = millis(); // Update reading record every ~n-mins e.g. 60,000uS = 1-min
      AssignSensorReadingsToArray();
    }

    if (millis() - lastConnectionTime > 10000) {
      lastConnectionTime = millis();
      Temperature = readSensor();
      // thingSpeakSend(Temperature);
    }
  #endif

  loopPID();
}



void AssignSensorReadingsToArray()
{
  #ifdef DEBUG_FUNC
    Serial.println(__func__);
  #endif
  
  SensorReading[1][0] = 1;
  SensorReading[1][1] = Temperature;
  SensorReading[1][2] = RelayState;
  AddReadingToSensorData(1, Temperature); 
}

void AddReadingToSensorData(byte RxdFromID, float Temperature)
{ 
  #ifdef DEBUG_FUNC
    Serial.println(__func__);
  #endif
  byte ptr, p;
  ptr = SensorReadingPointer[RxdFromID];
  sensordata[RxdFromID][ptr].Temp = Temperature;
  ptr++;
  if (ptr >= SensorReadings)
  {
    p = 0;
    do
    {
      sensordata[RxdFromID][p].Temp = sensordata[RxdFromID][p + 1].Temp;
      p++;
    } while (p < SensorReadings);
    ptr = SensorReadings - 1;
    sensordata[RxdFromID][SensorReadings - 1].Temp = Temperature;
  }
  SensorReadingPointer[RxdFromID] = ptr;
}


void setupSystem()
{
  Serial.begin(115200); // Initialise serial communications
  Serial.setDebugOutput(true);
  delay(200);

  #ifdef DEBUG
    Serial.println(__FILE__);
    Serial.println("Starting...");
  #endif

  
}




