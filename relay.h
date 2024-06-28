#ifndef RELAY_H
#define RELAY_H

#include <Arduino.h>
#include "settings.h"
#include "menu.h"
#include "countTimer.h"

bool heatPidSt = false;
bool coolPidSt = false;

class Relay
{
private:
  uint8_t pin;
  uint8_t state = false;
  uint8_t changeFlag = false;
  Line* CurrLine = NULL;
  float* tempR = NULL;
  bool statusTemp = false;

public:
  bool allowed = true;
  bool isMain = false;
  bool isHeatOrCool = false;
  bool* pidState ;


  Screen *screen = NULL;

  Relay()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  }

  Relay(uint8_t pin, uint8_t initState = LOW, Screen* screen = NULL, bool isHeatOrCool = false)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    this->pin = pin;
    this->state = initState;
    this->isHeatOrCool = isHeatOrCool;

    pinMode(pin, OUTPUT);
    digitalWrite(pin, state);

    if(screen != NULL)
    {
      attachScreen(screen);
    }
  }

  void setLine(Line* line, float* temp){
    this->CurrLine = line;
    this->tempR = temp;
  }

  void setPidBool(bool* flag){
    this->pidState = flag;
  }

  void getLine(){
    if(this->CurrLine && this->tempR){
      temp = this->tempR;
      currLine = this->CurrLine;
      this->statusTemp = true;
    }
  }
  void setNull(){
    temp = NULL;
    currLine = NULL;
    this->statusTemp = false;
  }

  bool attachScreen(Screen* screen)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    if(screen != NULL)
    {
      this->screen = screen;
      return true;
    }
    return false;
  }

  void setMain(bool value)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    isMain = value;
  }

  void setAllowed(bool value)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    allowed = value;
  }

  void toggleFlag()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    changeFlag = !changeFlag;


  }

  void toggle()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    Serial.println(this->screen->name);
    state = !state;
    if(state){
      FrostTemp = 5;
    }else{
      FrostTemp = 0;
    }
    digitalWrite(pin, state);
    #ifdef TIMER_S
      if (isHeatOrCool && state == true) {
        menu.curr = this->screen;
        startTimer();
      } else if (isHeatOrCool && state == false) {
        stopTimer();
      }
    #endif
    if(this->pidState){
      *(this->pidState) = !*(this->pidState);
    }
  }

  bool workStatus(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    return this->state;
  }

  void tick()
  {

    if (allowed || isMain)
    {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

      if (isMain)
      {
        allowed = state;
      }

      if (changeFlag)
      {
        changeFlag = false;
        toggle();

        if(this->screen != NULL && this->workStatus()){
          menu.curr = this->screen;
          FLAG_LCD = true;
        }
        if(this->statusTemp){
          this->getLine();
        }else{
          this->setNull();
        }
        
      }
      
    }
    else
    {
      if (state == HIGH)
      {
        state = LOW;
        digitalWrite(pin, state);
      }
    }
  }

  void relayOff(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    temp = NULL;
    currLine = NULL;
    state = LOW;
    if(this->pidState){
      *(this->pidState) = false;
    }
    digitalWrite(pin, state);
  }
  void relayOn(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    if(this->pidState){
      *(this->pidState) = true;
    }
    state = HIGH;
    digitalWrite(pin, state);
  }


  ~Relay()
  {

    this->screen = NULL;
  }
};
#ifdef RELAY_S

Relay relayHeat(HEAT_PIN, LOW, Heat, true);
Relay relayCool(COOL_PIN, LOW, Cooling, true);
Relay relayFan(FAN_PIN, LOW, FAN);

void relaySetup()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  Serial.println(__FILE__);

  relayHeat.attachScreen(Heat);
  relayCool.attachScreen(Cooling);

  relayHeat.setPidBool(&heatPidSt);
  relayCool.setPidBool(&coolPidSt);

  relayFan.attachScreen(FAN);

  relayHeat.setLine(TempSetH, &TargetTemp);

  relayFan.setMain(true);
}

void relayTick()
{
  relayHeat.tick();
  relayCool.tick();
  relayFan.tick();
}

#endif


#endif