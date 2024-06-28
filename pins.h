#ifndef PINS_DEF_H
#define PINS_DEF_H

#define BTN1_PIN 32
#define BTN2_PIN 33
#define BTN3_PIN 25

// #define LED_PIN1 18
// #define LED_PIN2 5
// #define LED_PIN3 17
#define LED_PIN1 19
#define LED_PIN2 4
#define LED_PIN3 2

#define CLK 18                     
#define DT  5                      
#define SW  17                       

#define REL1 26
#define REL2 27
#define REL3 14 

#define HEAT_PIN REL1
#define COOL_PIN REL2
#define FAN_PIN REL3


// #define V33 12

#define ONE_WIRE_BUS 13

#define RelayPIN        16              // Define the Relay Control pin
#define LEDPIN          4              // Define the LED Control pin
#define RelayReverse    false          // Set to true for Relay that requires a signal LOW for ON


#define ENC_BTN_PIN CLK
#define ENC_R_PIN DT
#define ENC_L_PIN SW

#endif
