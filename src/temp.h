#ifndef HEATER_TEMP_
#define HEATER_TEMP_

#define SERIESRESISTOR 18000
#define NUMSAMPLES 5
#define BCOEFFICIENT 3950
#define THERMISTORPIN A5
// R on 25 celsius
#define THERMISTORNOMINAL 93000
#define TEMPERATURENOMINAL 26

float safeRead();
float getR();
float getTemp();
    
#endif