#ifndef MENULIB_H
#define MENULIB_H

#include <LiquidCrystal_I2C.h>
// include encoder later

#define COLUMS              20  
#define ROWS                4    
#define LCD_SPACE_SYMBOL    0x20//no need, stored in lcd memory 

#define SCL_PIN             22 //should be in pins.h
#define SDA_PIN             21

#define SPEED_I2C           400000
#define TIME_I2C            250

typedef struct Line{
    String id;
    String content;
    Line* next;
    Line* prev;
}Line;

typedef struct Screen{
    String id; // Id is NUMBER, not string

    Line* descLine = NULL;
    Line* currentLine = NULL;
    Screen* next; //
    Screen* prev;
    
    Line*   (*getLine)(Screen* scr, String id);
    Line*   (*newLine)(Screen* scr, String content, String id);
    void    (*removeLine)(Screen* scr, String Id);
   
}Screen;

typedef struct Menu{
    LiquidCrystal_I2C lcd;

    Screen* currentScreen = NULL;
    void    (*printScreen)(Menu* menu);
    Screen* (*getScreen)    (Menu* menu, String id);
    Screen* (*newScreen)    (Menu* menu, String descript, String id);
    void    (*removeScreen) (Menu* menu, String id);

    void    (*nextLine)     (Menu* menu); //should be not in menu
    void    (*prevLine)     (Menu* menu);

    void    (*nextScreen)   (Menu* menu); //same
    void    (*prevScreen)   (Menu* menu);

    Line*   (*addLine)      (Menu* menu, String content, String id);
    String  (*getName)      (Menu* menu);

}Menu;

//GLOBALS

//Constructor
LiquidCrystal_I2C   initLCD();

Menu*               initMenu();
Screen*             initScreen(String id, String description, Screen* next, Screen* prev);
Line*               initLine(String id, String content, Line* next, Line* prev);

//Screen

Line*   getLine     (Screen* scr, String id);
Line*   newLine     (Screen* scr, String content, String id);
void    removeLine  (Screen* scr, String Id);

//Menu

void    printScreen (Menu* menu);
Screen* getScreen   (Menu* menu, String id);
Screen* newScreen   (Menu* menu, String descript, String id);
void    removeScreen(Menu* menu, String id);
void    nextLine    (Menu* menu);
void    prevLine    (Menu* menu);
Line*   addLine     (Menu* menu, String context, String id);
String  getName     (Menu* menu);

void nextScreen(Menu* menu);
void prevScreen(Menu* menu);



//destructor

#endif
