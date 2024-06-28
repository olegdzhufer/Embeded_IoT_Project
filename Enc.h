#ifndef ENC_H
#define ENC_H

#include <Arduino.h>
#include <EncButton.h>

#include "settings.h"
#include "pins.h"
#include "menu.h"



#ifdef ENC_S
  EncButton en(CLK, DT, SW);
  
bool updateTemp = false;



#if DEBUG
#define D_SerialBegin(...) Serial.begin(__VA_ARGS__);
#define D_print(...)       Serial.print(__VA_ARGS__)
#define D_write(...)       Serial.write(__VA_ARGS__)
#define D_println(...)     D_println(__VA_ARGS__)
#else
#define D_SerialBegin(bauds)
#define D_print(...)
#define D_write(...)
#define D_println(...)
#endif

#define MAX_TEMP_LIMIT 60

void encoder_setup(){
  
  en.setEncType(EB_STEP4_LOW);
  en.setBtnLevel(HIGH);
}

//int getResult(){
//  return enc_pre; 
//}

// bool flagEnc = false;


void read_encoder(){

  // int currtime;
  // if(!currtime)currtime = 0;
  // if( millis() > currtime + 10){
    // currtime += millis();
    
    en.tick();

  
    if(en.leftH()){
      if(menu.curr == Heat && TargetTemp > 0){
        updateTemp = true;
        TargetTemp -= 0.5;
      }else if(menu.curr == Cooling && FrostTemp > 0){
        updateTemp = true;
        FrostTemp -= 0.5;
        TempSetC->val->setfloat(TempSetC->val, FrostTemp);
        menu.lineUpdate(&menu, TempSetC);

      }
    }
    else if(en.rightH()){
      if(menu.curr == Heat && TargetTemp < MAX_TEMP_LIMIT){
        updateTemp = true;
        TargetTemp += 0.5;
        Serial.println(TargetTemp);
      }else if(menu.curr == Cooling && FrostTemp > 0){
        updateTemp = true;
        FrostTemp += 0.5;
        TempSetC->val->setfloat(TempSetC->val, FrostTemp);
        menu.lineUpdate(&menu, TempSetC);

      }
    }

    // else if (en.left()) {
    //     enc_pre = 0x03;
    //     D_println("3");
    // }
    // else if(en.right()){
    //     enc_pre = 0x04;
    //     D_println("4");
    // }
    //  else if (en.press())  
    // {
    //     enc_pre = 0x05;
    //     D_println("5");
    // }
  // }


  if(updateTemp){
    updateTemp = false;
    TempSetH->val->setfloat(TempSetH->val, TargetTemp);
    menu.lineUpdate(&menu, TempSetH);
  }
}

#endif



#endif
