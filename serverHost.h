#include "ESPAsyncWebServer.h"
#ifndef serverHost_h
#define serverHost_h

#include <Arduino.h>
#include "settings.h"
#include "timerThermo.h"
#include "webPages.h"
#include "btns.h"


#ifdef WEB_S

  /**
  * @brief The name that will identify your device on the network
  * @todo move to wifi configuration file
  * @param DeviceName New device name
  */
  void setupDeviceName(const char *DeviceName) {
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    if (MDNS.begin(DeviceName)) {
      Serial.println("mDNS responder started");
      Serial.print("Device name: ");
      Serial.println(DeviceName);
      MDNS.addService("n8i-mlp", "tcp", 23);  // Add service
    } else {
      Serial.println("Error setting up MDNS responder");
    }
  }


  void saveSettings() {
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    Serial.println("Getting ready to Save settings...");
    File dataFile = SPIFFS.open("/" + DataFile, "w");
    if (dataFile) {  // Save settings
      Serial.println("Saving settings...");
      for (byte dow = 0; dow < 7; dow++) {
        Serial.println("Day of week = " + String(dow));
        for (byte p = 0; p < NumOfEvents; p++) {
          dataFile.println(Timer[dow].Temp[p]);
          dataFile.println(Timer[dow].Start[p]);
          dataFile.println(Timer[dow].Stop[p]);
          Serial.println("Period: " + String(p) + " " + Timer[dow].Temp[p] + " from: " + Timer[dow].Start[p] + " to: " + Timer[dow].Stop[p]);
        }
      }
      dataFile.println(Hysteresis, 1);
      dataFile.println(FrostTemp, 1);
      dataFile.println(EarlyStart);
      Serial.println("Saved Hysteresis : " + String(Hysteresis));
      Serial.println("Saved Frost Temp : " + String(FrostTemp));
      Serial.println("Saved EarlyStart : " + String(EarlyStart));
      dataFile.close();
      Serial.println("Settings saved...");
    }
  }

  void serverHandlersSetup() {
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    // server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
    // request->send(SPIFFS, "/index.html", String(), false);
    // });


    server.on("/", HTTP_GET, [](AsyncWebServerRequest *request) {
      request->redirect("/homepage");
    });
    // Set handler for '/homepage'
    server.on("/homepage", HTTP_GET, [](AsyncWebServerRequest *request) {
      homepage();
      request->send(200, "text/html", webpage);
    });

    server.on("/temperature", HTTP_GET, [](AsyncWebServerRequest *request) {  //todo
      // sensors.requestTemperatures();
      // float temperature = sensors.getTempCByIndex(0);
      float temperature = random(20, 30);
      String json = "{\"temperature\": " + String(temperature) + "}";
      request->send(200, "application/json", json);
    });

    // Set handler for '/graphs'
    server.on("/graphs", HTTP_GET, [](AsyncWebServerRequest *request) {
      graphs();
      request->send(200, "text/html", webpage);
    });
    // Set handler for '/timer'
    server.on("/timer", HTTP_GET, [](AsyncWebServerRequest *request) {
      timerSet();
      request->send(200, "text/html", webpage);
    });
    // Set handler for '/setup'
    server.on("/setup", HTTP_GET, [](AsyncWebServerRequest *request) {
      Setup();
      request->send(200, "text/html", webpage);
    });
    // Set handler for '/help'
    server.on("/help", HTTP_GET, [](AsyncWebServerRequest *request) {
      Help();
      request->send(200, "text/html", webpage);
    });
    // Set handler for '/handletimer' inputs
    server.on("/handletimer", HTTP_GET, [](AsyncWebServerRequest *request) {
      for (byte dow = 0; dow < 7; dow++) {
        for (byte p = 0; p < NumOfEvents; p++) {
          Timer[dow].Temp[p] = request->arg(String(dow) + "." + String(p) + ".Temp");
          Timer[dow].Start[p] = request->arg(String(dow) + "." + String(p) + ".Start");
          Timer[dow].Stop[p] = request->arg(String(dow) + "." + String(p) + ".Stop");
        }
      }
      saveSettings();
      request->redirect("/homepage");  // Go back to home page
    });
    // Set handler for '/handlesetup' inputs
    server.on("/handlesetup", HTTP_GET, [](AsyncWebServerRequest *request) {
      if (request->hasArg("hysteresis")) {
        String numArg = request->arg("hysteresis");
        Hysteresis = numArg.toFloat();
      }
      if (request->hasArg("frosttemp")) {
        String numArg = request->arg("frosttemp");
        FrostTemp = numArg.toFloat();
      }
      if (request->hasArg("earlystart")) {
        String numArg = request->arg("earlystart");
        EarlyStart = numArg.toInt();
      }
      if (request->hasArg("manualoverride")) {
        String stringArg = request->arg("manualoverride");
        if (stringArg == "ON")
          ManualOverride = true;
        else
          ManualOverride = false;
      }
      if (request->hasArg("manualoverridetemp")) {
        String numArg = request->arg("manualoverridetemp");
        ManOverrideTemp = numArg.toFloat();
      }
      saveSettings();
      request->redirect("/homepage");  // Go back to home page
    });


    server.on("/changemode", HTTP_GET, [](AsyncWebServerRequest *request) {
      changeMode();
      request->send(200, "text/html", webpage);
    });
    

    server.on("/heat_set", HTTP_GET, heatControl);
    server.on("/cold_set", HTTP_GET, coldControl);
    server.on("/fan_set", HTTP_GET, fanControl);
  }

  void startServerHost() {
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    setupDeviceName(serverName);
    serverHandlersSetup();
    server.begin();
  }

#endif

#endif