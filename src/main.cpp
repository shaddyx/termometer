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
int baking_start_time = 0;
char statusText[30];
char secondStatusText[30];
char targetTimeText[10];
char startText[] = "Start";
char stopText[] = "Stop";
char bakeText[] = "Bake";
bool started = false;
bool baking = false;

bool waitingForBakingStart(){
	return heater.get_ready() && started && !baking;
}

unsigned long getBakingTime(){
	return millis() - baking_start_time;
}

char * getStatus(){
	char format[30];
	dtostrf(temp, 4, 2, format);
	if (baking){
		sprintf(statusText, "t:%s->%s", format, timeToString(getBakingTime()).c_str());
	} else {
		sprintf(statusText, "t:%s", format);
	}
	return statusText;
}

char * getSecondStatus(){
	String statusText = String();
	if (!heater.is_error()){
		if (heater.get_signal()){
			statusText += "H ";
		}
		if (started){
			statusText += "S ";
		}
		if (baking){
			statusText += "B ";
		}
		if (waitingForBakingStart()){
			statusText += "W ";
		}
	} else {
		statusText += "Heater error";
	}
	
	sprintf(secondStatusText, "%s", statusText.c_str());
	return secondStatusText;
}

char * getTargetTime(){
	sprintf(targetTimeText, "%s", timeToString(settings.targetTime).c_str());
	return targetTimeText;
}

char * getStartStop(){
	if (waitingForBakingStart()){
		return bakeText;
	}
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

void startBaking(){
	if (!waitingForBakingStart()){
		return;
	}
	Serial.println("Started baking");
	baking = true;
	baking_start_time = millis();
}
void stop(){
	if (started){
		heater.set_enabled(false);
		baking_start_time = 0;
		start_time = 0;
		baking = false;
		started = false;
		Serial.println("Stopped");
	}
}
void start(){
	if (waitingForBakingStart()){
		startBaking();
		return;
	}
	if (started){
		stop();
	}
	started = !started;
	if (started){
		Serial.println("Started: " + String (started));
		settings_save(settings);
	} 
}

void setupMenu(){
	menu.set_focusSymbol(Position::LEFT, rFocus);
	temp_control_value_line.attach_function(INCR, targetTempUp);
	temp_control_value_line.attach_function(DECR, targetTempDown);
	time_control_value_line.attach_function(INCR, targetTimeUp);
	time_control_value_line.attach_function(DECR, targetTimeDown);
	start_line.attach_function(INCR, start);
	main_screen.set_displayLineCount(LINE_COUNT);
	settings_screen.set_displayLineCount(LINE_COUNT);
	menu.add_screen(main_screen);
	menu.add_screen(settings_screen);
}

void setup() {
	btns_init();
	settings = settings_load();
	Serial.begin(115200);
	Serial.println("App Started");
	
	pinMode(HEATER_PIN, OUTPUT);
	digitalWrite(HEATER_PIN, 0);
	pinMode(LED_BUILTIN, OUTPUT);
	pinMode(THERMISTORPIN, INPUT);
	pinMode(THERMISTOR_PLUS_PIN, OUTPUT);
	digitalWrite(THERMISTOR_PLUS_PIN, 1);
	pinMode(THERMISTOR_GROUND_PIN, OUTPUT);
	digitalWrite(THERMISTOR_GROUND_PIN, 0);
	lcd.begin(16, 2);
	lcd.setCursor(0,0);
	lcd.print("Starting up...");
	
	delay(200);
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
		menu.call_function(INCR);
	}
	if (btns_pressed(BTN_LEFT)){
		Serial.println("Left pressed");
		menu.call_function(DECR);
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
void checkError(){
	if (heater.is_error()){
		stop();
	}
}

void checkBaking(){
	if (baking && getBakingTime() >= settings.targetTime){
		stop();
	}
}

void updateHeater(){
	heater.set_current(temp);
	heater.set_targetTemp(settings.targetTemp);
	heater.set_enabled(started);
	digitalWrite(HEATER_PIN, heater.get_signal());
}

// the loop function runs over and over again forever
void loop() {
	temp = getTemp();
	updateHeater();
	updateButtons();
	checkBaking();
	checkError();
	menuUpdateTimer.update();
	timeUpdateTimer.update();
}
