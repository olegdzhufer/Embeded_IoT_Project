#ifndef timerThermo_h
#define timerThermo_h

#include <Arduino.h>
#include <DS18B20Events.h>          //#include <OneWire.h> is already included in DS18B20Events.h
#include "settings.h"
#include "tempSensor.h"

String ConvertUnixTime(int unix_time)
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  time_t tm = unix_time;
  struct tm *now_tm = localtime(&tm);
  char output[40];
  strftime(output, sizeof(output), "%H:%M", now_tm); // Returns 21:12
  return output;
}

void UpdateTargetTemperature()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  String TimeNow;
  TimeNow = ConvertUnixTime(UnixTime);
  for (byte dow = 0; dow < 7; dow++)
  {
    for (byte p = 0; p < NumOfEvents; p++)
    {
      if (String(dow) == DoW_str && (TimeNow >= Timer[dow].Start[p] && TimeNow < Timer[dow].Stop[p]))
      {
        TargetTemp = Timer[dow].Temp[p].toFloat(); // Found the programmed set-point temperature from the scheduled time period
      }
    }
  }
  if (ManualOverride == ON)
    // TargetTemp = ManOverrideTemp;
  Serial.println("Target Temperature = " + String(TargetTemp, 1) + "°");
}


void actuateHeating(bool demand)
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  pinMode(RelayPIN, OUTPUT);
  pinMode(LEDPIN, OUTPUT);
  if (demand)
  {
    RelayState = "ON";
    if (RelayReverse)
    {
      digitalWrite(RelayPIN, LOW);
    }
    else
    {
      digitalWrite(RelayPIN, HIGH);
    }
    digitalWrite(LEDPIN, LOW);
    Serial.println("Thermostat ON");
  }
  else
  {
    RelayState = "OFF";
    if (RelayReverse)
    {
      digitalWrite(RelayPIN, HIGH);
    }
    else
    {
      digitalWrite(RelayPIN, LOW);
    }
    digitalWrite(LEDPIN, HIGH);
    Serial.println("Thermostat OFF");
  }
}


void ControlHeating()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  if (Temperature < (TargetTemp - Hysteresis))
  {                     // Check if room temeperature is below set-point and hysteresis offset
    actuateHeating(ON); // Switch Relay/Heating ON if so
  }
  if (Temperature > (TargetTemp + Hysteresis))
  {                      // Check if room temeperature is above set-point and hysteresis offset
    actuateHeating(OFF); // Switch Relay/Heating OFF if so
  }
  if (Temperature > MaxTemperature)
  {                      // Check for faults/over-temperature
    actuateHeating(OFF); // Switch Relay/Heating OFF if temperature is above maximum temperature
  }
}


void CheckAndSetFrostTemperature()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  if (TimerState == "OFF" && ManualOverride == OFF)
  { // Only check for frost protection when heating is off
    if (Temperature < (FrostTemp - Hysteresis))
    {                     // Check if temperature is below Frost Protection temperature and hysteresis offset
      actuateHeating(ON); // Switch Relay/Heating ON if so
      Serial.println("Frost protection actuated...");
    }
    if (Temperature > (FrostTemp + Hysteresis))
    {                      // Check if temerature is above Frost Protection temperature and hysteresis offset
      actuateHeating(OFF); // Switch Relay/Heating OFF if so
    }
  }
}

boolean UpdateLocalTime()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  struct tm timeinfo;
  time_t now;
  char time_output[30];
  while (!getLocalTime(&timeinfo, 15000))
  { // Wait for up to 15-sec for time to synchronise
    return false;
  }
  time(&now);
  UnixTime = now;
  // See http://www.cplusplus.com/reference/ctime/strftime/
  strftime(time_output, sizeof(time_output), "%H:%M", &timeinfo); // Creates: '14:05'
  Time_str = time_output;
  strftime(time_output, sizeof(time_output), "%w", &timeinfo); // Creates: '0' for Sun
  DoW_str = time_output;

  Serial.println("Time: " + Time_str + " Day: " + DoW_str);
  return true;
}



void initDaysArray()
{
  Serial.println(__func__);
  Timer[0].DoW = "Sun";
  Timer[1].DoW = "Mon";
  Timer[2].DoW = "Tue";
  Timer[3].DoW = "Wed";
  Timer[4].DoW = "Thu";
  Timer[5].DoW = "Fri";
  Timer[6].DoW = "Sat";
}


void CheckTimerEvent()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  String TimeNow;
  UpdateTargetTemperature();
  TimeNow = ConvertUnixTime(UnixTime); // Get the current time e.g. 15:35
  TimerState = "OFF";                  // Switch timer off until decided by the schedule
  if (EarlyStart > 0)
  {                                                        // If early start is enabled by a value > 0
    TimeNow = ConvertUnixTime(UnixTime + EarlyStart * 60); // Advance the clock by the Early Start Duration
  }
  if (ManualOverride == ON)
  {                               // If manual override is enabled then turn the heating on
    //TargetTemp = ManOverrideTemp; // Set the target temperature to the manual overide temperature
    //ControlHeating();             // Control the heating as normal
  }
  else
  {
    for (byte dow = 0; dow < 7; dow++)
    { // Look for any valid timer events, if found turn the heating on
      for (byte p = 0; p < NumOfEvents; p++)
      {
        // Now check for a scheduled ON time, if so Switch the Timer ON and check the temperature against target temperature
        if (String(dow) == DoW_str && (TimeNow >= Timer[dow].Start[p] && TimeNow <= Timer[dow].Stop[p] && Timer[dow].Start[p] != ""))
        {
          TimerState = "ON";
          ControlHeating();
          ManualOverride = OFF; // If it was ON turn it OFF when the timer starts a controlled period
        }
      }
    }
  }
  CheckAndSetFrostTemperature();
}



boolean setupTime()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  configTime(0, 0, "time.nist.gov"); // (gmtOffset_sec, daylightOffset_sec, ntpServer)
  setenv("TZ", timezone, 1);         // setenv()adds "TZ" variable to the environment, only used if set to 1, 0 means no change
  tzset();
  delay(200);
  bool TimeStatus = UpdateLocalTime();
  return TimeStatus;
}



// Variable to save current epoch time
// unsigned long epochTime; 
// Function that gets current epoch time
// unsigned long getTime() {
  // time_t now;
  // struct tm timeinfo;
  // if (!getLocalTime(&timeinfo)) {
  //  Serial.println("Failed to obtain time");
    // return(0);
  // }
  // time(&now);
  // return now;
// }



#endif