#include "HardwareSerial.h"
#ifndef CNTUPTMR
#define CNTUPTMR

#include <Arduino.h>
#include <Countimer.h>
#include "menu.h"

#ifdef TIMER_S

Countimer countimer;

char* convectorStrToChar(String text);

void onInterval() {
  String currentTime = countimer.getCurrentTime();
  char* textChar = convectorStrToChar(currentTime);
  Serial.println(textChar);


  if (footerLine) {
    footerLine->val->setChar(footerLine->val, textChar);
    menu.footerUpdate(&menu, footerLine->val);
  }
}
void startTimer() {
  countimer.start();
  menu.curr->footer = footerLine;
}

void stopTimer() {
  countimer.stop();
  menu.curr->footer = NULL;
  
}

void handleTimer(bool state) {
  if (state == HIGH) {
    startTimer();
  } else {
    stopTimer();
  }
}

void timer_setup() {
  countimer.setCounter(24, 60, 60, Countimer::COUNT_UP, NULL);
  countimer.setInterval(onInterval, 1000);
}


void timer_loop() {
  countimer.run();
}

char* convectorStrToChar(String text) {
  uint8_t lenText = text.length();

  char* res = (char*)malloc(lenText * sizeof(char) + 1);

  for (uint8_t iter = 0; iter < lenText; iter++) {
    res[iter] = text[iter];
  }
  res[lenText] = '\0';

  return res;
}
#endif

#endif