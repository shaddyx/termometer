#include <LiquidCrystal.h>

//LCD pin to Arduino
const int pin_RS = 8;
const int pin_EN = 9;
const int pin_d4 = 4;
const int pin_d5 = 5;
const int pin_d6 = 6;
const int pin_d7 = 7;
const int pin_BL = 10;
LiquidCrystal lcd( pin_RS,  pin_EN,  pin_d4,  pin_d5,  pin_d6,  pin_d7);
#define SERIESRESISTOR 18000
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define THERMISTORPIN A5
// R on 25 celsius
#define THERMISTORNOMINAL 93000
#define TEMPERATURENOMINAL 26

float temp = 0;
float r = 0;
void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0,0);
  lcd.print("shaddy.org.ua");
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(A5, INPUT);
  digitalWrite(A4, 1);
  delay(200);
}

void updateTemp(){
  lcd.setCursor(0,0);
  lcd.print("T:" + String(temp) + "               ");
  lcd.setCursor(0,1);
  lcd.print("R:" + String(r) + "               ");
}

float safeRead(){
	int samples[NUMSAMPLES];
	for (int i=0; i< NUMSAMPLES; i++) {
		samples[i] = analogRead(THERMISTORPIN);
		delay(10);
	}
	float average = 0;
	for (int i=0; i< NUMSAMPLES; i++) {
		average += samples[i];
	}
	average /= NUMSAMPLES;
	return average;
}

float getTemp(){
	float average = safeRead();
	float rr = 1023/average - 1;
	r = SERIESRESISTOR / rr;
	float steinhart;
	steinhart = r / THERMISTORNOMINAL; // (R/Ro)
	steinhart = log(steinhart); // ln(R/Ro)
	steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)
	steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart; // invert
	steinhart -= 273.15; // converting to celsius
	return steinhart;

}
// the loop function runs over and over again forever
void loop() {
  temp = getTemp();
  updateTemp();
}
