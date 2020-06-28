#ifndef HEATER_CONFIG_
#define HEATER_CONFIG_

#define MINUTE ((long) 60 * 1000)
#define HOUR (60 * MINUTE)
#define TIME_INCREMENT (MINUTE * 5)
#define TEMP_INCREMENT 10
#define MAX_TEMP 250
#define LINE_COUNT 2

//LCD pin to Arduino
#define INCR 1
#define DECR 2
#define SELECT 2

#define HEATER_PIN A1

#define pin_RS 8
#define pin_EN 9
#define pin_d4 4
#define pin_d5 5
#define pin_d6 6
#define pin_d7 7
#define pin_BL 10
#endif