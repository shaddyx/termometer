#include "buttons.h"
#include <Arduino.h>

#define BOUNCE_DELAY 2000
#define PRESS_INTERVAL 700

void btns_clear();

unsigned long keydown[] = {0, 0, 0, 0, 0};
bool pressed[] = {0, 0, 0, 0, 0};
unsigned long last = 0;
void btns_init(){
    pinMode(A0, INPUT);
    btns_clear();
}

// read the buttons
int read_LCD_buttons()
{
 int adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return BTN_NONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return BTN_RIGHT;
 if (adc_key_in < 250)  return BTN_UP;
 if (adc_key_in < 450)  return BTN_DOWN;
 if (adc_key_in < 650)  return BTN_LEFT;
 if (adc_key_in < 850)  return BTN_SELECT;

 // For V1.0 comment the other threshold and use the one below:

//  if (adc_key_in < 50)   return BTN_RIGHT;
//  if (adc_key_in < 195)  return BTN_UP;
//  if (adc_key_in < 380)  return BTN_DOWN;
//  if (adc_key_in < 555)  return BTN_LEFT;
//  if (adc_key_in < 790)  return BTN_SELECT;

}

unsigned long interval_from_last(){
    return millis() - last;
}

void btns_clear(){
    for (int i=0; i<5; i++){
        keydown[i] = 0;
        pressed[i] = 0;
    }
}

void btns_update(){
    int btn = read_LCD_buttons();
    if (btn == BTN_NONE){
        btns_clear();
        return;
    }
    keydown[btn] += interval_from_last();
    last = millis();
}

bool btns_pressed(int button){
    if (keydown[button] && !pressed[button]){
        pressed[button] = 1;
        return true;
    }
    return false;
}