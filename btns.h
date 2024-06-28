#ifndef BTNS_DEF_H
#define BTNS_DEF_H

#include <Arduino.h>
#include <EncButton.h>
#include "menu.h"
#include "settings.h"
#include "relay.h"
#include "Pid.h"



bool fan = false;

class ButtonSwitch : public VirtButton
{
public:
  void (*callbackOnPress)() = NULL;

  ButtonSwitch() {    
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    }

  ButtonSwitch(uint8_t btnPin, uint8_t ledPin, uint8_t btnMode = INPUT_PULLUP, uint8_t btnLevel = LOW, uint8_t ledInitState = LOW, bool workEn =false)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    init(btnPin, ledPin, btnMode, btnLevel, ledInitState, workEn);
  }

  void init(uint8_t btnPin, uint8_t ledPin, uint8_t btnMode = INPUT_PULLUP, uint8_t btnLevel = LOW, uint8_t ledInitState = LOW, bool workEn =false)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    this->btnPin = btnPin;
    this->workEn = workEn;
    EB_mode(btnPin, btnMode);
    setBtnLevel(btnLevel);

    setLed(ledPin, ledInitState);
  }

  bool read()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    return EB_read(btnPin) ^ bf.read(EB_INV);
  }

  bool tick()
  {
    bool tick = VirtButton::tick(EB_read(btnPin));
    if (tick && this->workEn)
    {
      #ifdef DEBUG_FUNC
        Serial.println(__func__);
      #endif

    #ifdef DEBUG
      Serial.print("check");
    #endif
      pressedBtn();

      return true;
    }else{

    }
    return false;
  }

  void tickSwitch()
  {
    if (tick()&& this->workEn)
    {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
      switch (VirtButton::action())
      {

      case EB_HOLD:
        // FAN_ON;
        fan = true;
        toggleLed();
        if (this->relay != NULL)
        {
          relay->toggleFlag();
        }
        callCallback();
        #ifdef DEBUG
          Serial.println("HOLD");
        #endif
        
        break;

      case EB_RELEASE:
        // FAN_OFF;
        #ifdef DEBUG_FUNC
          Serial.println(__func__);
        #endif
        menu.curr = STOP;
        if(this->relay != NULL){
          relay->toggleFlag();
        }

        this->LedOff();
        fan = false;
        FLAG_LCD = true;

        #ifdef DEBUG
          Serial.println("RELEASE");
        #endif

        break;
      default:
        break;
      }
    }
  }

  bool tickRaw()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    return VirtButton::tickRaw(EB_read(btnPin));
  }

  void setLed(uint8_t ledPin, uint8_t ledState = LOW)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

    #ifdef DEBUG
      Serial.print("Led");
    #endif
    
    this->ledPin = ledPin;
    this->ledState = ledState;
    pinMode(ledPin, OUTPUT);
    digitalWrite(ledPin, ledState);
  }

  void toggleLed()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    ledState = !ledState;
    digitalWrite(ledPin, ledState);
  }

  void pressedBtn()
  {
    uint16_t btnState = VirtButton::action();
    // Serial.println("Action \n");
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

    switch (btnState)
    {

    case EB_CLICK:
      // Serial.println("click");
      toggleLed();
      if (relay != NULL)
      {
        relay->toggleFlag();
      }
      callCallback();
      FLAG_LCD = true;
      break;
    default:
      #ifdef DEBUG
        Serial.println("other action");
      #endif
      break;
      
    }
  }

  void attachCallback(void (*callback)())
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    this->callbackOnPress = callback;
  }

  void callCallback()
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    if (callbackOnPress != NULL)
    {
      callbackOnPress();
    }
  }

  bool attachRelay(Relay *relay)
  {
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    if (relay == NULL)
    {
      return false;
    }
    this->relay = relay;
    return true;
  }

  void LedOff(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    ledState = LOW;
    digitalWrite(ledPin, ledState);
  }
  void LedOn(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    ledState = HIGH;
    digitalWrite(ledPin, ledState);
  }

  void OffMode(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

    this->LedOff();

    if(this->relay != NULL){
      this->relay->relayOff();
    }
  }

  void EnWork(){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

    this->workEn = true;
  }
  void DeWork(){

    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif

    this->workEn = false;
    OffMode();
  }

private:
  uint8_t btnPin;
  uint8_t ledPin;
  uint8_t ledState;
  bool workEn = false;

  Relay *relay = NULL;
};
#ifdef BTN_S

ButtonSwitch btn1(BTN1_PIN, LED_PIN1, INPUT_PULLUP, LOW);
ButtonSwitch btn2(BTN2_PIN, LED_PIN2, INPUT_PULLUP, LOW);
ButtonSwitch btnSwitch(BTN3_PIN, LED_PIN3, INPUT_PULLUP, LOW);


void callbackBtn1()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  switch (btn1.action())
  {

  case EB_CLICK:
    btn2.OffMode();
    TempSetC->val->setfloat(TempSetC->val, FrostTemp); 
    menu.lineUpdate(&menu, TempCurC);


    break;

  default:
    break;
  }
}

void callbackBtn2()
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  switch (btn2.action())
  {
  
  case EB_CLICK:
    btn1.OffMode();

    
    break;

  default:
    break;
  }
}

void btnsSetup()
{
  #ifdef DEBUG_FUNC
    Serial.println(__func__);
  #endif
  Serial.println(__FILE__);

  btnSwitch.EnWork();
  
  btn1.attachCallback(callbackBtn1);
  btn2.attachCallback(callbackBtn2);


  #ifdef RELAY_S
    btn1.attachRelay(&relayHeat);
    btn2.attachRelay(&relayCool);
    btnSwitch.attachRelay(&relayFan);
  #endif
}

void btnsLoop()
{
  if(fan){
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    btn1.EnWork();
    btn2.EnWork();

  }else{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
    btn1.DeWork();
    btn2.DeWork();
    Heat->footer = NULL;
    Cooling->footer = NULL;
    stopTimer();

  }


  btn1.tick();
  btn2.tick();
  btnSwitch.tickSwitch();

}
#endif


#ifdef WEB_S

void heatControl(AsyncWebServerRequest *request)
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  String state = "OFF";
  if (request->hasParam("heat_state"))
  {
    String act_state = request->getParam("heat_state")->value();
    if (act_state == "1")
    {
      state = "ON";
      // HEAT_ON;
      request->send(200, "text/html", state);
    }
    else if (act_state == "0")
    {
      state = "OFF";
      // HEAT_OFF;
      request->send(200, "text/html", state);
    }
  }
}

void coldControl(AsyncWebServerRequest *request)
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  String state = "OFF";
  if (request->hasParam("cold_state"))
  {
    String act_state = request->getParam("cold_state")->value();
    if (act_state == "2")
    {
      state = "ON";
      // COOLING_ON;
      request->send(200, "text/html", state);
    }
    else if (act_state == "3")
    {
      state = "OFF";
      // COOLING_OFF;
      request->send(200, "text/html", state);
    }
  }
}

void fanControl(AsyncWebServerRequest *request)
{
    #ifdef DEBUG_FUNC
      Serial.println(__func__);
    #endif
  String state = "OFF";
  if (request->hasParam("fan_state"))
  {
    String act_state = request->getParam("fan_state")->value();
    if (act_state == "4")
    {
      state = "ON";
      // FAN_ON;
      request->send(200, "text/html", state);
    }
    else if (act_state == "5")
    {
      state = "OFF";
      // FAN_OFF;
      request->send(200, "text/html", state);
    }
  }
}

#endif


#endif