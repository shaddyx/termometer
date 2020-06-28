#ifndef HEATER_SETTINGS_
#define HEATER_SETTINGS_
typedef struct _Settings{
    float targetTemp;
    long targetTime;
} Settings;

void settings_save(Settings temp);
Settings settings_load();
    
#endif