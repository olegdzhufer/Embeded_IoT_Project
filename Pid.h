#ifndef PID_H
#define PID_H
#include "settings.h"
#include "relay.h"

class simpPid{

  private:
    bool work = false;
    float* target;
    float temp;
    uint8_t pin;
    

  public:
    simpPid(){}

    simpPid(float* target, uint8_t pin){
      this->target = target;
      this->pin = pin;
    }
    void PidActivate(){
      this->work = true;
    }
    void PidDeactivate(){
      this->work = true;
    }

    void tempNow(float temp){
      this->temp = temp;
    }

    void tickPIDUp(){
      if((this->target && *(this->target) - 1.5 <= temp || this->temp >= 58 ) && this->work ){
        digitalWrite(this->pin, LOW);
      }else if(this->work){
        digitalWrite(this->pin, HIGH);
      }
    }
    
    void tickPIDDown(){
      if((this->target && *(this->target) <= temp || this->temp <= 1 ) && this->work ){
        digitalWrite(this->pin, HIGH);
      }else if(this->work){
        digitalWrite(this->pin, LOW);
      }
    }


    void setTemp(float* target){
      this->target = target;
    }

    void offPid(){
      this->work = false;
      digitalWrite(this->pin, LOW);
    }

    void switchPidMode(){
      this->work = !work;
    }

};

simpPid heatPid(&TargetTemp, HEAT_PIN);
simpPid coolPid(&FrostTemp,  COOL_PIN);

void setupPID(){

}

void loopPID(){




  if(Heat->footer != NULL){
    heatPid.PidActivate();
    heatPid.tempNow(Temperature);
    heatPid.tickPIDUp();
  }
  else{
    heatPid.PidDeactivate();
    relayHeat.relayOff();
  }


  if(Cooling->footer != NULL){
    coolPid.PidActivate();
  coolPid.tempNow(Temperature);
  coolPid.tickPIDDown();
  }
  else{
    coolPid.PidDeactivate();
    relayCool.relayOff();
  }
  
}


  

    


#endif