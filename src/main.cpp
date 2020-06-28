#include "config.h"
#include <Arduino.h>
#include <LiquidCrystal.h>
#include "temp.h"
#include "util.h"
#include "buttons.h"
#include <Ticker.h>
#include <LiquidMenu.h>
#include "settings_storage.h"
#include "heater.h"

// Indicator symbol definition, it will change the default symbol for the right focus.
// http://omerk.github.io/lcdchargen/
uint8_t rFocus[8] = {
	0b00000,
	0b00000,
	0b00100,
	0b01110,
	0b00100,
	0b00000,
	0b00000,
	0b00000
};

void updateMenu();
void updateTime();

LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
LiquidMenu menu(lcd);
Settings settings;
Heater heater;

float temp = 0;
int start_time = 0;
char statusText[30];
char secondStatusText[30];
char targetTimeText[20];
char startText[] = "Start";
char stopText[] = "Stop";
bool started = false;
bool baking = false;

char * getStatus(){
	char format[30];
	long t = millis() - start_time;
	dtostrf(temp, 4, 2, format);
	sprintf(statusText, "%s tm %s", format, timeToString(t).c_str());
	return statusText;
}

char * getSecondStatus(){
	String statusText = String();
	if (heater.get_signal()){
		statusText += "H ";
	}
	if (started){
		statusText += "S ";
	}
	if (baking){
		statusText += "B ";
	}
	sprintf(secondStatusText, "%s", statusText.c_str());
	return secondStatusText;
}

char * getTargetTime(){
	sprintf(targetTimeText, "%s", timeToString(settings.targetTime).c_str());
	return targetTimeText;
}

char * getStartStop(){
	return started ? stopText: startText;
}

LiquidLine time_control_value_line(0, 0, "time: ", getTargetTime);
LiquidLine temp_control_value_line(0, 1, "temp: ", settings.targetTemp);
LiquidLine start_line(0, 2, getStartStop);
LiquidLine main_status_line(0, 0, getStatus);
LiquidLine main_second_status_line(0, 1, getSecondStatus);
LiquidScreen main_screen(main_status_line, main_second_status_line);
LiquidScreen settings_screen(start_line, time_control_value_line, temp_control_value_line);

Ticker menuUpdateTimer(updateMenu, 300, 0, MILLIS);
Ticker timeUpdateTimer(updateTime, 100, 0, MILLIS);

void targetTimeUp(){
	Serial.println("Increasing time: " + String(settings.targetTime));
	settings.targetTime += TIME_INCREMENT;
	if (settings.targetTime > HOUR - MINUTE * 5){
		settings.targetTime = HOUR - MINUTE * 5;
	}
	Serial.println("Result: " + String(settings.targetTime));
}

void targetTimeDown(){
	Serial.println("Decreasing time:" + String(settings.targetTime));
	if (settings.targetTime < TIME_INCREMENT){
		settings.targetTime = 0;
		return;
	}
	Serial.println("Result: " + String(settings.targetTime));
	settings.targetTime -= TIME_INCREMENT;
}

void targetTempUp(){
	Serial.println("Increasing temp");
	settings.targetTemp += TEMP_INCREMENT;
	if (settings.targetTemp > MAX_TEMP){
		settings.targetTemp = MAX_TEMP;
	}
}

void targetTempDown(){
	Serial.println("Decreasing temp");
	if (settings.targetTemp < TEMP_INCREMENT){
		settings.targetTemp = 0;
		return;
	}
	settings.targetTemp -= TEMP_INCREMENT;
}

void start(){
	started = !started;
	Serial.println("Started: " + String (started));
}

void setupMenu(){
	menu.set_focusSymbol(Position::LEFT, rFocus);
	temp_control_value_line.attach_function(INC, targetTempUp);
	temp_control_value_line.attach_function(DEC, targetTempDown);
	time_control_value_line.attach_function(INC, targetTimeUp);
	time_control_value_line.attach_function(DEC, targetTimeDown);
	start_line.attach_function(1, start);
	main_screen.set_displayLineCount(LINE_COUNT);
	settings_screen.set_displayLineCount(LINE_COUNT);
	menu.add_screen(main_screen);
	menu.add_screen(settings_screen);
}

void setup() {
	btns_init();
	Serial.begin(115200);
	Serial.println("App Started");
	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	lcd.print("Starting up...");
	delay(200);

	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(A4, OUTPUT);
	pinMode(A5, INPUT);
	digitalWrite(A4, 1);
	setupMenu();
	//
	//	SETUP timers
	//
	menuUpdateTimer.start();
	timeUpdateTimer.start();
}

void updateButtons(){
	btns_update();
	if (btns_pressed(BTN_SELECT)){
		Serial.println("Select pressed");
		menu.next_screen();
	}
	if (btns_pressed(BTN_RIGHT)){
		Serial.println("Right pressed");
		menu.call_function(INC);
	}
	if (btns_pressed(BTN_LEFT)){
		Serial.println("Left pressed");
		menu.call_function(DEC);
	}
	if (btns_pressed(BTN_UP)){
		Serial.println("up pressed");
		menu.switch_focus(false);
	}
	if (btns_pressed(BTN_DOWN)){
		Serial.println("down pressed");
		menu.switch_focus(true);
		
	}
}

void updateMenu(){
	menu.update();
}
void updateTime(){
	//targetTime += 100;
	//menu.update();
}

void updateHeater(){
	heater.set_current(temp);
	heater.set_targetTemp(settings.targetTemp);
	heater.set_enabled(started);
}

// the loop function runs over and over again forever
void loop() {
	temp = getTemp();
	updateHeater();
	updateButtons();
	menuUpdateTimer.update();
	timeUpdateTimer.update();
}
