#include <Arduino.h>
#include "config.h"
#include "temp.h"
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

float getR(){
	float average = safeRead();
	float rr = 1023/average - 1;
	return SERIESRESISTOR / rr;
}

float getTemp(){
	float steinhart;
	steinhart = getR() / THERMISTORNOMINAL; // (R/Ro)
	steinhart = log(steinhart); // ln(R/Ro)
	steinhart /= BCOEFFICIENT; // 1/B * ln(R/Ro)
	steinhart += 1.0 / (TEMPERATURENOMINAL + 273.15); // + (1/To)
	steinhart = 1.0 / steinhart; // invert
	steinhart -= 273.15; // converting to celsius
	return steinhart;

}