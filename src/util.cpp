#include <Arduino.h>
String timeToString(unsigned long t)
{
    t = t / 1000;
    static char str[6];
    long h = t / 3600;
    t = t % 3600;
    int m = t / 60;
    int s = t % 60;
    sprintf(str, "%02d:%02d", m, s);
    return String(str);
}
