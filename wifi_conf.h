#ifndef WIFI_CONF_H
#define WIFI_CONF_H

#include <Arduino.h>
#include <WiFi.h>              
#include <ESPmDNS.h>   

#include <ESPAsyncWebServer.h> // https://github.com/me-no-dev/ESPAsyncWebServer/tree/63b5303880023f17e1bca517ac593d8a33955e94
#include <AsyncTCP.h>          // https://github.com/me-no-dev/AsyncTCP
#include <HTTPClient.h>

#include "settings.h"

// https://github.com/espressif/arduino-esp32/blob/master/libraries/WiFi/src/WiFiGeneric.h
// Set your Static IP address
// IPAddress local_IP(192, 168, 1, 184);
// Set your Gateway IP address
// IPAddress gateway(192, 168, 1, 1);

// IPAddress subnet(255, 255, 0, 0);
// IPAddress secondaryDNS(8, 8, 4, 4); // optional

/**
0	ARDUINO_EVENT_WIFI_READY	ESP32 Wi-Fi ready
1	ARDUINO_EVENT_WIFI_SCAN_DONE	ESP32 finishes scanning AP
2	ARDUINO_EVENT_WIFI_STA_START	ESP32 station start
3	ARDUINO_EVENT_WIFI_STA_STOP	ESP32 station stop
4	ARDUINO_EVENT_WIFI_STA_CONNECTED	ESP32 station connected to AP
5	ARDUINO_EVENT_WIFI_STA_DISCONNECTED	ESP32 station disconnected from AP
6	ARDUINO_EVENT_WIFI_STA_AUTHMODE_CHANGE	the auth mode of AP connected by ESP32 station changed
7	ARDUINO_EVENT_WIFI_STA_GOT_IP	ESP32 station got IP from connected AP
8	ARDUINO_EVENT_WIFI_STA_LOST_IP	ESP32 station lost IP and the IP is reset to 0
9	ARDUINO_EVENT_WPS_ER_SUCCESS	ESP32 station wps succeeds in enrollee mode
10	ARDUINO_EVENT_WPS_ER_FAILED	ESP32 station wps fails in enrollee mode
11	ARDUINO_EVENT_WPS_ER_TIMEOUT	ESP32 station wps timeout in enrollee mode
12	ARDUINO_EVENT_WPS_ER_PIN	ESP32 station wps pin code in enrollee mode
13	ARDUINO_EVENT_WIFI_AP_START	ESP32 soft-AP start
14	ARDUINO_EVENT_WIFI_AP_STOP	ESP32 soft-AP stop
15	ARDUINO_EVENT_WIFI_AP_STACONNECTED	a station connected to ESP32 soft-AP
16	ARDUINO_EVENT_WIFI_AP_STADISCONNECTED	a station disconnected from ESP32 soft-AP
17	ARDUINO_EVENT_WIFI_AP_STAIPASSIGNED	ESP32 soft-AP assign an IP to a connected station
18	ARDUINO_EVENT_WIFI_AP_PROBEREQRECVED	Receive probe request packet in soft-AP interface
19	ARDUINO_EVENT_WIFI_AP_GOT_IP6	ESP32 access point v6IP addr is preferred
19	ARDUINO_EVENT_WIFI_STA_GOT_IP6	ESP32 station v6IP addr is preferred
19	ARDUINO_EVENT_ETH_GOT_IP6	Ethernet IPv6 is preferred
20	ARDUINO_EVENT_ETH_START	ESP32 ethernet start
21	ARDUINO_EVENT_ETH_STOP	ESP32 ethernet stop
22	ARDUINO_EVENT_ETH_CONNECTED	ESP32 ethernet phy link up
23	ARDUINO_EVENT_ETH_DISCONNECTED	ESP32 ethernet phy link down
24	ARDUINO_EVENT_ETH_GOT_IP	ESP32 ethernet got IP from connected AP
25	ARDUINO_EVENT_MAX	
*/

  #ifdef WIFI_S

  void WiFiStationConnected(WiFiEvent_t event, WiFiEventInfo_t info){
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    connections+=1;
    Serial.println("Connected to AP successfully!");
  }

  void WiFiGotIP(WiFiEvent_t event, WiFiEventInfo_t info){
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
  }

  void WiFiStationDisconnected(WiFiEvent_t event, WiFiEventInfo_t info){
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    Serial.println("Disconnected from WiFi access point");
    Serial.print("WiFi lost connection. Reason: ");
    Serial.println(info.wifi_sta_disconnected.reason);
    Serial.println("Trying to Reconnect");
    WiFi.begin(ssid, password);
  }

  void initWiFi()
  {
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
    Serial.print("\r\nConnecting to: ");
    Serial.println(String(ssid));

    IPAddress primaryDNS(8, 8, 8, 8);   //  Use Google as DNS

    WiFi.disconnect();

  //   WiFi.softAP(ssid, password);
  //   WiFi.onEvent(WiFiStationConnected, SYSTEM_EVENT_AP_STACONNECTED);

    WiFi.mode(WIFI_MODE_APSTA); // switch off AP
  //   WiFi.config(INADDR_NONE, INADDR_NONE, INADDR_NONE, INADDR_NONE);
  //   WiFi.setHostname(hostname.c_str()); //define hostname
    WiFi.setAutoReconnect(true);

    WiFi.onEvent(WiFiStationConnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_CONNECTED);
    WiFi.onEvent(WiFiGotIP, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_GOT_IP);
    WiFi.onEvent(WiFiStationDisconnected, WiFiEvent_t::ARDUINO_EVENT_WIFI_STA_DISCONNECTED);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(1000);
    }

    Serial.println("\nWiFi connected at: " + WiFi.localIP().toString());
    Serial.print("RRSI: ");
    Serial.println(WiFi.RSSI());
  }

  void scanNetwork(){
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif
      Serial.println("scan start");

    // WiFi.scanNetworks will return the number of networks found
    int n = WiFi.scanNetworks();
    Serial.println("scan done");
    if (n == 0) {
        Serial.println("no networks found");
    } else {
      Serial.print(n);
      Serial.println(" networks found");
      for (int i = 0; i < n; ++i) {
        // Print SSID and RSSI for each network found
        Serial.print(i + 1);
        Serial.print(": ");
        Serial.print(WiFi.SSID(i));
        Serial.print(" (");
        Serial.print(WiFi.RSSI(i));
        Serial.print(")");
        Serial.println((WiFi.encryptionType(i) == WIFI_AUTH_OPEN)?" ":"*");
        delay(10);
      }
    }
    Serial.println("");
  }
  #endif
  
#endif